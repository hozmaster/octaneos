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

#ifndef MAX_BLKDDEV
#define MAX_BLKDEV 32
#endif

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
