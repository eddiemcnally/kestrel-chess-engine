/*
 * occupancy_mask.h
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

#include "kestrel.h"

uint64_t get_knight_occ_mask(enum square sq);
uint64_t get_bishop_occ_mask(enum square sq);
uint64_t get_king_occ_mask(enum square sq);
uint64_t get_queen_occ_mask(enum square sq);
uint64_t get_rook_occ_mask(enum square sq);



