/*
 * board.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating the chess board
 * ---------------------------------------------------------------------
 *
 *
 *
 *
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * kestrel is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kestrel is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "fen.h"
#include "board.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "board_utils.h"
#include "tt.h"
#include "makemove.h"
#include "hashkeys.h"
#include "pieces.h"



static inline void add_black_pawn_info(struct board *brd, enum square sq);
static inline void add_white_pawn_info(struct board *brd, enum square sq);
static inline void remove_black_pawn_info(struct board *brd, enum square sq);
static inline void remove_white_pawn_info(struct board *brd, enum square sq);
static inline void update_black_pawn_control(struct board *brd, enum square sq, int val);
static inline void update_white_pawn_control(struct board *brd, enum square sq, int val);


/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param fen : the FEN string
 * @return	a new board
 *
 */
void init_board(char *fen, struct board *brd)
{
    get_clean_board(brd);
    consume_fen_notation(fen, brd);
    
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif

}

/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
void get_clean_board(struct board *brd)
{
    memset(brd, 0, sizeof(struct board));

    for(uint8_t i = 0; i < NUM_SQUARES; i++) {
        brd->pieces[i] = NO_PIECE;
    }

    brd->king_sq[WHITE] = NO_SQUARE;
    brd->king_sq[BLACK] = NO_SQUARE;

    brd->side_to_move = WHITE;
    brd->en_passant = NO_SQUARE;

    for(uint16_t i = 0; i < MAX_SEARCH_DEPTH; i++) {
        brd->pv_line[i] = NO_MOVE;
    }

    for(uint16_t i = 0; i < MAX_GAME_MOVES; i++) {
        brd->history[i].move = NO_MOVE;
        brd->history[i].en_passant = NO_SQUARE;
        // other struct values are already set to zero with memset
    }
}

// returns the count.
uint8_t populate_pv_line(struct board *brd, uint8_t depth)
{

    mv_bitmap mv = probe_tt(brd->board_hash);

    uint8_t count = 0;

    while((mv != NO_MOVE) && (count < depth)) {

        //assert(count < MAX_SEARCH_DEPTH);

        make_move(brd, mv);
        brd->pv_line[count++] = mv;

        mv = probe_tt(brd->board_hash);
    }

    // rollback moves
    while(brd->ply > 0) {
        take_move(brd);
    }

    return count;
}



inline uint64_t overlay_white_piece_bitboards(const struct board * brd)
{
    return brd->bitboards[W_PAWN] | brd->bitboards[W_BISHOP]
           | brd->bitboards[W_KNIGHT] | brd->bitboards[W_ROOK]
           | brd->bitboards[W_QUEEN] | brd->bitboards[W_KING];
}

inline uint64_t overlay_black_piece_bitboards(const struct board * brd)
{
    return brd->bitboards[B_PAWN] | brd->bitboards[B_BISHOP]
           | brd->bitboards[B_KNIGHT] | brd->bitboards[B_ROOK]
           | brd->bitboards[B_QUEEN] | brd->bitboards[B_KING];
}


inline bool is_piece_on_square(const struct board *brd, enum piece pce, enum square sq)
{
    enum piece on_board = brd->pieces[sq];
    return (pce == on_board);
}


inline uint64_t square_to_bitboard(enum square sq)
{
    uint64_t retval = 0;
    set_bit(&retval, sq);
    return retval;
}


void move_piece(struct board *brd, enum square from, enum square to)
{	
    enum piece pce = brd->pieces[from];

#ifdef ENABLE_ASSERTS
	assert(pce != NO_PIECE);
	printf("------------_> pce = %c\n", get_piece_label(pce));
	assert(from != to);
	ASSERT_BOARD_OK(brd);
	assert_material_correct(brd);
#endif
	
    // adjust the hash
    brd->board_hash ^= get_piece_hash(pce, from);
    brd->board_hash ^= get_piece_hash(pce, to);

    brd->pieces[from] = NO_PIECE;
    clear_bit(&brd->bitboards[pce], from);
    clear_bit(&brd->board, from);

    // set up the "to" resources
    brd->pieces[to] = pce;
    set_bit(&brd->bitboards[pce], to);
    set_bit(&brd->board, to);

	switch(pce){
		case W_PAWN:
            // easiest way to move a pawn
            remove_white_pawn_info(brd, from);
            add_white_pawn_info(brd, to);
			break;
		case B_PAWN:
            // easiest way to move a pawn
            remove_black_pawn_info(brd, from);
            add_black_pawn_info(brd, to);
            break;
		case W_KING:
            brd->king_sq[WHITE] = to;
			break;
		case B_KING:
			brd->king_sq[BLACK] = to;
			break;
		default:
			break;
    }

#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
	assert_material_correct(brd);
#endif
}



