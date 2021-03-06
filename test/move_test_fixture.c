/*
 * move_test_fixture.c
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "seatest.h"
#include "kestrel.h"
#include "attack.h"
#include "fen/fen.h"
#include "board.h"
#include "bitboard.h"
#include "pieces.h"
#include "utils.h"
#include "board_utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"


void test_generation_white_pawn_moves(void);
void test_generation_black_pawn_moves(void);
void test_generation_white_knight_pawn_moves(void);
void test_generation_black_knight_pawn_moves(void);
void test_generation_sliding_horizontal_and_vertical_moves(void);
void test_generation_sliding_diagonal_moves(void);
void test_generation_queen_moves(void);
void test_generation_king_moves(void);
void test_king_castling_moves(void);
void test_sample_board_position(void);
void test_clear_piece(void);
void test_add_piece(void);
void test_move_piece(void);
void test_capture_move_gen_1(void);
void test_capture_move_gen_2(void);
void test_capture_move_gen_3(void);
void test_zobrist_hashing_makemove_takemove(void);
void test_en_passant(void);
void test_en_passent_move_gen(void);
void move_test_fixture(void);
void test_make_move_take_move_1(void);
void test_generate_all_moves_level_1(void);



void test_generation_white_pawn_moves(void)
{

    /*Move:1 > a2a3 (score:0)
    Move:2 > a2a4 (score:0)
    Move:3 > c2c3 (score:0)
    Move:4 > c2c4 (score:0)
    Move:5 > e2e3 (score:0)
    Move:6 > e2e4 (score:0)
    Move:7 > h3h4 (score:0)
    Move:8 > b4b5 (score:0)
    Move:9 > b4c5 (score:0)
    Move:10 > d4d5 (score:0)
    Move:11 > d4c5 (score:0)
    Move:12 > d4e5 (score:0)
    Move:13 > f5f6 (score:0)
    Move:14 > f5e6 (score:0)
    Move:15 > g7g8q (score:0)
    Move:16 > g7g8r (score:0)
    Move:17 > g7g8b (score:0)
    Move:18 > g7g8n (score:0)
    Move:19 > g7f8q (score:0)
    Move:20 > g7f8r (score:0)
    Move:21 > g7f8b (score:0)
    Move:22 > g7f8n (score:0)
    Move:23 > g7h8q (score:0)
    Move:24 > g7h8r (score:0)
    Move:25 > g7h8b (score:0)
    Move:26 > g7h8n (score:0)
    MoveList Total 26 Moves:
     */

    // setup
    char *test_fen =
        "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_white_pawn_moves(pos, &mvl);

    assert_true(mvl.move_count == 26);

    mv_bitmap mv = MOVE_DEBUG(pos, a2, a3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, a2, a4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
    //print_move_details(mv);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, c2, c3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, e2, e3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, e2, e4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, h3, h4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b4, b5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b4, c5, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d4, d5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d4, c5, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d4, e5, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, f5, f6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, g8, NO_PIECE, W_QUEEN, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, g8, NO_PIECE, W_KNIGHT, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, g8, NO_PIECE, W_BISHOP, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, g8, NO_PIECE, W_ROOK, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, f8, B_BISHOP, W_ROOK, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, f8, B_BISHOP, W_QUEEN, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, f8, B_BISHOP, W_BISHOP, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, f8, B_BISHOP, W_KNIGHT, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    /*

    Move:23 > g7h8q (score:0)
    Move:24 > g7h8r (score:0)
    Move:25 > g7h8b (score:0)
    Move:26 > g7h8n (score:0)

     */

    mv = MOVE_DEBUG(pos, g7, h8, B_ROOK, W_KNIGHT, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, h8, B_ROOK, W_BISHOP, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, h8, B_ROOK, W_QUEEN, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g7, h8, B_ROOK, W_ROOK, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_generation_black_pawn_moves(void)
{
    // setup
    char *test_fen =
        "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1";
    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_black_pawn_moves(pos, &mvl);

    // test
    assert_true(mvl.move_count == 26);

    mv_bitmap mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_ROOK, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_QUEEN, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_BISHOP, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_KNIGHT, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, h1, W_ROOK, B_KNIGHT, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, h1, W_ROOK, B_BISHOP, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, h1, W_ROOK, B_ROOK, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, h1, W_ROOK, B_QUEEN, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_KNIGHT, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_BISHOP, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_ROOK, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, B_QUEEN, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, f4, f3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b5, b4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d5, d4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    free_board(pos);

}

void test_generation_white_knight_pawn_moves(void)
{
    // setup
    char *test_fen = "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_knight_piece_moves(pos, &mvl, WHITE);

    //printf("move count %d\n", mvl.move_count);
    //print_move_list_details(mvl);

    // test
    assert_true(mvl.move_count == 14);

    // check moves from d3
    mv_bitmap mv = MOVE_DEBUG(pos, d3, f2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, e1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, c1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, b2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, b4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, c5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, e5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, f4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    // check moves from g5
    mv = MOVE_DEBUG(pos, g5, h3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g5, f3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g5, e4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g5, f7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g5, h7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g5, e6, B_KNIGHT, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_generation_black_knight_pawn_moves(void)
{
    // setup
    char *test_fen =
        "r1bqkb1r/pppppppp/2N5/3n1N2/8/1n6/PPPPPPPP/R1BQKB1R b KQkq - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_knight_piece_moves(pos, &mvl, BLACK);

    //printf("move count %d\n", mvl.move_count);
    //print_move_list_details(mvl);

    // test
    assert_true(mvl.move_count == 12);

    // start on b3
    mv_bitmap mv = MOVE_DEBUG(pos, b3, d2, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, c1, W_BISHOP, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, a1, W_ROOK, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b3, d2, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, c1, W_BISHOP, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, a1, W_ROOK, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    // start on d5

    mv = MOVE_DEBUG(pos, d5, f4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d5, e3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d5, c3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d5, b4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d5, b6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d5, f6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_generation_king_moves(void)
{
    // setup
    char *test_fen =
        "8/p1P4p/R3p2P/P1pP1q2/PRP1p1P1/3Qprr1/kp1p3P/7K w - - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_king_moves(pos, &mvl, BLACK);

    assert_true(mvl.move_count == 4);

    // black king on a2
    mv_bitmap mv = MOVE_DEBUG(pos, a2, a3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, a2, b3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, a2, b1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, a2, a1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

	// reset things and check for white king
    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    memset(&mvl, 0, sizeof(struct move_list));
    TEST_generate_king_moves(pos, &mvl, WHITE);

    assert_true(mvl.move_count == 2);

    // black king on h1
    mv = MOVE_DEBUG(pos, h1, g2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, h1, g1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

    //=======================
    // reset things and do another test
    test_fen = "r5r1/2KP2R1/R1PP2p1/1pPp2P1/pp1Pp3/3kp1P1/3P2pQ/6q1 w - - 0 1";

	pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    memset(&mvl, 0, sizeof(struct move_list));
    TEST_generate_king_moves(pos, &mvl, BLACK);

    assert_true(mvl.move_count == 6);

    // black king on d3
    mv = MOVE_DEBUG(pos, d3, c4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, c3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, c2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, d2, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, d4, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d3, e2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

    // reset things and check for white king
	pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    memset(&mvl, 0, sizeof(struct move_list));
    TEST_generate_king_moves(pos, &mvl, WHITE);

    assert_true(mvl.move_count == 5);

    // black king on c7
    mv = MOVE_DEBUG(pos, c7, b6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, b7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, b8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, c8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, d8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_generation_sliding_diagonal_moves(void)
{

    char *sliding_test =
        "8/2B1p2P/4PppK/p2pP1P1/1P4pp/Pb2p3/3P1Pk1/2bB4 w - - 0 1";
    struct position *pos = allocate_board();
    consume_fen_notation(sliding_test, pos);


    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    //print_board(pos);

    // black bishops
    TEST_generate_sliding_diagonal_moves(pos, &mvl, BLACK);

    //print_move_list_details(mvl);

    assert_true(mvl.move_count == 8);

    mv_bitmap mv = MOVE_DEBUG(pos, b3, a4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, c2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, d1, W_BISHOP, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, a2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b3, c4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, c1, b2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c1, a3, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c1, d2, W_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    assert_false(is_sq_attacked(pos, h6, BLACK));

	free_board(pos);
///////////////////////////////////////////////////
    // now look at white bishops
    pos = allocate_board();
    consume_fen_notation(sliding_test, pos);

    memset(&mvl, 0, sizeof(struct move_list));

    //print_board(pos);

    TEST_generate_sliding_diagonal_moves(pos, &mvl, WHITE);
    assert_true(mvl.move_count == 10);

    //print_move_list_details(mvl);

    mv = MOVE_DEBUG(pos, d1, c2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d1, b3, B_BISHOP, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d1, e2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d1, f3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, d1, g4, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, c7, b6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, a5, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, d6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, b8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, c7, d8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_generation_sliding_horizontal_and_vertical_moves(void)
{

    char *sliding_test = "K7/1rp5/5R1P/6p1/7P/1k3p1P/1P1p2r1/4R3 w - - 0 1";
    struct position *pos = allocate_board();
    consume_fen_notation(sliding_test, pos);


    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    memset(&mvl, 0, sizeof(struct move_list));

    TEST_generate_sliding_horizontal_vertical_moves(pos, &mvl, BLACK);

    assert_true(mvl.move_count == 11);

    mv_bitmap mv = MOVE_DEBUG(pos, g2, e2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g2, f2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g2, h2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g2, g1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g2, g3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, g2, g4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b7, b8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b7, a7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b7, b6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b7, b5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, b7, b4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

    // check the white rooks
    pos = allocate_board();
    consume_fen_notation(sliding_test, pos);

    memset(&mvl, 0, sizeof(struct move_list));

    TEST_generate_sliding_horizontal_vertical_moves(pos, &mvl, WHITE);

    assert_true(mvl.move_count == 25);

    mv = MOVE_DEBUG(pos, e1, d1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, c1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, b1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, a1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, f1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, g1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, h1, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e2, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e3, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, e8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, f6, f7, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, f8, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, g6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, a6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, b6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, c6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, d6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, e6, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, f5, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, f4, NO_PIECE, NO_PIECE, 0);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, f6, f3, B_PAWN, NO_PIECE, MFLAG_CAPTURE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);
}

void test_king_castling_moves(void)
{
    char *sliding_test = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1\n";
    struct position *pos = allocate_board();
    consume_fen_notation(sliding_test, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    TEST_generate_castle_moves(pos, &mvl, WHITE);

    assert_true(mvl.move_count == 2);
    mv_bitmap mv = MOVE_DEBUG(pos, e1, g1, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e1, c1, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

	pos = allocate_board();
    consume_fen_notation(sliding_test, pos);

    memset(&mvl, 0, sizeof(struct move_list));

    TEST_generate_castle_moves(pos, &mvl, BLACK);

    assert_true(mvl.move_count == 2);
    mv = MOVE_DEBUG(pos, e8, g8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    assert_true(TEST_is_move_in_list(&mvl, mv));
    mv = MOVE_DEBUG(pos, e8, c8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

    sliding_test = "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1";
    pos = allocate_board();
    consume_fen_notation(sliding_test, pos);

    memset(&mvl, 0, sizeof(struct move_list));

    TEST_generate_castle_moves(pos, &mvl, WHITE);

    assert_true(mvl.move_count == 0);
	free_board(pos);


	///////////////////////////////////////////////
    pos = allocate_board();
    consume_fen_notation(sliding_test, pos);
    memset(&mvl, 0, sizeof(struct move_list));

    TEST_generate_castle_moves(pos, &mvl, BLACK);
    assert_true(mvl.move_count == 1);

    mv = MOVE_DEBUG(pos, e8, g8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    assert_true(TEST_is_move_in_list(&mvl, mv));

	free_board(pos);

}

void test_sample_board_position()
{

    char *sample_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";
    struct position *pos = allocate_board();
    consume_fen_notation(sample_position, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_moves(pos, &mvl);

    assert_true(mvl.move_count == 48);
    free_board(pos);
}

void test_generate_all_moves_level_1(){
char *sample_position =
        "r2q1rk1/2p1pppp/p1n2n2/1p1p4/1PbP2b1/B1N1PN2/P1P1BPPP/R2QK2R w KQ - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(sample_position, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_moves(pos, &mvl);

    assert_true(mvl.move_count == 48);

	free_board(pos);
}




void test_clear_piece()
{

    //
    // remove the knight from c3
    //
    char *sample_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";
    struct position *pos = allocate_board();
    consume_fen_notation(sample_position, pos);

   	const struct bitboards *bb_str = get_bitboard_struct(pos);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), c3) == true);
    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), c3) == true);

    // save some info before the move for comparison
    uint64_t old_hash = get_board_hash(pos);
    int32_t old_material = get_material_value(pos, WHITE);
    enum piece old_pce = get_piece_on_square(pos, c3);

    // remove the knight from c3
    remove_piece_from_board(pos, W_KNIGHT, c3);

    assert_true(old_hash != get_board_hash(pos));

    int32_t new_material = get_material_value(pos, WHITE) + (int32_t)GET_PIECE_VALUE(W_KNIGHT);
    assert_true(new_material == old_material);

    assert_true(old_pce == W_KNIGHT);
    assert_true(get_piece_on_square(pos, c3) == NO_PIECE);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), c3) == false);
    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), c3) == false);

	free_board(pos);
}

void test_add_piece()
{

    //
    // add a White Knight to c4
    //
    char *sample_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(sample_position, pos);

   	const struct bitboards *bb_str = get_bitboard_struct(pos);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), c4) == false);
    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), c4) == false);
    assert_true(count_bits(get_bitboard_for_piece(bb_str, W_KNIGHT)) == 2);

    // save some info before the move for comparison
    uint64_t old_hash = get_board_hash(pos);
    int32_t old_material = get_material_value(pos, WHITE);

    // add a white knight to c4
    add_piece_to_board(pos, W_KNIGHT, c4);

    assert_true(old_hash != get_board_hash(pos));
    int32_t new_material = get_material_value(pos, WHITE);
    assert_true(new_material == (old_material + (int32_t)GET_PIECE_VALUE(W_KNIGHT)));

    assert_true(get_piece_on_square(pos, c4) == W_KNIGHT);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), c4) == true);
    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), c4) == true);

	free_board(pos);
}

