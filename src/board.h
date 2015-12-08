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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>
#include "types.h"
#include "board.h"

#define	BOARD_EMPTY	((uint64_t)0x0ull)

#define NUM_RANKS 	8
#define NUM_FILES	8

enum {
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum {
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

#define GET_PIECE_MASK(square)		((uint64_t)(0x01ull << (int)(square)))

// zero-based
#define GET_RANK(square)			((uint8_t)((square) >> 3))
#define GET_FILE(square)			((uint8_t)((square) % 8))
#define GET_SQUARE(RANK, FILE)		((uint8_t)((RANK << 3) + FILE))

#define IS_VALID_RANK(rank)		((rank >= RANK_1) && (rank <= RANK_8))
#define IS_VALID_FILE(file)		((file >= FILE_A) && (file <= FILE_H))
#define IS_VALID_SQUARE(sq)		((sq >= a1) && (sq <= h8))


// castling permissions
enum {
	WKCA = 0x01,		// white, king-side
	WQCA = 0x02,		// white, queen-side
	BKCA = 0x04,		// black, king-side
	BQCA = 0x08			// black, queen-side
};

void dispose_board(struct board *brd);
struct board * init_board(char *fen);
uint8_t populate_pv_line(struct board *brd, uint8_t depth);
void set_piece_material(struct board *brd);
uint64_t overlay_white_pieces(const struct board * brd);
uint64_t overlay_black_pieces(const struct board * brd);
void overlay_boards(struct board *the_board);
uint8_t count_bits(uint64_t bb);
uint64_t square_to_bitboard(enum square sq);

#endif
