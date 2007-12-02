/*
 * filesystem.h
 *
 * Copyright (C) 2003, 2007 Berlin Brown
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _OCTANE_FILESYSTEM_H
#define _OCTANE_FILESYSTEM_H

#include <linux/sched.h>

#undef NR_OPEN
#define NR_OPEN           256

#define NR_INODE          2048	// this should be bigger than NR_FILE
#define NR_FILE           1024	// this can well be larger on a larger system
#define NR_SUPER          32
#define NR_IHASH          131
#define NR_FILE_LOCKS     64
#define BLOCK_SIZE        1024
#define BLOCK_SIZE_BITS   10

#define MAY_EXEC  1
#define MAY_WRITE 2
#define MAY_READ  4

#define READ      0
#define WRITE     1
#define READA     2		// read-ahead - don't pause
#define WRITEA    3	    // "write-ahead" - silly, but somewhat useful

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

extern struct sec_size * blk_sec[MAX_BLKDEV];
extern struct blk_dev_struct blk_dev[MAX_BLKDEV];
extern struct wait_queue * wait_for_request;

#define SECTOR_MASK (blksize_size[MAJOR_NR] &&                  \
	blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] ?               \
	((blksize_size[MAJOR_NR][MINOR(CURRENT->dev)] >> 9) - 1) :  \
	((BLOCK_SIZE >> 9)  -  1))

#define SUBSECTOR(block) (CURRENT->current_nr_sectors > 0)

extern int *blk_size[MAX_BLKDEV];
extern int *blksize_size[MAX_BLKDEV];

//************************************************
//
// Filesystem Datastructures
//
//************************************************
typedef char buffer_block[BLOCK_SIZE];

struct buffer_head {
	char            *b_data;			/* pointer to data block (1024 bytes) */
	unsigned long    b_size;		/* block size */
	unsigned long    b_blocknr;	/* block number */
	dev_t            b_dev;			/* device (0 = free) */
	unsigned short   b_count;		/* users using this block */
	unsigned char    b_uptodate;
	unsigned char    b_dirt;		/* 0-clean,1-dirty */
	unsigned char    b_lock;		/* 0 - ok, 1 -locked */
	unsigned char    b_req;		/* 0 if the buffer has been invalidated */
	unsigned char    b_list;		/* List that this buffer appears */
	unsigned char    b_retain;         /* Expected number of times this will
					   be used.  Put on freelist when 0 */
	unsigned long    b_flushtime;      /* Time when this (dirty) buffer should be written */
	unsigned long    b_lru_time;       /* Time when this buffer was last used. */
	struct wait_queue  *b_wait;
	struct buffer_head *b_prev;		/* doubly linked list of hash-queue */
	struct buffer_head *b_next;
	struct buffer_head *b_prev_free;	/* doubly linked list of buffers */
	struct buffer_head *b_next_free;
	struct buffer_head *b_this_page;	/* circular list of buffers in one page */
	struct buffer_head *b_reqnext;		/* request queue */
};


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
#endif // If major NR device check
//************************************************

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

// == ELSE ===================
#else

#define SET_INTR(x) (DEVICE_INTR = (x))

// == END IF ===============
#endif


static void (DEVICE_REQUEST)(void);

static void end_request(int uptodate)
{
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
		unlock_buffer(bh);
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

#endif // End of Filesystem.h
