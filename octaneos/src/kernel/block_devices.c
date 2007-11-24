//
// Berlin Brown
//
// $Id: block_devices.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//
// based on ll_rw_block.c
//

#include <system/system.h>
#include <system/alpha.h>
#include <system/beta.h>

struct class_block_request public_block_requests[NO_BLOCK_REQUESTS];

// see beta.h...
// // 7 = number of block devices
struct class_block_device public_block_devices[7] = {
  { NULL, NULL },         // [ 0 ] no_dev
  { NULL, NULL },         // [ 1 ] mem
  { NULL, NULL },         // [ 2 ] fd
  { NULL, NULL },         // [ 3 ] hd
  { NULL, NULL },         // [ 4 ] ttyx
  { NULL, NULL },         // [ 5 ] tty
  { NULL, NULL }          // [ 6 ] lp
};


/// see fs_buffer.c - called by bread, for example
void readwrite_block(int readwrite_flag, struct class_buffer_head *bh)
{

  char buf[80];

  unsigned int major_dev;

  major_dev = GET_MAJOR_MASK(bh->buffer_dev);

  // for example for floppy: request_func =  { do_floppy_request }
  if (!public_block_devices[major_dev].request_function) {

    __sprintf(buf, "[readwrite_block] Invalid block request\n");
    __puts(buf);
    
    return;
  }
  
}

void block_devices_init(void)
{

  int i;

  for (i = 0; i < NO_BLOCK_REQUESTS; i++)
  {
        
    public_block_requests[i].dev  = -1;
    public_block_requests[i].next = NULL;
    

  }

}

