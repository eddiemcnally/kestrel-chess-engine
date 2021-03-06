/*
 * uci_protocol.h
 *
 * ---------------------------------------------------------------------
 * Description : code associated with handling the UCI protocol
 * ---------------------------------------------------------------------
 *
 *
 *
 * Copyright (C) 2016 Eddie McNally <emcn@gmx.com>
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
 *
 */
#include <stdio.h>
#include "kestrel.h"
#include "search.h"

void uci_print_hello(void);
void read_input(struct search_info *si);
void uci_print_ready(void);
int uci_check_input_buffer(void);
void uci_parse_position(char *line, struct position *pos);
void uci_print_bestmove(mv_bitmap mv);
void uci_parse_go(char *line, struct search_info *si, struct position *pos);
void uci_print_info_score(int32_t best_score, uint8_t depth, uint32_t nodes,
                          uint64_t time_in_ms, uint8_t num_pv_moves, mv_bitmap *pv_line);