/**
 * Sets up board and verifies that the board is as expected after an en passant move
 *
 8   .  .  .  .  k  .  .  .
 7   .  .  p  .  .  .  .  .
 6   .  .  .  .  .  .  .  .
 5   .  .  .  P  .  .  .  .
 4   .  .  .  .  .  .  .  .
 3   .  .  .  .  .  .  .  .
 2   .  .  .  .  .  .  .  .
 1   .  .  .  .  K  .  .  .

     A  B  C  D  E  F  G  H

 */
void test_en_passant(void)
{
    char *test_fen = "4k3/2p5/8/3P4/8/8/8/4K3 b - - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

   	const struct bitboards *bb_str = get_bitboard_struct(pos);

    mv_bitmap mv = MOVE_DEBUG(pos, c7, c5, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
    make_move(pos, mv);

    // make sure all other pieces are as expected
    assert_true(get_piece_on_square(pos, e8) == B_KING);
    assert_true(get_piece_on_square(pos, c5) == B_PAWN);
    assert_true(get_piece_on_square(pos, d5) == W_PAWN);
    assert_true(get_piece_on_square(pos, e1) == W_KING);
    // 4 pieces on the board
    assert_true(count_bits(get_bitboard_all_pieces(bb_str)) == 4);
    assert_true(get_en_passant_sq(pos) == c6);

    // now, make the en passant move
    mv = MOVE_DEBUG(pos, d5, c6, NO_PIECE, NO_PIECE, MFLAG_EN_PASSANT);
    make_move(pos, mv);

    // make sure all other pieces are as expected
    assert_true(get_piece_on_square(pos, e8) == B_KING);
    assert_true(get_piece_on_square(pos, c6) == W_PAWN);
    assert_true(get_piece_on_square(pos, e1) == W_KING);
    // 4 pieces on the board
    assert_true(count_bits(get_bitboard_all_pieces(bb_str)) == 3);

	free_board(pos);
}

void test_en_passent_move_gen(){
    char *test_fen = "rnbqkbnr/pp2pppp/8/2ppP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3\n";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };
    TEST_generate_white_pawn_moves(pos, &mvl);

    assert_true(mvl.move_count == 16);

    //print_move_list_details(&mvl);

	// x2 pawn moves
    mv_bitmap mv = MOVE_DEBUG(pos, a2, a4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b2, b4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, c2, c4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d2, d4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, f2, f4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, h2, h4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(is_move_in_list(&mvl, mv));

	// one square pawn moves
    mv = MOVE_DEBUG(pos, a2, a3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, b2, b3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, c2, c3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, d2, d3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, f2, f3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, g2, g3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, h2, h3, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));


    mv = MOVE_DEBUG(pos, e5, e6, NO_PIECE, NO_PIECE, MFLAG_NONE);
	assert_true(is_move_in_list(&mvl, mv));

    mv = MOVE_DEBUG(pos, e5, d6, B_PAWN, NO_PIECE, MFLAG_EN_PASSANT);
	assert_true(is_move_in_list(&mvl, mv));

	free_board(pos);

}


