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

#include <system/major_devices.h>

#include <linux/string.h>
#include <linux/signal.h>

#include <linux/block_devices_structs.h>

#include <system/filesystem.h>

extern struct blk_dev_struct blk_dev[MAX_BLKDEV];

void test_block_1(void) {
	
	struct blk_dev_struct *dev;
	unsigned int major;

	printk("*** Running block 1\n");
	
	dev = &blk_dev[FLOPPY_MAJOR];
	if (!dev || !dev->request_fn) {
		printk("ll_rw_block: Trying to read nonexistent block-device (%ld)\n");
		return;
	} else {
		printk("have block device");
	}
	
	struct inode _inode;
	struct file _file;

	struct file_operations *fops = get_blkfops(FLOPPY_MAJOR);
	
	fops->open(NULL, NULL);
	fops->release(NULL, NULL);

}
