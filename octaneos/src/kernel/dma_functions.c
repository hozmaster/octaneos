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

#include <system/alpha.h>

static volatile unsigned int dma_chan_busy[MAX_DMA_CHANNELS] = {
  1, 0, 0, 0, 1, 0, 0, 0
};

static __inline__ unsigned int mutex_atomic_swap(volatile unsigned int *p,
						 unsigned int newval)
{
  unsigned int semval = newval;
  asm __volatile__ (
		    "xchgl %2, %0\n"         \
		    : "=r" (semval)          \
		    : "0" (semval), "m" (*p) \
		    );

  return semval;
}

int request_dma(unsigned int dmanr)
{
  if (dmanr >= MAX_DMA_CHANNELS)
    return -500;

  if (mutex_atomic_swap(&dma_chan_busy[dmanr], 1) != 0)
    return -600;
  else
    return 0;

} // end of the functino +++

void free_dma(unsigned int dmanr) {

  char buf[80];
  if (dmanr >= MAX_DMA_CHANNELS) {
    __sprintf(buf, "Trying to free DMA%d\n", dmanr);
    __puts(buf);
    return;
  }

  if (mutex_atomic_swap(&dma_chan_busy[dmanr], 0) == 0) {
    __sprintf(buf, "Trying to free free DMA%d\n", dmanr);
    __puts(buf);
  }

}