void test_move_piece()
{

    //
    // move White Knight from e5 to d3
    //
    char *sample_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(sample_position, pos);

	const struct bitboards *bb_str = get_bitboard_struct(pos);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), e5) == true);
    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), e5) == true);
    assert_true(count_bits(get_bitboard_for_piece(bb_str, W_KNIGHT)) == 2);

    // save some info before the move for comparison
    uint64_t old_hash = get_board_hash(pos);
    int32_t old_material = get_material_value(pos, WHITE);

    // add a white knight from e4 to d3
    move_piece(pos, e5, d3);

    assert_true(old_hash != get_board_hash(pos));

    int32_t new_material = get_material_value(pos, WHITE);
    assert_true(new_material == old_material);

    assert_true(get_piece_on_square(pos, d3) == W_KNIGHT);
    assert_true(get_piece_on_square(pos, e5) == NO_PIECE);

    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), d3) == true);
    assert_true(is_square_occupied(get_bitboard_all_pieces(bb_str), e5) == false);

    assert_true(is_square_occupied(get_bitboard_for_piece(bb_str, W_KNIGHT), d3) == true);

	free_board(pos);
}

void test_make_move_take_move_1(void)
{
    const int NUM_POSITIONS = 21;

    char *positions[NUM_POSITIONS];
    positions[0] = "4q3/p4p2/n2ppp2/kP1P1b1P/2p1PbrQ/1P3RNp/2P2K1R/8 w - - 0 1\n";
    positions[1] = "2k4r/2pqpp2/2pppn2/P1R1P1bQ/1PP3PP/8/p6N/4KR1b w - - 0 1\n";
    positions[2] = "B2qQ3/1bpp1K2/p3PP1p/p1R5/rP4N1/1P4R1/3pPP1p/5kb1 w - - 0 1\n";
    positions[3] = "3R2q1/1b3Rpp/3pq1p1/2br1p1N/1P2P1Qp/1B1P4/PPK2k1p/8 w - - 0 1\n";
    positions[4] = "4k3/1PPb2q1/Pp3bP1/2R2p1K/q1n1p2R/Q1p1rp2/1p2PP2/7n w - - 0 1\n";
    positions[5] = "K7/1PP1pP2/BPRPqp2/1P1rp1P1/Q2pR3/2Pp4/1pb1p1kb/2N1q3 w - - 0 1\n";
    positions[6] = "1R6/p3p1Q1/NPPP1PPP/1Rp2kp1/1p1p4/1K1P1B2/2P2qp1/B3r2q w - - 0 1\n";
    positions[7] = "8/pp2k3/B4q2/6RP/2P2BK1/7R/Q3Pr2/1q3N2 w - - 0 1\n";
    positions[8] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";
    positions[9] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n";
    positions[10] = "8/1pQ2B2/p3p2p/2Pqr1p1/2bP1bPq/1p3kPP/PB6/N5K1 w - - 0 1\n";
    positions[11] = "2qq3Q/3b1Kp1/P2b1P1p/r3pp2/1B4BP/4P1P1/2pp1P2/1N3k2 w - - 0 1\n";
    positions[12] = "4B1q1/1Q6/1rp3qk/3P3p/1bp3PP/3PPp1P/ppN4K/4Bb2 w - - 0 1\n";
    positions[13] = "7q/P3p1q1/1k1p1P2/3pNbb1/B1p3P1/1Ppp1KPr/5P1B/Q7 w - - 0 1\n";
    positions[14] = "k2B1N2/p4B1p/2KP4/pp1P4/2pPPbqP/4rp1Q/5P2/3b2q1 w - - 0 1\n";
    positions[15] = "4Bb1q/qb2p1P1/p2Qr1P1/Kpp1B3/2p1P2P/6P1/P1k1p3/3N4 w - - 0 1\n";
    positions[16] = "6B1/3P1p2/1b2rp2/3ppk2/PPPP4/NqP1B2p/bpqQK3/8 w - - 0 1\n";
    positions[17] = "2N5/2r1b3/4p2p/PPPb4/B1n1np2/1ppp2P1/1P1Pk3/4B1K1 w - - 0 1\n";
    positions[18] = "4B3/1Pk1p1P1/P3P2r/N1pbpKp1/2PP2nB/4p2n/6p1/2b5 w - - 0 1\n";
    positions[19] = "3n4/2pp2P1/p1b4r/2p1nPP1/8/P1pP1kBB/1p1b3P/1N3K2 w - - 0 1\n";
    positions[20] = "7B/p2p4/B1b1r3/P1PP2b1/k2N1p1n/3pp1Pp/1K2PP2/5n2 w - - 0 1\n";

    for (int pos = 0; pos < NUM_POSITIONS; pos++) {
        char *sample_position = positions[pos];

        // printf("processing FEN %s\n", sample_position);

        struct position *pos = allocate_board();
        consume_fen_notation(sample_position, pos);

        struct move_list list = {
            .moves = {0},
            .move_count = 0
        };


        generate_all_moves(pos, &list);

        struct position *starting_pos = allocate_board();

        clone_board(pos, starting_pos);

        for (int i = 0; i < list.move_count; i++) {
            // make a move, take it back, and compare board before and after
            mv_bitmap mv = list.moves[i];

            struct position *before_move = allocate_board();
            clone_board(pos, before_move);

            bool valid_move = make_move(pos, mv);
            if (!valid_move) {
                continue;
            }

            take_move(pos);

            assert_boards_are_equal(pos, before_move);
			free_board(before_move);

        }

        assert_boards_are_equal(pos, starting_pos);

        free_board(starting_pos);
        free_board(pos);
    }
}


