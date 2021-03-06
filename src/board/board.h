/*
 * board.h
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

#pragma once

#include <stdbool.h>
#include "kestrel.h"
#include "board.h"


#define	BOARD_EMPTY	((uint64_t)0x0ull)

#define NUM_RANKS 	8
#define NUM_FILES	8

enum rank {
    RANK_1 = 0, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum file {
    FILE_A = 0, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

#define GET_PIECE_MASK(square)		((uint64_t)(0x01ull << (int)(square)))

#define IS_VALID_RANK(rank)		((rank >= RANK_1) && (rank <= RANK_8))
#define IS_VALID_FILE(file)		((file >= FILE_A) && (file <= FILE_H))
#define IS_VALID_SQUARE(sq)		((sq >= a1) && (sq <= h8))





//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 04 05 06 07
// board directions
enum {
    NORTH 	= 8,
    NN 		= (NORTH + NORTH),
    SOUTH 	= -8,
    SS 		= (SOUTH + SOUTH),
    EAST 	= 1,
    WEST 	= -1,
    NE 		= 9,
    SW 		= -9,
    NW 		= 7,
    SE 		= -7
};


// castling permissions
enum castle_perm {
	CASTLE_PERM_NONE = 0,
    WKCA = 0x01,		// white, king-side
    WQCA = 0x02,		// white, queen-side
    BKCA = 0x04,		// black, king-side
    BQCA = 0x08			// black, queen-side
};




void move_piece(struct position *pos, enum square from, enum square to);
void remove_piece_from_board(struct position *pos,  enum piece pce_to_remove, enum square sq);
void add_piece_to_board(struct position *pos, enum piece pce, enum square sq);
int32_t get_material_value(const struct position *pos, enum colour col);
enum square get_king_square(const struct position *pos, enum colour col);

enum colour get_side_to_move(const struct position *pos);
void set_side_to_move(struct position *pos, enum colour side);

void set_castle_permission(struct position *pos, enum castle_perm perm);
enum castle_perm get_castle_permissions(const struct position *pos);

void set_en_passant_sq(struct position *pos, enum square sq);
enum square get_en_passant_sq(const struct position *pos);

void update_board_hash(struct position *pos);
uint64_t get_board_hash(const struct position *pos);

enum piece get_piece_on_square(const struct position *pos, enum square sq);

void push_history(struct position *pos, mv_bitmap move);
mv_bitmap pop_history(struct position *pos);

uint8_t get_ply(const struct position *pos);
void set_ply(struct position *pos, uint8_t ply);

uint8_t get_history_ply(const struct position *pos);
void set_history_ply(struct position *pos, uint8_t hist_ply);

void assert_boards_are_equal(const struct position *pos1, const struct position *pos2);

mv_bitmap get_best_pvline(const struct position *pos);
mv_bitmap get_pvline(const struct position *pos, uint8_t search_depth);
void set_pvline(struct position *pos, uint8_t search_depth, mv_bitmap move);
uint8_t populate_pv_line(struct position *pos, uint8_t depth);




uint8_t get_fifty_move_counter(const struct position *pos);

bool make_move(struct position *pos, mv_bitmap mv);
void take_move(struct position *pos);
void flip_sides(struct position *pos);

bool is_pawn_controlling_sq(const struct position *pos, enum colour col, enum square sq);
uint8_t get_num_pawns_on_rank(const struct position *pos, enum colour col, enum rank rank);
uint8_t get_num_pawns_on_file(const struct position *pos, enum colour col, enum file file);
uint8_t get_num_squares_under_pawn_ctl(const struct position *pos, enum colour col, enum square sq);



void set_piece_material(struct position *pos);
void overlay_boards(struct position *the_board);
bool is_piece_on_square(const struct position *pos, enum piece pce, enum square sq);
bool is_square_occupied(uint64_t bitboard, enum square sq);
bool is_repetition(const struct position *pos);

void init_search_history(struct position *pos);
void init_search_killers(struct position *pos);

mv_bitmap get_search_killer(struct position *pos, uint8_t killer_move_num, uint8_t ply);
uint32_t get_search_history(struct position *pos, enum piece pce, enum square sq);
void add_to_search_history(struct position *pos, enum piece pce, enum square to_sq, uint8_t depth);



void shuffle_search_killers(struct position *pos, mv_bitmap mv);

const struct bitboards * get_bitboard_struct(const struct position *pos);

struct position* init_game(char *fen);

struct position* allocate_board(void);
void free_board(struct position *pos);



uint8_t get_rank(enum square sq);
uint8_t get_file(enum square sq);
enum square get_square(enum rank r, enum file f);
void clone_board(const struct position *board_to_clone, struct position *cloned);
