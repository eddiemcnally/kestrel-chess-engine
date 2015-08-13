/*
 * attack.c
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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "board.h"
#include "board_utils.h"
#include "move.h"
#include "attack.h"
#include "occupancy_mask.h"

static bool search_horizontally(const struct board *brd, enum square sq1, enum square sq2);
static bool search_vertically(const struct board *brd, enum square sq1, enum square sq2);
static bool search_up_left_and_right(const struct board *brd, enum square sq_one, enum square sq_two);
static bool search_down_left_and_right(const struct board *brd, enum square sq_one, enum square sq_two);
static bool is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two);
static bool is_attacked_diagonally(const struct board *brd, enum square sq_one, enum square sq_two);


/*
 * Checks to see if a given square is being attacked by
 * any piece on the board
 * name: is_sq_attacked
 * @param
 * @return
 *
 */

bool is_sq_attacked(const struct board *brd, enum square sq, enum colour attacking_side)
{
    ASSERT_BOARD_OK(brd);

	//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	//printf("checking sq %s being attacked\n", print_square(sq));

	// TODO:
	// combine rook/queen and bishop/queen to reduce number of lookups

    if (is_knight_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W Knight attacking %s\n", print_square(sq));
		//} else{
			//printf("B Knight attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_pawn_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W PAWN attacking %s\n", print_square(sq));
		//} else{
			//printf("B PAWN attacking %s\n", print_square(sq));
		//}

		return true;
	}

    if (is_king_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W KING attacking %s\n", print_square(sq));
		//} else{
			//printf("B KING attacking %s\n", print_square(sq));
		//}

		return true;
	}
    if (is_queen_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W QUEEN attacking %s\n", print_square(sq));
		//} else{
			//printf("B QUEEN attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_bishop_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W BISHOP attacking %s\n", print_square(sq));
		//} else{
			//printf("B BISHOP attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_rook_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W ROOK attacking %s\n", print_square(sq));
		//} else{
			//printf("B ROOK attacking %s\n", print_square(sq));
		//}
		return true;
	}

    return false;
}



//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

static inline bool is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_KNIGHT;
    else
		attacking_piece = B_KNIGHT;

    // get the bitboard representing all knights on the board of
    // this colour
    U64 bbKnight = brd->bitboards[attacking_piece];

    while (bbKnight != 0) {
		enum square att_pce_sq = POP(&bbKnight);

		// get occupancy mask for this piece and square
		U64 mask = GET_KNIGHT_OCC_MASK(att_pce_sq);

		if ((mask & sqBB) != 0) {
			// a Knight is attacking this square
			return true;
		}
    }
    return false;
}

static bool is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_BISHOP;
    else
		attacking_piece = B_BISHOP;

    // get the bitboard representing all bishops on the board of
    // this colour
    U64 bbBishop = brd->bitboards[attacking_piece];

    while (bbBishop != 0) {
		enum square att_pce_sq = POP(&bbBishop);

		// get occupancy mask for this piece and square
		U64 mask = GET_BISHOP_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a bishop is possibly attacking this square
			// search for any blocking pieces
			bool attacked = is_attacked_diagonally(brd, sq, att_pce_sq);
			if (attacked){
				//printf("not blocked between %s and %s\n", print_square(sq), print_square(att_pce_sq));
				return true;
			}
		}
    }

	//printf("not attacking %s\n", print_square(sq));
    return false;
}

static bool is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_ROOK;
    else
		attacking_piece = B_ROOK;

    // get the bitboard representing all rooks on the board of
    // this colour
    U64 bbRook = brd->bitboards[attacking_piece];

    // check all rooks
    while (bbRook != 0) {
		enum square att_pce_sq = POP(&bbRook);

		// get occupancy mask for this piece and square
		U64 mask = GET_ROOK_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a Rook is possibly attacking this square
			// search for any blocking pieces
			bool attacked = is_attacked_horizontally_or_vertically(brd, sq, att_pce_sq);
			if (attacked){
				return true;
			} else {
				// keep looking
				continue;
			}
		}
    }
    return false;
}

static inline bool is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    enum piece attacking_piece;

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    // ------------------------
    // check which pawn
    // ------------------------
    if (attacking_side == WHITE)
		attacking_piece = W_PAWN;
    else
		attacking_piece = B_PAWN;

    // get the bitboard representing all pawns on the board of
    // this colour
    U64 bbPawn = brd->bitboards[attacking_piece];
    while (bbPawn != 0) {
		enum square att_pce_sq = POP(&bbPawn);

		// get occupancy mask for this piece
		U64 mask = 0;
		if (attacking_side == WHITE)
			mask = GET_WHITE_PAWN_OCC_MASK(att_pce_sq);
		else
			mask = GET_BLACK_PAWN_OCC_MASK(att_pce_sq);

		if (mask & sqBB) {
			// a pawn is attacking this square
			return true;
		}
	}
    return false;
}

