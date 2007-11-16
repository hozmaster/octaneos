//
//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: fs_super.c,v 1.6 2005/05/26 00:06:53 bigbinc Exp $
//
// for the time being - filesystem code
//
// I will try to add fs_ as a prefix to code filenames
// 
// I wanted to keep all code in one directory(shrug)
//

#include <system/system.h>
#include <system/alpha.h>
#include <system/beta.h>

//==========================================================
//
// ROOT_DEV = 0x200 - set in init main
//
//==========================================================
int ROOT_DEV = 0x0;

// we are working with ROOT_DEVICE = 0x200 = floppy [ major_nr = 2 ]
//
//  READ = 0
//  WRITE = 1
//  READA = 2
//  WRITEA = 3
//
// [ NO_OPEN = 20 ]
//
// [ NO_INODES = 64 ]
//
// [ NO_FILES = 64 ]
//
// [ NO_SUPER_BLOCKS 8 ]
//
// [ NO_HASH = 307
//
// [ NO_BUFFERS --- 
//
// [ BLOCK_SIZE = 1024 ]
//
// [ BLOCK_SIZE_BITS = 10 ]

//========
// 1024 = BLOCK_SIZE
// see beta.h... typedef char buffer_block[1024];


// [ more externs ]
extern struct class_buffer_head *_buffer_read(int, int);

// [ end of globals ]

// -- [ 8 = NO_SUPER_BLOCKS ] --
struct super_block super_block[8];


static struct super_block *read_super(int);

static struct super_block *read_super(int dev)
{

  struct super_block *s;
  struct class_buffer_head *bh;

  int i;
  int block;
  
  // [ see if the disk has changed
  // mainly dealing with floppies ]
  check_disk_change(dev);


  // [ buffer - read - read the data and check the device magic number ]
  bh = _buffer_read(dev, 1);
  
   
  return NULL;

}


void mount_root(void)
{
  
  char buf[80];
  
  int i;
  int free;

  struct super_block *p;
  struct m_inode *mi;

  __sprintf(buf, "(mount_root)INODE-Size: %d typically = 32\n", sizeof(struct d_inode));
  __puts(buf);
  
  // -- null the super blocks...
  // where 8 = NO_SUPER_BLOCKS - 1 = 7
  for (p = &super_block[0]; p < &super_block[7]; p++) {
    
    p->s_dev = 0;
    p->s_lock = 0;
    
  } // end of the for - clear the super_blocks
  
  // [ we need to mount the floppy ]  
  // [ read the super block ]
  read_super(ROOT_DEV);

  __sprintf(buf, "(mount_root)...done..\n", sizeof(struct d_inode));
  __puts(buf);
  
}
