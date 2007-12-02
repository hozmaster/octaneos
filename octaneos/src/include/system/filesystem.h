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
#include <linux/block_devices.h>

//************************************************
// Major Devices
//************************************************

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
 * assignments
 *
 * devices are as follows (same as linux)
 *
 *      character              block                  comments
 *      --------------------   --------------------   --------------------
 *  0 - unnamed                unnamed                minor 0 = true nodev
 *  1 - /dev/mem               ramdisk
 *  2 -                        floppy
 *  3 -                        hd
 *  4 - /dev/tty*
 *  5 - /dev/tty; /dev/cua*
 *  6 - lp
 *  7 -                                               UNUSED
 *  8 -                        scsi disk
 *  9 - scsi tape
 * 10 - mice
 * 11 -                        scsi cdrom
 * 12 - qic02 tape
 * 13 -                        xt disk
 * 14 - sound card
 * 15 -                        cdu31a cdrom
 * 16 - sockets
 * 17 - af_unix
 * 18 - af_inet
 * 19 -                                               UNUSED
 * 20 -                                               UNUSED
 * 21 - scsi generic
 * 22 -                        (at2disk)
 * 23 -                        mitsumi cdrom
 * 24 -	                       sony535 cdrom
 * 25 -                        matsushita cdrom       minors 0..3
 * 26 -                        matsushita cdrom 2     minors 0..3
 * 27 - qic117 tape            matsushita cdrom 3     minors 0..3
 * 28 -                        matsushita cdrom 4     minors 0..3
 */
#define UNNAMED_MAJOR 	            0
#define MEM_MAJOR	                1
#define FLOPPY_MAJOR	            2
#define HD_MAJOR	                3
#define TTY_MAJOR	                4
#define TTYAUX_MAJOR	            5
#define LP_MAJOR	                6
/* unused: 7 */
#define SCSI_DISK_MAJOR	            8
#define SCSI_TAPE_MAJOR	            9
#define MOUSE_MAJOR	                10
#define SCSI_CDROM_MAJOR            11
#define QIC02_TAPE_MAJOR            12
#define XT_DISK_MAJOR	            13
#define SOUND_MAJOR                 14
#define CDU31A_CDROM_MAJOR          15
#define SOCKET_MAJOR 	            16
#define AF_UNIX_MAJOR	            17
#define AF_INET_MAJOR	            18
/* unused: 19, 20 */
#define SCSI_GENERIC_MAJOR          21
/* unused: 22 */
#define MITSUMI_CDROM_MAJOR         23
#define CDU535_CDROM_MAJOR          24
#define MATSUSHITA_CDROM_MAJOR      25
#define MATSUSHITA_CDROM2_MAJOR     26
#define MATSUSHITA_CDROM3_MAJOR     27
#define MATSUSHITA_CDROM4_MAJOR     28
#define QIC117_TAPE_MAJOR           27

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


struct inode {
	dev_t		     i_dev;
	unsigned long	 i_ino;
	umode_t		     i_mode;
	nlink_t		     i_nlink;
	uid_t		     i_uid;
	gid_t		     i_gid;
	dev_t		     i_rdev;
	off_t		     i_size;
	time_t		     i_atime;
	time_t		     i_mtime;
	time_t		     i_ctime;
	unsigned long	 i_blksize;
	unsigned long	 i_blocks;
	struct semaphore i_sem;
	struct inode_operations   *i_op;
	struct super_block        *i_sb;
	struct wait_queue         *i_wait;
	struct file_lock          *i_flock;
	struct vm_area_struct     *i_mmap;
	struct inode              *i_next, *i_prev;
	struct inode              *i_hash_next, *i_hash_prev;
	struct inode              *i_bound_to, *i_bound_by;
	struct inode              *i_mount;
	struct socket             *i_socket;
	unsigned short            i_count;
	unsigned short            i_flags;
	unsigned char             i_lock;
	unsigned char             i_dirt;
	unsigned char             i_pipe;
	unsigned char             i_seek;
	unsigned char             i_update;

	//union {
	//	struct pipe_inode_info  pipe_i;
	//	struct ext_inode_info   ext_i;
	//	struct ext2_inode_info  ext2_i;
	//	void                    *generic_ip;
	//} u;
};

struct file {
	mode_t f_mode;
	off_t f_pos;
	unsigned short f_flags;
	unsigned short f_count;
	off_t f_reada;
	struct file *f_next, *f_prev;
	int f_owner;		/* pid or -pgrp where SIGIO should be sent */
	struct inode * f_inode;
	struct file_operations * f_op;
	void *private_data;	/* needed for tty driver, and maybe others */
};

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

struct file_operations {
	int (*lseek) (struct inode *, struct file *, off_t, int);
	int (*read) (struct inode *, struct file *, char *, int);
	int (*write) (struct inode *, struct file *, char *, int);
	int (*readdir) (struct inode *, struct file *, struct dirent *, int);
	int (*select) (struct inode *, struct file *, int, select_table *);
	int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
	int (*mmap) (struct inode *, struct file *, unsigned long, size_t, int, unsigned long);
	int (*open) (struct inode *, struct file *);
	void (*release) (struct inode *, struct file *);
	int (*fsync) (struct inode *, struct file *);
	int (*fasync) (struct inode *, struct file *, int);
	int (*check_media_change) (dev_t dev);
	int (*revalidate) (dev_t dev);
};



extern int char_read(struct inode *, struct file *, char *, int);
extern int block_read(struct inode *, struct file *, char *, int);

extern int char_write(struct inode *, struct file *, char *, int);
extern int block_write(struct inode *, struct file *, char *, int);

extern int block_fsync(struct inode *, struct file *);
extern int file_fsync(struct inode *, struct file *);

extern void (*do_floppy)(void);

// End of Filesystem.h
#endif
