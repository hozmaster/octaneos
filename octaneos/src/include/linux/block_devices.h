/*
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
 */

#ifndef __BLOCK_DEVICES_H_
#define __BLOCK_DEVICES_H_

#include <system/major_devices.h>
#include <system/filesystem.h>

#include <linux/block_devices_structs.h>

extern struct sec_size       *blk_sec[MAX_BLKDEV];
extern struct blk_dev_struct  blk_dev[MAX_BLKDEV];
extern struct wait_queue     *wait_for_request;

extern int *blk_size[MAX_BLKDEV];
extern int *blksize_size[MAX_BLKDEV];

//------------------------------------------------
// Define Major device interrupts
//
// (IFDEF MAJOR_NR)
//------------------------------------------------
#ifdef MAJOR_NR

//------------------------------------------------
// Check MAJOR_NR against MAJOR devices
//------------------------------------------------
#if (MAJOR_NR == MEM_MAJOR)

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
#endif //// If major NR == some MAJOR device

// If MAJOR_NR defined (e.g floppy or hard drive device)

#define SECTOR_MASK (blksize_size[MAJOR_NR] &&                  \
	blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] ?               \
	((blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] >> 9) - 1) :  \
	((BLOCK_SIZE >> 9)  -  1))

#define SUBSECTOR(block) (CURRENT->current_nr_sectors > 0)

#ifndef DEVICE_REQUEST
#error "Device Request not set in block devices"
#endif

//************************************************
// Define Current Block Device Timer Interrupts
//************************************************

#ifndef CURRENT
#define CURRENT (blk_dev[MAJOR_NR].current_request)
#endif

#define CURRENT_DEV DEVICE_NR(CURRENT->dev)

#ifdef DEVICE_INTR
void (*DEVICE_INTR)(void) = NULL;
#endif


#ifdef DEVICE_TIMEOUT
// == BEGIN IF ===============

#define SET_TIMER                 \
((timer_table[DEVICE_TIMEOUT].expires = jiffies + TIMEOUT_VALUE), \
(timer_active |= 1<<DEVICE_TIMEOUT))

#define CLEAR_TIMER               \
timer_active &= ~(1<<DEVICE_TIMEOUT)

#define SET_INTR(x)               \
if ((DEVICE_INTR = (x)) != NULL)  \
	SET_TIMER;                    \
else                              \
	CLEAR_TIMER;

// == ELSE =========
#else

#define SET_INTR(x) (DEVICE_INTR = (x))

// == END IF =======
#endif

static void (DEVICE_REQUEST)(void);

/*
 * Define end_request statically for each major device source,
 * for example the floppy.c major device.
 */
static void end_request(int uptodate) {

	struct request       *req;
	struct buffer_head   *bh;
	struct task_struct   *p;

	req = CURRENT;
	req->errors = 0;
	if (!uptodate) {
		printk(DEVICE_NAME " I/O error\n");
		printk("dev %04lX, sector %lu\n",
		       (unsigned long)req->dev, req->sector);
		req->nr_sectors--;
		req->nr_sectors &= ~SECTOR_MASK;
		req->sector += (BLOCK_SIZE / 512);
		req->sector &= ~SECTOR_MASK;		
	}

	if ((bh = req->bh) != NULL) {
		req->bh = bh->b_reqnext;
		bh->b_reqnext = NULL;
		bh->b_uptodate = uptodate;
		//unlock_buffer(bh);
		if ((bh = req->bh) != NULL) {
			req->current_nr_sectors = bh->b_size >> 9;
			if (req->nr_sectors < req->current_nr_sectors) {
				req->nr_sectors = req->current_nr_sectors;
				printk("end_request: buffer-list destroyed\n");
			}
			req->buffer = bh->b_data;
			return;
		}
	}
	DEVICE_OFF(req->dev);
	CURRENT = req->next;
	if ((p = req->waiting) != NULL) {
		req->waiting = NULL;
		p->swapping = 0;
		p->state = TASK_RUNNING;
		if (p->counter > current->counter)
			need_resched = 1;
	}
	req->dev = -1;
	wake_up(&wait_for_request);
}


#ifdef DEVICE_INTR
#define CLEAR_INTR SET_INTR(NULL)
#else
#define CLEAR_INTR
#endif

#define INIT_REQUEST                                   \
	if (!CURRENT) {                                    \
		CLEAR_INTR;                                    \
		return;                                        \
	}                                                  \
	if (MAJOR(CURRENT->dev) != MAJOR_NR)               \
		panic(DEVICE_NAME ": request list destroyed"); \
	if (CURRENT->bh) {                                 \
		if (!CURRENT->bh->b_lock)                      \
			panic(DEVICE_NAME ": block not locked");   \
	}

#endif
//------------------------------------------------
// END: If major NR device check
//------------------------------------------------

#endif
