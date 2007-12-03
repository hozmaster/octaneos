//
// Berlin Brown
//
// $Id: block_devices.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//
// based on ll_rw_block.c
//

#include <system/system.h>
#include <system/alpha.h>
#include <linux/block_devices.h>
#include <linux/errno.h>

#ifndef MAX_BLKDDEV
#define MAX_BLKDEV 32
#endif

/*
 * used to wait on when there are no free requests
 */
struct wait_queue *wait_for_request = NULL;

struct device_struct {
	const char * name;
	struct file_operations * fops;
};

static struct device_struct chrdevs[MAX_CHRDEV] = {
	{ NULL, NULL },
};

static struct device_struct blkdevs[MAX_BLKDEV] = {
	{ NULL, NULL },
};


/* blk_dev_struct is:
 *	do_request-address
 *	next-request
 */
struct blk_dev_struct blk_dev[MAX_BLKDEV] = {
	{ NULL, NULL },		/* no_dev */
	{ NULL, NULL },		/* dev mem */
	{ NULL, NULL },		/* dev fd */
	{ NULL, NULL },		/* dev hd */
	{ NULL, NULL },		/* dev ttyx */
	{ NULL, NULL },		/* dev tty */
	{ NULL, NULL },		/* dev lp */
	{ NULL, NULL },		/* dev pipes */
	{ NULL, NULL },		/* dev sd */
	{ NULL, NULL }		/* dev st */
};

int *blk_size[MAX_BLKDEV] = { NULL, NULL, };

/*
 * blksize_size contains the size of all block-devices:
 *
 * blksize_size[MAJOR][MINOR]
 *
 * if (!blksize_size[MAJOR]) then 1024 bytes is assumed.
 */
int * blksize_size[MAX_BLKDEV] = { NULL, NULL, };

int block_write(struct inode *inode, struct file * filp, char * buf, int count)
{
	return -1;
}

int block_read(struct inode *inode, struct file * filp, char * buf, int count)
{
	return -1;
}

int block_fsync(struct inode *inode, struct file *filp)
{
	return -1;
}

int register_blkdev(unsigned int major, 
					const char *name, struct file_operations *fops)
{
	if (major >= MAX_BLKDEV)
		return -EINVAL;
	if (blkdevs[major].fops)
		return -EBUSY;

	blkdevs[major].name = name;
	blkdevs[major].fops = fops;
	return 0;
}

