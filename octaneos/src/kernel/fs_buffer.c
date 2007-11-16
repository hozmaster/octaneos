//
// Berlin Brown
//
//
// bigbinc@hotmail.com
//
// $Id: fs_buffer.c,v 1.4 2005/05/26 00:06:53 bigbinc Exp $
//
//


#include <system/system.h>
#include <system/alpha.h>
#include <system/beta.h>

// see floppy.c...
extern int public_floppy_change(unsigned int);

extern void readwrite_block(int, struct class_buffer_head *);


// see readwrite_block - block_devices -
// - buffer_read = bread
struct class_buffer_head *_buffer_read(int dev, int block)
{
  
  struct class_buffer_head *_bh;
  
  readwrite_block(_READ_FLAG, _bh);

  return NULL;

}


//
// [ see fs_super, where this is called...
// - the mask is defined in beta.h
//      .... called by read_super..  ]
//
void check_disk_change(int dev)
{

  // [ where dev = 0x200 ]
  int i;

  // if we dont have a floppy, exit
  if (GET_MAJOR_MASK(dev) != 0x02) 
    return;

  // change the floppy - incoming = 0x00
  // see... floppy.c
  if (!public_floppy_change(dev & 0x03)) {
    
    // something went wrong, see floppy.c - turn_floppy_on/off
    return;

  }

}