static inline bool is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_QUEEN;
    else
		attacking_piece = B_QUEEN;

    // get the bitboard representing all queens on the board of
    // this colour
    U64 bbQueen = brd->bitboards[attacking_piece];

    // check *all* queens to see if blocked.
    while (bbQueen != 0) {
		enum square att_pce_sq = POP(&bbQueen);

		//if (attacking_side == WHITE)
			//printf("checking W_QUEEN on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));
		//else
			//printf("checking B_QUEEN on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));


		// get occupancy mask for this square
		U64 mask = GET_QUEEN_OCC_MASK(att_pce_sq);

		if (mask & sqBB) {
			// the queen is potentially attacking this square.
			// Need to see if any blocking pieces

			if (is_attacked_diagonally(brd, sq, att_pce_sq)) {
				//printf("Queen on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));
				return true;
			}
			if (is_attacked_horizontally_or_vertically(brd, sq, att_pce_sq)) {
				//printf("Queen on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));
				return true;
			}
		}
    }

    return false;

}

static inline bool is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    // ------------------------
    // check king
    // ------------------------
    if (attacking_side == WHITE)
		attacking_piece = W_KING;
    else
		attacking_piece = B_KING;

    // get the bitboard representing the king on the board of
    // this colour
    U64 bbPawn = brd->bitboards[attacking_piece];
    while (bbPawn != 0) {
		enum square att_pce_sq = POP(&bbPawn);

		// get occupancy mask for this square
		U64 mask = GET_KING_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a king is attaching this square
			return true;
		}
    }
    return false;
}

static inline bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two)
{

    int sq_one_rank = GET_RANK(sq_one);
    int sq_one_file = GET_FILE(sq_one);

    int sq_two_rank = GET_RANK(sq_two);
    int sq_two_file = GET_FILE(sq_two);

    if (sq_one_rank == sq_two_rank) {
		bool attacked = search_horizontally(brd, sq_one, sq_two);
		return attacked;
    } else if (sq_one_file == sq_two_file) {
		bool attacked = search_vertically(brd, sq_one, sq_two);
		return attacked;
    }
    return false;
}

/*
 * Searches along the rank between the squares to see if the path is
 * blocked by a piece
 * name: search_horizontally
 * @param
 * @return : true if sq2 can be attacked from sq1, false otherwise
 *
 */
static inline bool search_horizontally(const struct board *brd, enum square sq1, enum square sq2)
{

    int s_rank = GET_RANK(sq1);
    int s_file = GET_FILE(sq1);

    int e_file = GET_FILE(sq2);

    // search horizontally
    if (s_file < e_file) {
		// search left
		for (int i = s_file + 1; i < e_file; i++) {
			enum square s = GET_SQUARE(s_rank, i);
			if (is_square_occupied(brd->board, s))
				return false;
			}
	} else {
		// search right
		for (int i = e_file + 1; i < s_file; i++) {
			enum square s = GET_SQUARE(s_rank, i);
			if (is_square_occupied(brd->board, s))
				return false;
		}
	}
    return true;
}

/*
 * Searches along the file between the squares to see if the path is
 * blocked by a piece
 * name: search_vertically
 * @param
 * @return : true if sq2 can be attacked from sq1, false otherwise
 *
 */
static inline bool search_vertically(const struct board *brd, enum square start_sq, enum square end_sq)
{

    int s_rank = GET_RANK(start_sq);
    int s_file = GET_FILE(start_sq);

    int e_rank = GET_RANK(end_sq);

    // search vertically
    if (s_rank < e_rank) {
		// search up
		for (int i = s_rank + 1; i < e_rank; i++) {
			enum square s = GET_SQUARE(i, s_file);
			if (is_square_occupied(brd->board, s))
				return false;
		}
    } else {
		// search down
		for (int i = e_rank + 1; i < s_rank; i++) {
			enum square s = GET_SQUARE(i, s_file);
			if (is_square_occupied(brd->board, s))
				return false;
		}
    }
    return true;

}

static inline bool is_attacked_diagonally(const struct board * brd, enum square start_sq, enum square end_sq)
{

    assert(start_sq != end_sq);

	//printf("checking if square %s can attack square %s\n", print_square(start_sq), print_square(end_sq));

    if (start_sq < end_sq) {
		bool attacked = search_up_left_and_right(brd, start_sq, end_sq);
		return attacked;
    } else {
		bool attacked = search_down_left_and_right(brd, start_sq, end_sq);
		return attacked;
    }
}

