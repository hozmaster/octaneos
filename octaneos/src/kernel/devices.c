/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: block_devices.c (see ll_rw_block in linux kernel)
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

#include <system/system.h>
#include <linux/errno.h>
#include <linux/page.h>
#include <linux/kernel_stat.h>

#include <system/major_devices.h>
#include <linux/block_devices.h>

#include <linux/string.h>


struct device_struct {
	const char *name;
	struct file_operations * fops;
};

static struct device_struct chrdevs[MAX_CHRDEV] = {
	{ NULL, NULL },
};

static struct device_struct blkdevs[MAX_BLKDEV] = {
	{ NULL, NULL },
};

int get_device_list(char *page) {

	int i;
	int len;
	len = sprintf(page, "Character devices:\n");
	for (i = 0; i < MAX_CHRDEV ; i++) {
		if (chrdevs[i].fops) {
			len += __sprintf(page+len, "%2d %s\n", i, chrdevs[i].name);
		}
	}
	len += sprintf(page+len, "\nBlock devices:\n");
	for (i = 0; i < MAX_BLKDEV ; i++) {
		if (blkdevs[i].fops) {
			len += __sprintf(page+len, "%2d %s\n", i, blkdevs[i].name);
		}
	}
	return len;
}

struct file_operations *get_blkfops(unsigned int major) {
	if (major >= MAX_BLKDEV)
		return NULL;
	return blkdevs[major].fops;
}

struct file_operations *get_chrfops(unsigned int major)
{
	if (major >= MAX_CHRDEV)
		return NULL;
	return chrdevs[major].fops;
}

int register_chrdev(unsigned int major, const char * name, struct file_operations *fops) {
	if (major >= MAX_CHRDEV)
		return -EINVAL;
	if (chrdevs[major].fops)
		return -EBUSY;
	chrdevs[major].name = name;
	chrdevs[major].fops = fops;
	return 0;
}

int register_blkdev(unsigned int major, const char * name, struct file_operations *fops) {

	if (major >= MAX_BLKDEV)
		return -EINVAL;
	if (blkdevs[major].fops)
		return -EBUSY;
	blkdevs[major].name = name;
	blkdevs[major].fops = fops;
	return 0;
}

int unregister_chrdev(unsigned int major, const char * name)
{
	if (major >= MAX_CHRDEV)
		return -EINVAL;
	if (!chrdevs[major].fops)
		return -EINVAL;
	if (strcmp(chrdevs[major].name, name))
		return -EINVAL;
	chrdevs[major].name = NULL;
	chrdevs[major].fops = NULL;
	return 0;
}

int unregister_blkdev(unsigned int major, const char *name) {

	if (major >= MAX_BLKDEV)
		return -EINVAL;
	if (!blkdevs[major].fops)
		return -EINVAL;
	if (strcmp(blkdevs[major].name, name))
		return -EINVAL;

	blkdevs[major].name = NULL;
	blkdevs[major].fops = NULL;
	return 0;
}

/*
 * This routine checks whether a removable media has been changed,
 * and invalidates all buffer-cache-entries in that case. This
 * is a relatively slow routine, so we have to try to minimize using
 * it. Thus it is called only upon a 'mount' or 'open'. This
 * is the best way of combining speed and utility, I think.
 * People changing diskettes in the middle of an operation deserve
 * to loose :-)
 */
int check_disk_change(dev_t dev) {

	int i;
	struct file_operations * fops;

	i = MAJOR(dev);
	if (i >= MAX_BLKDEV || (fops = blkdevs[i].fops) == NULL)
		return 0;
	if (fops->check_media_change == NULL)
		return 0;
	if (!fops->check_media_change(dev))
		return 0;

	printk("VFS: Disk change detected on device %d/%d\n",
					MAJOR(dev), MINOR(dev));
	for (i=0 ; i<NR_SUPER ; i++)
		if (super_blocks[i].s_dev == dev)
			put_super(super_blocks[i].s_dev);

	//invalidate_inodes(dev);
	//invalidate_buffers(dev);

	if (fops->revalidate)
		fops->revalidate(dev);
	return 1;
}

/**
 * Called every time a block special file is opened.
 *
 * In the case of the floppy device driver, the floppy_open
 * function will get called.
 *
 * (see filp->f_op->open)
 */
int blkdev_open(struct inode *inode, struct file *filp) {

	int i;

	i = MAJOR(inode->i_rdev);
	if (i >= MAX_BLKDEV || !blkdevs[i].fops) {
		return -ENODEV;
	}

	filp->f_op = blkdevs[i].fops;
	if (filp->f_op->open) {
		return filp->f_op->open(inode,filp);
	}
	return 0;
}	

/*
 * Dummy default file-operations: the only thing this does
 * is contain the open that then fills in the correct operations
 * depending on the special file...
 */
struct file_operations def_blk_fops = {
	NULL,		    /* lseek */
	NULL,		    /* read */
	NULL,		    /* write */
	NULL,		    /* readdir */
	NULL,		    /* select */
	NULL,		    /* ioctl */
	NULL,           /* mmap */
	blkdev_open,	/* open */
	NULL,		    /* release */
};

struct inode_operations blkdev_inode_operations = {
	&def_blk_fops,	/* default file operations */
	NULL,			/* create */
	NULL,			/* lookup */
	NULL,			/* link */
	NULL,			/* unlink */
	NULL,			/* symlink */
	NULL,			/* mkdir */
	NULL,			/* rmdir */
	NULL,			/* mknod */
	NULL,			/* rename */
	NULL,			/* readlink */
	NULL,			/* follow_link */
	NULL,			/* bmap */
	NULL,			/* truncate */
	NULL			/* permission */
};

/*
 * Called every time a character special file is opened
 */
int chrdev_open(struct inode *inode, struct file *filp) {

	int i;
	i = MAJOR(inode->i_rdev);
	if (i >= MAX_CHRDEV || !chrdevs[i].fops) {
		return -ENODEV;
	}

	filp->f_op = chrdevs[i].fops;
	if (filp->f_op->open) {
		return filp->f_op->open(inode,filp);
	}

	return 0;
}

/*
 * Dummy default file-operations: the only thing this does
 * is contain the open that then fills in the correct operations
 * depending on the special file...
 */
struct file_operations def_chr_fops = {
	NULL,		/* lseek */
	NULL,		/* read */
	NULL,		/* write */
	NULL,		/* readdir */
	NULL,		/* select */
	NULL,		/* ioctl */
	NULL,		/* mmap */
	chrdev_open,	/* open */
	NULL,		/* release */
};

struct inode_operations chrdev_inode_operations = {
	&def_chr_fops,		/* default file operations */
	NULL,			/* create */
	NULL,			/* lookup */
	NULL,			/* link */
	NULL,			/* unlink */
	NULL,			/* symlink */
	NULL,			/* mkdir */
	NULL,			/* rmdir */
	NULL,			/* mknod */
	NULL,			/* rename */
	NULL,			/* readlink */
	NULL,			/* follow_link */
	NULL,			/* bmap */
	NULL,			/* truncate */
	NULL			/* permission */
};
