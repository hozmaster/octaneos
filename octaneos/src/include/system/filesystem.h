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

#include <system/major_devices.h>
#include <linux/sched.h>

//************************************************
//
// Filesystem Datastructures
//
//************************************************
typedef char buffer_block[BLOCK_SIZE];

struct buffer_head {
	char            *b_data;			/* pointer to data block (1024 bytes) */
	unsigned long    b_size;		    /* block size */
	unsigned long    b_blocknr;	        /* block number */
	dev_t            b_dev;			    /* device (0 = free) */
	unsigned short   b_count;		    /* users using this block */
	unsigned char    b_uptodate;
	unsigned char    b_dirt;		    /* 0-clean,1-dirty */
	unsigned char    b_lock;		    /* 0 - ok, 1 -locked */
	unsigned char    b_req;		        /* 0 if the buffer has been invalidated */
	unsigned char    b_list;		    /* List that this buffer appears */
	unsigned char    b_retain;          /* Expected number of times this will
					                       be used.  Put on freelist when 0 */
	unsigned long    b_flushtime;       /* Time when this (dirty) buffer should be written */
	unsigned long    b_lru_time;        /* Time when this buffer was last used. */
	struct wait_queue  *b_wait;
	struct buffer_head *b_prev;		    /* doubly linked list of hash-queue */
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
