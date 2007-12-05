/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: dma_functions.c
 *
 * Octane OS (Operating System)
 * Copyright (C) 2007 Berlin Brown
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See LICENSE.OCTANE for more details
 */
#define __KERNEL_CS     0x10
#define __KERNEL_DS     0x18

#define __USER_CS       0x23
#define __USER_DS       0x2B

#define PAGE_SHIFT                      12
#define PAGE_SIZE                       ((unsigned long)1<<PAGE_SHIFT)

long user_stack [ PAGE_SIZE>>2 ];

struct {
  long *a;
  short b;
} stack_start = { &user_stack[PAGE_SIZE>>2], __KERNEL_DS };

static void non_function(void) {
}
