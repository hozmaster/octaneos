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

/*
 * These are the fs-independent mount-flags: up to 16 flags are supported
 */
#define MS_RDONLY    1 /* mount read-only */
#define MS_NOSUID    2 /* ignore suid and sgid bits */
#define MS_NODEV     4 /* disallow access to device special files */
#define MS_NOEXEC    8 /* disallow program execution */
#define MS_SYNC     16 /* writes are synced at once */
#define	MS_REMOUNT  32 /* alter flags of a mounted FS */

/*
 * Flags that can be altered by MS_REMOUNT
 */
#define MS_RMT_MASK (MS_RDONLY)


#define MS_MGC_VAL 0xC0ED0000 /* magic flag number to indicate "new" flags */
#define MS_MGC_MSK 0xffff0000 /* magic flag number mask */

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

#include <linux/ext2_fs_sb.h>
struct super_block {
	dev_t s_dev;
	unsigned long s_blocksize;
	unsigned char s_blocksize_bits;
	unsigned char s_lock;
	unsigned char s_rd_only;
	unsigned char s_dirt;
	struct super_operations *s_op;
	unsigned long s_flags;
	unsigned long s_magic;
	unsigned long s_time;
	struct inode * s_covered;
	struct inode * s_mounted;
	struct wait_queue * s_wait;
	union {
		struct ext2_sb_info ext2_sb;
		void *generic_sbp;
	} u;
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

struct inode_operations {
	struct file_operations * default_file_ops;
	int (*create) (struct inode *,const char *,int,int,struct inode **);
	int (*lookup) (struct inode *,const char *,int,struct inode **);
	int (*link) (struct inode *,struct inode *,const char *,int);
	int (*unlink) (struct inode *,const char *,int);
	int (*symlink) (struct inode *,const char *,int,const char *);
	int (*mkdir) (struct inode *,const char *,int,int);
	int (*rmdir) (struct inode *,const char *,int);
	int (*mknod) (struct inode *,const char *,int,int,int);
	int (*rename) (struct inode *,const char *,int,struct inode *,const char *,int);
	int (*readlink) (struct inode *,char *,int);
	int (*follow_link) (struct inode *,struct inode *,int,int,struct inode **);
	int (*bmap) (struct inode *,int);
	void (*truncate) (struct inode *);
	int (*permission) (struct inode *, int);
};

struct super_operations {
	void (*read_inode) (struct inode *);
	int (*notify_change) (int flags, struct inode *);
	void (*write_inode) (struct inode *);
	void (*put_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	void (*write_super) (struct super_block *);
	void (*statfs) (struct super_block *, struct statfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
};

struct file_system_type {
	struct super_block *(*read_super) (struct super_block *, void *, int);
	char *name;
	int requires_dev;
	struct file_system_type * next;
};

extern int read_ahead[];

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);

asmlinkage int sys_open(const char *, int, int);
asmlinkage int sys_close(unsigned int);		/* yes, it's really unsigned */

extern void kill_fasync(struct fasync_struct *fa, int sig);

extern int getname(const char * filename, char **result);
extern void putname(char * name);

extern int register_blkdev(unsigned int, const char *, struct file_operations *);
extern int unregister_blkdev(unsigned int major, const char * name);
extern int blkdev_open(struct inode * inode, struct file * filp);
extern struct file_operations def_blk_fops;
extern struct inode_operations blkdev_inode_operations;

extern int register_chrdev(unsigned int, const char *, struct file_operations *);
extern int unregister_chrdev(unsigned int major, const char * name);
extern int chrdev_open(struct inode * inode, struct file * filp);
extern struct file_operations def_chr_fops;
extern struct inode_operations chrdev_inode_operations;

extern void init_fifo(struct inode * inode);

extern struct file_operations connecting_fifo_fops;
extern struct file_operations read_fifo_fops;
extern struct file_operations write_fifo_fops;
extern struct file_operations rdwr_fifo_fops;
extern struct file_operations read_pipe_fops;
extern struct file_operations write_pipe_fops;
extern struct file_operations rdwr_pipe_fops;

extern struct file_system_type *get_fs_type(char *name);

extern int fs_may_mount(dev_t dev);
extern int fs_may_umount(dev_t dev, struct inode * mount_root);
extern int fs_may_remount_ro(dev_t dev);

extern struct file *first_file;
extern int nr_files;
extern struct super_block super_blocks[NR_SUPER];

extern int shrink_buffers(unsigned int priority);
extern void refile_buffer(struct buffer_head * buf);
extern void set_writetime(struct buffer_head * buf, int flag);
extern void refill_freelist(int size);

extern struct buffer_head **buffer_pages;
extern int nr_buffers;
extern int buffermem;
extern int nr_buffer_heads;

#define BUF_CLEAN 0
#define BUF_UNSHARED 1 /* Buffers that were shared but are not any more */
#define BUF_LOCKED 2   /* Buffers scheduled for write */
#define BUF_LOCKED1 3  /* Supers, inodes */
#define BUF_DIRTY 4    /* Dirty buffers, not yet scheduled for write */
#define BUF_SHARED 5   /* Buffers shared */
#define NR_LIST 6


extern int char_read(struct inode *, struct file *, char *, int);
extern int block_read(struct inode *, struct file *, char *, int);

extern int char_write(struct inode *, struct file *, char *, int);
extern int block_write(struct inode *, struct file *, char *, int);

extern int block_fsync(struct inode *, struct file *);
extern int file_fsync(struct inode *, struct file *);

extern void (*do_floppy)(void);


// End of Filesystem.h
#endif
