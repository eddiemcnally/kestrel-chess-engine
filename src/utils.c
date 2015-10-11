/*
 * utils.c
 * 
 * ---------------------------------------------------------------------
 * DESCRIPTION: misc utilities for the application
 * ---------------------------------------------------------------------
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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sched.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "types.h"
#include "utils.h"

U64 generate_rand64(void)
{
	U64 retval = 0;

	retval = (U64) rand();
	retval |= (U64) rand() << 15;
	retval |= (U64) rand() << 30;
	retval |= (U64) rand() << 45;
	retval |= ((U64) rand() & 0xf) << 60;

	return retval;
}

void set_priority_and_affinity(void)
{
	// set up CPU affinity
	cpu_set_t my_set;
	CPU_ZERO(&my_set);
	CPU_SET(1, &my_set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &my_set) > 0) {
		printf("affinity error");
		exit(-1);
	}
	// set process priority to max
	if (setpriority(PRIO_PROCESS, 0, PRIO_MAX) != 0) {
		printf("process priority error");
		exit(-1);
	}

}


U64 get_time_in_millis(void)
{
	struct timeval tp;

	int errno = gettimeofday(&tp, NULL);

	if (errno == 0) {
		return (U64) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
	}

	return 0;
}

U64 get_elapsed_time_in_millis(U64 start_time)
{
	U64 now_in_millis = get_time_in_millis();
	return (now_in_millis - start_time);
}
