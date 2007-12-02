//
// Berlin Brown
//
// $Id: block_devices.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//
// based on ll_rw_block.c
//

#include <system/system.h>
#include <system/alpha.h>

extern int *blk_size[];
extern int *blksize_size[];

#define NBUF 64

int block_write(struct inode * inode, struct file * filp, char * buf, int count)
{
	return -1;
}

int block_read(struct inode * inode, struct file * filp, char * buf, int count)
{
	return -1;
}

int block_fsync(struct inode *inode, struct file *filp)
{
	return -1;
}