void test_zobrist_hashing_makemove_takemove(void)
{


    // check enpassant hashing
    // =======================
    char *enpass_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(enpass_pos, pos);

    uint64_t pre_ep_hash = get_board_hash(pos);

    // make move to cause enpassant on e3
    mv_bitmap mv = MOVE_DEBUG(pos, e2, e4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
    make_move(pos, mv);
    // check hash has changed
    assert_true(pre_ep_hash != get_board_hash(pos));
    take_move(pos);
    // check hash is back to original
    assert_true(get_board_hash(pos) == pre_ep_hash);

	free_board(pos);

    // check castle hashing
    // ====================
    char *castle_pos = "r1bqkbnr/pppp2pp/2n1pp2/8/8/3BPN2/PPPP1PPP/RNBQK2R w KQkq - 0 1\n";

    pos = allocate_board();
    consume_fen_notation(castle_pos, pos);


    uint64_t pre_castle_hash = get_board_hash(pos);

    // make castle move
    mv = MOVE_DEBUG(pos, e1, g1, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
    make_move(pos, mv);
    // check hash has changed
    assert_true(pre_castle_hash != get_board_hash(pos));
    take_move(pos);
    // check hash is back to original
    assert_true(get_board_hash(pos) == pre_castle_hash);

	free_board(pos);

    // check side-to-move hashing
    // ==========================
    char *side_to_move_pos = "r1bqkbnr/pppp2pp/2n1pp2/8/8/3BPN2/PPPP1PPP/RNBQK2R w KQkq - 0 1\n";

    pos = allocate_board();
    consume_fen_notation(side_to_move_pos, pos);

    uint64_t pre_swap_hash = get_board_hash(pos);

    // flip sides
    flip_sides(pos);

    // check hash has changed
    assert_true(pre_swap_hash != get_board_hash(pos));

    // flip back and check hash is back to original
    flip_sides(pos);
    assert_true(get_board_hash(pos) == pre_swap_hash);

	free_board(pos);
}



void test_capture_move_gen_1(void)
{
    struct position *pos = allocate_board();
    consume_fen_notation(STARTING_FEN, pos);

    struct move_list list = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_capture_moves(pos, &list);

    assert_true(list.move_count == 0);

	free_board(pos);
}

void test_capture_move_gen_2(void)
{
    char *fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(fen, pos);

    struct move_list list = {
        .moves = {0},
        .move_count = 0
    };
    generate_all_capture_moves(pos, &list);

    assert_true(list.move_count == 8);

    for(int i = 0; i < list.move_count; i++) {
        mv_bitmap mv = list.moves[i];
        enum piece pce = CAPTURED_PCE(mv);

        assert_true(pce != NO_PIECE);
        assert_true(IS_CAPTURE_MOVE(mv) == true);
    }
    free_board(pos);
}

void test_capture_move_gen_3(void)
{
    char *fen = "6r1/1b2npb1/1p2P3/1PPBpnR1/Pk1PpPpQ/N1qP1rp1/3P1Npp/BK1R4 w - - 0 1\n";

    struct position *pos = allocate_board();
    consume_fen_notation(fen, pos);


    struct move_list list = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_capture_moves(pos, &list);

    assert_true(list.move_count == 17);

    for(int i = 0; i < list.move_count; i++) {
        mv_bitmap mv = list.moves[i];
        enum piece pce = CAPTURED_PCE(mv);

        assert_true(pce != NO_PIECE);
        assert_true(IS_CAPTURE_MOVE(mv) == true);
    }
    free_board(pos);
}



void move_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_generation_white_pawn_moves);
    run_test(test_generation_black_pawn_moves);
    run_test(test_generation_white_knight_pawn_moves);
    run_test(test_generation_black_knight_pawn_moves);
    run_test(test_generation_king_moves);
    run_test(test_king_castling_moves);
    run_test(test_en_passant);
    run_test(test_en_passent_move_gen);
    run_test(test_generation_sliding_horizontal_and_vertical_moves);
    run_test(test_generation_sliding_diagonal_moves);
    run_test(test_sample_board_position);
    run_test(test_clear_piece);
    run_test(test_add_piece);
    run_test(test_move_piece);
    run_test(test_make_move_take_move_1);

    run_test(test_capture_move_gen_1);
    run_test(test_capture_move_gen_2);
    run_test(test_capture_move_gen_3);

    run_test(test_zobrist_hashing_makemove_takemove);

    test_fixture_end();	// ends a fixture
}