void add_piece_to_board(struct board *brd, enum piece pce, enum square sq)
{
#ifdef ENABLE_ASSERTS
    assert(pce != NO_PIECE);
    assert(IS_VALID_PIECE(pce));
    assert(IS_VALID_SQUARE(sq));
#endif


    enum colour col = GET_COLOUR(pce);
    brd->board_hash ^= get_piece_hash(pce, sq);

    brd->pieces[sq] = pce;
    brd->material[col] += GET_PIECE_VALUE(pce);

    // set piece on bitboards
    set_bit(&brd->bitboards[pce], sq);
    set_bit(&brd->board, sq);

    switch (pce) {
    case W_PAWN:
        add_white_pawn_info(brd, sq);
        break;
    case B_PAWN:
        add_black_pawn_info(brd, sq);
        break;
    case W_KING:
    case B_KING:
        brd->king_sq[col] = sq;
        break;
    default:
        break;
    }

}



void remove_piece_from_board(struct board *brd, enum piece pce_to_remove, enum square sq)
{
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
    assert(pce_to_remove != NO_PIECE);
    assert(IS_VALID_PIECE(pce_to_remove));
	assert(IS_VALID_SQUARE(sq));
	
	enum piece pce_on_sq = brd->pieces[sq];
	assert(pce_on_sq != NO_PIECE);
	if (pce_on_sq != pce_to_remove){
		printf("*****");
	}
	
	assert(pce_on_sq == pce_to_remove);
	
#endif

    enum colour col = GET_COLOUR(pce_to_remove);
    brd->board_hash ^= get_piece_hash(pce_to_remove, sq);
    brd->pieces[sq] = NO_PIECE;
    brd->material[col] -= GET_PIECE_VALUE(pce_to_remove);

    // remove piece from bitboards
    clear_bit(&brd->bitboards[pce_to_remove], sq);
    clear_bit(&brd->board, sq);

    switch (pce_to_remove) {
    case W_PAWN:
        remove_white_pawn_info(brd, sq);
        break;
    case B_PAWN:
        remove_black_pawn_info(brd, sq);
        break;
    case W_KING:
    case B_KING:
        brd->king_sq[col] = NO_SQUARE;
        break;
    default:
        break;
    }
    
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif
}




static inline void remove_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[BLACK][file]--;
    brd->pawns_on_rank[BLACK][rank]--;

    update_black_pawn_control(brd, sq, -1);
}



static inline void remove_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]--;
    brd->pawns_on_rank[WHITE][rank]--;

    update_white_pawn_control(brd, sq, -1);

}




// adds a pawn to the underlying board struct
static inline void add_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[BLACK][file]++;
    brd->pawns_on_rank[BLACK][rank]++;

    update_black_pawn_control(brd, sq, 1);
}



// adds a pawn to the underlying board struct
static inline void add_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]++;
    brd->pawns_on_rank[WHITE][rank]++;

    update_white_pawn_control(brd, sq, 1);

}



static inline void update_black_pawn_control(struct board *brd, enum square sq, int val)
{
    int32_t next_sq = 0;
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    if (file > FILE_A) {
        if (rank > RANK_1) {
            next_sq = (int32_t)sq + SW;
            brd->pawn_control[BLACK][next_sq] += val;
        }
    }
    if (file < FILE_H) {
        if (rank > RANK_1) {
            next_sq = (int32_t)sq + SE;
            brd->pawn_control[BLACK][next_sq] += val;
        }
    }
}

static inline void update_white_pawn_control(struct board *brd, enum square sq, int val)
{

    int32_t next_sq = 0;
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    if (file > FILE_A) {
        if (rank < RANK_8) {
            next_sq = sq + NW;
            brd->pawn_control[WHITE][next_sq] += val;
        }
    }
    if (file < FILE_H) {
        if (rank < RANK_8) {
            next_sq = sq + NE;
            brd->pawn_control[WHITE][next_sq] += val;
        }
    }
}




/*
 * Counts set bits in a uint64_t
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 *
 */
inline uint8_t count_bits(uint64_t bb)
{
    return (uint8_t) __builtin_popcountll(bb);
}
