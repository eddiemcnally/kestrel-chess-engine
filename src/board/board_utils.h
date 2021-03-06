/*
 * board_utils.h
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

void print_board(const struct position *pos);
bool ASSERT_BOARD_OK(const struct position *pos);
void assert_material_correct(const struct position *pos);
char *print_square(enum square sq);


void print_compressed_board(const struct position *pos);
mv_bitmap parse_move(char *ip_move, struct position *pos);
