/*
 *  linux/fs/filesystems.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  table of configured filesystems
 */

extern void device_setup(void);

#include <system/system.h>

/**
 * This may be used only once, enforced by 'static int callable' 
 */
asmlinkage int sys_setup(void) {

	static int callable = 1;
	if (!callable) {
		return -1;
	}

	callable = 0;
	device_setup();

	//register_filesystem(&(struct file_system_type)
	//	{ext2_read_super, "ext2", 1, NULL});

	mount_root();
	return 0;
}
