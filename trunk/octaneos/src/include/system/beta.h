///
//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// beta.h
//
//
// a lot of filesystem code
//
// $Id: beta.h,v 1.10 2005/05/26 00:06:45 bigbinc Exp $
//

#ifndef __BETA_H_
#define __BETA_H_

//
// major and minor device numbers
//
//   Major         Minor
// [0000][0000] [0000][0000]
//
// typically the device format will be 0x200 (hex)
// for example the floppy device  0 has a
// MAJOR number of 2 and a minor of 0
//  
// and hd0 = major 3, minor = partition = part1 = 1?
//
// in linux code,
//
// the defines to look out for are ROOT_DEV = 0x200
// MAJOR/MINOR and MAJOR_NR
//
// MAJOR = no mask
// MINOR = no mask
// ROOT_DEV = mask
// MAJOR_NR = no mask
//
// typically variables [dev] = mask
//
//
// in terms of block - drivers
//
// you typically see dev & 0x03
//  - for example [ 0x200 & 0x03 ]
//


// shift and get major device number ie 2
// probably should be unmask...whatever
#define GET_MAJOR_MASK(a) (((unsigned) (a))>>8)
#define GET_MINOR_MASK(a) ((a)&0xff)


// see linux fs.h  ...
#define _READ_FLAG        0
#define _WRITE_FLAG       1
#define _READ_AHEAD_FLAG  2
#define _WRITE_AHEAD_FLAG 3


// major_no = 2 = FLOPPY_MAJOR
// rootdev = FLOPPY_DEVICE
#define _FLOPPY_DEVICE           0x200
#define _FLOPPY_DEVICE_NO        2
// see define above...
#define _FLOPPY_MAJOR            GET_MAJOR_MASK(0x200)
#define _FLOPPY_MINOR            GET_MINOR_MASK(0x000)


#define NO_BLOCK_REQUESTS        32
#define NO_BLOCK_DEVICES         7

struct class_buffer_head {

  char *buffer_data;
  unsigned long buffer_blocknr;

  unsigned short buffer_dev;
  unsigned char buffer_uptodate;
  unsigned char buffer_dirt;

  unsigned char buffer_count;
  unsigned char buffer_lock;


  struct class_buffer_head *buffer_prev;
  struct class_buffer_head *buffer_next;
  struct class_buffer_head *buffer_prev_free;
  struct class_buffer_head *buffer_next_free;
  
};

struct class_block_request {
  
  int dev;
  int cmd;
  int errors;

  unsigned long sector;
  unsigned long nr_sectors;
  
  char *buffer;

  struct class_buffer_head *bh;
  struct class_block_request *next;
};


//
//
//
// see block_devices. 
//
// : 0 = no_dev
// : 2 = fd
//
struct class_block_device {

  void (*request_function)(void);
  struct class_block_request *current_request;

};


// see... block_devices.c
extern struct class_block_request public_block_requests[NO_BLOCK_REQUESTS];
extern struct class_block_device public_block_devices[NO_BLOCK_DEVICES];


// see public_buffer_head above 
struct d_inode {

  unsigned short    i_mode;
  unsigned short    i_uid;
  unsigned long     i_size;
  unsigned long     i_time;
  unsigned char     i_gid;
  unsigned char     i_nlinks;

  unsigned short    i_zone[9];

};

struct file {
  unsigned short f_mode;
  unsigned short f_flags;
  unsigned short f_count;

  struct m_inode *f_inode;

  //off_t f_pos;
};


struct m_inode {

  unsigned short i_mode;
  unsigned short i_uid;
  unsigned long i_size;
  unsigned long i_mtime;
  unsigned char i_gid;
  unsigned char i_nlinks;
  unsigned short i_zone[9];
 
  unsigned long i_atime;
  unsigned long i_ctime;
  unsigned short i_dev;
  unsigned short i_num;
  unsigned short i_count;
  unsigned char i_lock;
  unsigned char i_dirt;
  unsigned char i_pipe;
  unsigned char i_mount;
  unsigned char i_seek;
  unsigned char i_update;
};

//
// Note: only buffer_head name has changed up to this point
//  --- changed from buffer_head to class_buffer_head
//
struct super_block {

  unsigned short s_ninodes;
  unsigned short s_nzones;
  unsigned short s_imap_blocks;
  unsigned short s_zmap_blocks;
  unsigned short s_firstdatazone;
  unsigned short s_log_zone_size;
  unsigned long s_max_size;
  unsigned short s_magic;
  
  struct class_buffer_head *s_imap[8];
  struct class_buffer_head *s_zmap[8];

  unsigned short s_dev;

  struct m_inode *s_isup;
  struct m_inode *s_imount;

  unsigned long s_time;

  unsigned char s_lock;
  unsigned char s_rd_only;
  unsigned char s_dirt;
};

typedef char buffer_block[1024];

//  --- [ end of filesystem structs ]
//=========================================================

#endif
