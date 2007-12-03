/*
 Octane OS (Operating System)
 Copyright (C) 2007 Berlin Brown

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __BLOCK_DEVICES_H_
#define __BLOCK_DEVICES_H_

#define MAX_CHRDEV 32
#define MAX_BLKDEV 32

/*
 * NR_REQUEST is the number of entries in the request-queue.
 * NOTE that writes may use only the low 2/3 of these: reads
 * take precedence.
 */
#define NR_REQUEST	64

/*
 * Ok, this is an expanded form so that we can use the same
 * request for paging requests when that is implemented. In
 * paging, 'bh' is NULL, and 'waiting' is used to wait for
 * read/write completion.
 */
struct request {
	int dev;		/* -1 if no request */
	int cmd;		/* READ or WRITE */
	int errors;
	unsigned long sector;
	unsigned long nr_sectors;
	unsigned long current_nr_sectors;
	char                *buffer;
	struct task_struct  *waiting;
	struct buffer_head  *bh;
	struct buffer_head  *bhtail;
	struct request      *next;
};

struct blk_dev_struct {
	void (*request_fn)(void);
	struct request * current_request;
};

struct sec_size {
	unsigned block_size;
	unsigned block_size_bits;
};

extern struct sec_size       *blk_sec[MAX_BLKDEV];
extern struct blk_dev_struct  blk_dev[MAX_BLKDEV];
extern struct wait_queue     *wait_for_request;

extern int *blk_size[MAX_BLKDEV];
extern int *blksize_size[MAX_BLKDEV];

//************************************************
// Define Major device interrupts
//************************************************
#ifdef MAJOR_NR

#if (MAJOR_NR == MEM_MAJOR)

/* ram disk */
#define DEVICE_NAME "ramdisk"
#define DEVICE_REQUEST do_rd_request
#define DEVICE_NR(device) ((device) & 7)
#define DEVICE_ON(device) 
#define DEVICE_OFF(device)

#elif (MAJOR_NR == FLOPPY_MAJOR)

static void floppy_on(unsigned int nr);
static void floppy_off(unsigned int nr);

#define DEVICE_NAME "floppy"
#define DEVICE_INTR do_floppy
#define DEVICE_REQUEST do_fd_request
#define DEVICE_REQUEST NULL
#define DEVICE_NR(device) ((device) & 3)
#define DEVICE_ON(device) floppy_on(DEVICE_NR(device))
#define DEVICE_OFF(device) floppy_off(DEVICE_NR(device))

#elif (MAJOR_NR == HD_MAJOR)

/* harddisk: timeout is 6 seconds.. */
#define DEVICE_NAME "harddisk"
#define DEVICE_INTR do_hd
#define DEVICE_TIMEOUT HD_TIMER
#define TIMEOUT_VALUE 600
#define DEVICE_REQUEST do_hd_request
#define DEVICE_NR(device) (MINOR(device)>>6)
#define DEVICE_ON(device)
#define DEVICE_OFF(device)

#else
#error "Unknown Block Device"
#endif // If major NR

// If MAJOR_NR defined (e.g floppy or hard drive device)

#define SECTOR_MASK (blksize_size[MAJOR_NR] &&                  \
	blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] ?               \
	((blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] >> 9) - 1) :  \
	((BLOCK_SIZE >> 9)  -  1))

#define SUBSECTOR(block) (CURRENT->current_nr_sectors > 0)

#endif // If major NR device check
//************************************************

#endif