/*
 * Searches diagonally up left and right, seeing of the path is blocked
 * between the two squares.
 *
 * name: search_up_left_and_right
 * @param
 * @return : true is square is being attacked, false otherwise
 *
 */
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

static inline bool search_up_left_and_right(const struct board *brd, enum square start_sq, enum square end_sq)
{
	// check north-east
    if (((end_sq - start_sq) % 9) == 0) {
		//printf("searching NE\n");
		// search up and right from sq_one
		enum square sq = start_sq + 9;
		int sq_rank = GET_RANK(sq);
		int sq_file = GET_FILE(sq);

		while ((sq <= end_sq) && (sq_file <= FILE_H) && (sq_rank <= RANK_8)) {
			//printf("checking %s\n", print_square(sq));

			if (is_square_occupied(brd->board, sq)) {
				if (sq == end_sq){
					// attacked
					return true;
				} else {
					// blocked by piece
					//printf("square blocking...%s\n", print_square(sq));
					return false;
				}
			}
			sq += 9;
			sq_rank = GET_RANK(sq);
			sq_file = GET_FILE(sq);
		}
    } else if (((end_sq - start_sq) % 7) == 0) {
		// search north-west
		//printf("searching NW\n");
		enum square sq = start_sq + 7;
		int sq_rank = GET_RANK(sq);
		int sq_file = GET_FILE(sq);

		while ((sq <= end_sq) && (sq_file >= FILE_A) && (sq_rank <= RANK_8)) {
			//printf("checking %s\n", print_square(sq));

			if (is_square_occupied(brd->board, sq)) {
				if (sq == end_sq){
					// attacked
					return true;
				} else {
					// blocked by piece
					//printf("square blocking...%s\n", print_square(sq));
					return false;
				}
			}
			sq += 7;
			sq_rank = GET_RANK(sq);
			sq_file = GET_FILE(sq);
		}
    } else {
		// not on a line of sight....ignore
		//printf("not line of sight....\n");
		return false;
	}
	//printf("DEFAULT FALSE.......\n");
	return false;
}

/*
 * Searches diagonally down left and right, seeing of the path is blocked
 * between the two squares.
 *
 * name: search_up_left_and_right
 * @param
 * @return : true is square is being attacked, false otherwise
 *
 */
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
static inline bool search_down_left_and_right(const struct board *brd, enum square start_sq, enum square end_sq)
{
	// search south-west
    if (((start_sq - end_sq) % 9) == 0) {
		// search down and left from sq_one
		//printf("searching SW\n");
		enum square sq = start_sq - 9;
		int sq_rank = GET_RANK(sq);
		int sq_file = GET_FILE(sq);

		while ((sq >= end_sq) && (sq_rank >= RANK_1) && (sq_file >= FILE_A)) {
			//printf("checking %s\n", print_square(sq));

			if (is_square_occupied(brd->board, sq)) {
				if (sq == end_sq){
					// attacked
					return true;
				} else {
					// blocked by piece
					//printf("square blocking...%s\n", print_square(sq));
					return false;
				}
			}
			sq -= 9;
			sq_rank = GET_RANK(sq);
			sq_file = GET_FILE(sq);
		}
    } else if (((start_sq - end_sq) % 7) == 0) {
		// search south-east
		//printf("searching SE\n");
		enum square sq = start_sq - 7;
		int sq_rank = GET_RANK(sq);
		int sq_file = GET_FILE(sq);

		//printf("checking %s\n", print_square(sq));

		while ((sq >= end_sq) && (sq_file <= FILE_H) && (sq_rank >= RANK_1)) {

			//printf("----------------\n");
			//print_board(brd);
			//printf("----------------\n");
			if (is_square_occupied(brd->board, sq)) {
				if (sq == end_sq){
					// attacked
					return true;
				} else {
					// blocked by piece
					//printf("square blocking...%s\n", print_square(sq));
					return false;
				}
			}
			sq -= 7;
			sq_rank = GET_RANK(sq);
			sq_file = GET_FILE(sq);
		}
    } else {
		// not on a line of sight....ignore
		return false;
	}
    return false;
}


/**
 * Test wrapper functions.
 * These wrapper functions facilitate access to unit test framework while
 * allowing the functions themselves to be static to this file and inline-able
 */
bool TEST_is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_bishop_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_knight_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_pawn_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_king_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_rook_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_queen_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_horizontally_or_vertically(brd, sq_one, sq_two);
}
bool TEST_is_attacked_diagonally(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_diagonally(brd, sq_one, sq_two);
}
