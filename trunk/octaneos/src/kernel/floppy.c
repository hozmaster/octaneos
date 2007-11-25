//------------------------------------------------
// Berlin Brown
// berlin.brown@gmail.com
//------------------------------------------------
// $Id: floppy.c,v 1.25 2005/05/26 00:06:53 bigbinc Exp $
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.
//
// See LICENSE.OCTANE for more details
// (Originally based on linux0.1 - Linus Torvalds)
//------------------------------------------------
// ** Revision History and Notes **
//------------------------------------------------
//     
// size         2880
// sect         18
// head         2
// track        80
// stretch      0
// (char)gap    0x1B
// (char)rate   0x00
// (char)spec1  0xCF
// 
// dor = digital output_register

// typically the dma buffer region must be 64k = 64 * 1024
//
// from newos: [
//  dma_address = 4
//  dma_count = 5,
//  dma_command = 8,
//  dma_channelMask = 0xa,
//  dma_mode = 0xb,
//  dma_flipflop = 0xc,
//  dma_page = 0x81,
//  ] 
//
// We need the dma buffer region to be below a MB
//

#include <system/system.h>
#include <asm/io.h>
#include <system/alpha.h>
#include <system/beta.h>
#include <system/dma_functions.h>


#define _BLOCK_SIZE    1024
#define _FLOPPY_DMA    2
#define _MAX_REPLIES   7

#define _FDC_TRACK_NO    255

// as per current - current block request
// -- --------------- _CURRENT_BLOCK_REQ
#define _CURRENT_BLOCK_REQ (public_block_devices[0x02].current_request)
#define _CURRENT_BLOCK_DEV_ (public_block_devices[0x02])

extern void new_floppy_init(void);

extern void _jiffy_delay_setup(int);
extern void _jiffy_start(void);
extern void _jiffy_delay(void);

//
// Floppy_swap_interrup:
// - the function that gets swapped out
//
// see linux = DEVICE_INTR or do_floppy
//
// see function below - lowlevel_floppy_interrupt
void (*floppy_swap_interrupt)(void) = NULL;

// see interrupts.c
extern void handle_interrupt(int);

static unsigned char reply_buffer[_MAX_REPLIES];

// floppy flags
// note: seek is different from seek_track
static int private_recalibrate  = 0;
static int private_reset = 0;
static int private_seek = 0;

// see current_DOC = current output register
static unsigned char current_output_register = 0x0C;

static int private_read_track = 0;
static int private_buffer_track = -1;
static int private_buffer_drive = -1;

// [ variables that define a floppy struct ]
// note: seek_track different from seek above 
static unsigned char private_current_drive = 255;
static unsigned char private_sector = 0;
static unsigned char private_head = 0;
static unsigned char private_track = 0;
static unsigned char private_seek_track =0;
static unsigned char private_command = 0;

static unsigned char private_selected          = 0;

// fdc_track_no = 255 - no private sorry
static unsigned char private_current_track = 255;
static int private_current_spec1 = -1;
static int private_current_rate = -1;


static int           floppy_specs_size        = 2880;
static int           floppy_specs_sectors     = 18;
static int           floppy_specs_heads       = 2;
static int           floppy_specs_track       = 80;
static int           floppy_specs_stretch     = 0;
static unsigned char floppy_specs_gap         = 0x1B;
static unsigned char floppy_specs_rate        = 0x00;
static unsigned char floppy_specs_spec1       = 0xCF;

static volatile int _test_floppy_state = 0;
static volatile int _test_floppy_hits = 0;

//
// see kernelhead.S for the location of floppy_area
//
// [ note: new_floppy_area does not exist below 1MB, 
///  deprecated to work the ISA dma ] 
extern unsigned char tmp_floppy_area[_BLOCK_SIZE];

//
// using new_floppy_area over new_floppy_area
unsigned char *new_floppy_area = NULL;

static void output_byte_fdc(char);
static void do_floppy_request(void);
static int floppy_get_results(void);
static void setup_dma_floppy(void);

static void do_floppy_request(void);
static void floppy_transfer_data(void);

// Note: changed the ecx,edx to memory
// use to copy the changing temp floppy buffer to kernel memory
static void floppy_copy_buffer(void *from, void *to) {  
  int d0, d1, d2;
  __asm__ __volatile__(			\
			"cld\n\t"           \
			"rep ; movsl"       \
			: "=&c" (d0), "=&D" (d1), "=&S" (d2) \
			: "0" (_BLOCK_SIZE / 4),"1" ((long) to),"2" ((long) from) \
			: "memory");
}

void view_floppy_data(void)
{

  char buf[80];
  
  __sprintf(buf,  " >>>>>>>>>> [ flg:%d %d ] seek:%d rst: %d recal:%d drv:%d\n", 
	    _test_floppy_state, _test_floppy_hits, private_seek, private_reset, private_recalibrate, private_current_drive);
  __puts(buf);

  public_hexdump((void *)new_floppy_area, _HEXROWS_8);
}

static void recalibrate_interrupt(void)
{
	
  _test_floppy_hits = 666;
  
  // FD_SENSI = [ 0x08 ]
  output_byte_fdc(0x08);
  
  int _res = 0;
  _res = floppy_get_results();
  
  unsigned char _reply0 = reply_buffer[0];
  
  if (_res != 2 
      || ((_reply0 & 0xE0) == 0x60))
    {
      
      private_reset = 1;
      
    } else {
      
      private_recalibrate = 0;
      
    }
  
  _test_floppy_state = 1;      
  do_floppy_request();
  
}

static void recalibrate_floppy(void)
{
  
  private_recalibrate = 0;
  private_current_track = 0;
  
  floppy_swap_interrupt = recalibrate_interrupt;
  
  // 0x07 = FD_RECALIBRATE - move to track = 0
  output_byte_fdc(0x07);  
  output_byte_fdc(private_head << 2 | private_current_drive);
  
  if (private_reset) {         
      do_floppy_request();      
  }  
}

static void reset_interrupt(void) {

  // where 8 = fd_sensei
  output_byte_fdc(0x08);
  floppy_get_results();
  output_byte_fdc(0x03);
  output_byte_fdc(private_current_spec1);
  output_byte_fdc(6);  
  do_floppy_request();

}

static void _debug_floppy_timer(void) {

  int i;
  unsigned char mask = 0x10;
  for (i = 0; i < 4; i++, mask <<=1) { 
    
    if (!(mask & current_output_register))
      continue;

    current_output_register &= ~mask;    
    outb(current_output_register, 0x3f2);
    
  }

}

//
//  [ floppy_on_interrupt ]
//
static void floppy_on_interrupt(void)
{

  private_selected = 1;
  _test_floppy_state = 2;
  if (private_current_drive != (current_output_register & 0x03))
    {

      current_output_register & 0xFC;
      current_output_register |= private_current_drive;
      
      outb(current_output_register, 0x3f2);
      public_add_timer(2, floppy_transfer_data);
      
    } else {
      
      floppy_transfer_data();
      
    }

}

//
// Use with floppy_on:
//
// turn floppy on - trys to set the current_output_register
//
// to the current drive, to the current drive
//
static void turn_floppy_on(unsigned int the_floppy_number)
{

  _disable_interrupts();
  
  unsigned char mask = 0x10 << the_floppy_number;

  cli();

  mask |= current_output_register;

  if (!private_selected) {
    
    mask &= 0xFC;
    mask |= the_floppy_number;

  } // end of the if -==-=-=
  
  if (mask != current_output_register) {
    /// [ 3f2 = floppy digital output register ]
    outb(mask, 0x3f2);
    current_output_register = mask;
  }

  _enable_interrupts();

}

//
// - [ public function public_floppy_change ] --
//
// typical incoming value = FLOPPY_DEVICE & 0x03 = 0x00
//
// so a minor device of 0x0201, would mean that the floppy_nr = 1
//
//
//
int public_floppy_change(unsigned int nr)
{
  
  char buf[80];

 repeat:
  
  turn_floppy_on(nr);
  
  __sprintf(buf, ".");
  __puts(buf);
  
  // We didnt get it yet, try again [ loop ]
  if ((current_output_register & 0x03) != nr)
    goto repeat;
  
  // digital input register = 0x3f7
  if (inb(0x3f7) & 0x80) {   
      
      // turn the floppy - off      
      // success !
      return 1; 
  }
  
  return 0;
  
}

static void reset_floppy(void) {

  int i;  
  private_reset = 0;
  private_current_spec1 = -1;
  private_current_rate = -1;
  private_recalibrate = 1; 
  _disable_interrupts();
  
  // [ swap with the reset interrupt ] 
  floppy_swap_interrupt = reset_interrupt;
  outb_p((current_output_register & ~0x04), 0x3f2);
  
  for ( i = 0 ; i < 100; i++)
    __asm__("nop");
  
  // [ send current value to FD_DOR = 0x3f2 ] 
  outb(current_output_register, 0x3f2);
  _enable_interrupts();

}

static void debug_floppy_request(int _blk)
{
  
}

//
// Copy data from floppy buffer region to kernel code
//
static void readwrite_interrupt(void) {

  char buf[80];
  int _res = floppy_get_results();

  unsigned char _reply0 = reply_buffer[0];
  unsigned char _reply1 = reply_buffer[1];
  unsigned char _reply2 = reply_buffer[2];
    
  __sprintf(buf, " >>> %d %d %d\n", _reply0, _reply1, _reply2);
  __puts(buf);
	    
  if (_res != 7 
      || (_reply0 & 0xf8) || (_reply1 & 0xbf) || (_reply2 & 0x73)) {
    
    if (_reply1 & 0x02) {
      
      __sprintf(buf, "Drive %d is write protected\n\r", private_current_drive);
      __puts(buf);
            
    } else {

      // nada
    
    }

    do_floppy_request();
    return;
  }
  
  //
  //if (command == FD_READ && (unsigned long)(CURRENT->buffer) >= 0x100000) 
  //  copy_buffer(new_floppy_area,CURRENT->buffer);
  //
  floppy_copy_buffer(new_floppy_area,_CURRENT_BLOCK_REQ->buffer);
   
  do_floppy_request();
  
}

//*******************************************************
//  setup_rw_floppy called by seek()
//*******************************************************
inline void setup_rw_floppy(void)
{

  setup_dma_floppy();

  // Can I finally get some data off the dang floppy!
  // this interrupt swap will copy data from floppy memory to kernel buffer data ] 
  
  floppy_swap_interrupt = readwrite_interrupt;
  output_byte_fdc(private_command);
  
  output_byte_fdc(private_head << 2 | private_current_drive);
  output_byte_fdc(private_track);
  output_byte_fdc(private_head);
  output_byte_fdc(private_sector);

  // sector size = 512
  output_byte_fdc(2);
  output_byte_fdc(floppy_specs_sectors);
  output_byte_fdc(floppy_specs_gap);
  output_byte_fdc(0xFF);

  if (private_reset)
  {
  	    
    do_floppy_request();

  }

}

//
// see transfer for a swap
//
static void seek_swap_interrupt(void)
{

  unsigned char _reply0 = 0;
  unsigned char _reply1 = 0;

  /// sense drive status
  // FD_SESEI = 0x08 = sense interrupt status
  output_byte_fdc(0x08);

  int _res = floppy_get_results();
  
  _reply0 = reply_buffer[0];
  _reply1 = reply_buffer[1];
  
  if (_res != 2 
      || (_reply0 & 0xF8) != 0x20 
      || (_reply1 != private_seek_track)) {
    
    do_floppy_request();
    return;
  }
  
  private_current_track = _reply1;
  setup_rw_floppy();
  
}

//
// see linux transfer
//
static void floppy_transfer_data(void)
{

  // note: initially current_spec1 and rate both set to -1
  _test_floppy_state = 3;
  
  if (private_current_spec1 != floppy_specs_spec1) {
	
	_test_floppy_state = 103;
	
    private_current_spec1 = floppy_specs_spec1;
    
    output_byte_fdc(FD_SPECIFY);
    output_byte_fdc(private_current_spec1);
    output_byte_fdc(6);		// Head load time =6ms, DMA
    
  }
  
  //
  // floppy diskette control register read / write
  // note: FD_DCR = 0x3f7
  //
  if (private_current_rate != floppy_specs_rate)
  {

	_test_floppy_state = 104;
    outb_p(private_current_rate = floppy_specs_rate, 0x3f7);
 
  }
  
  if (private_reset)
  {    
  	_test_floppy_state = 105;
    do_floppy_request();
    return;

  }
  
  if (!private_seek)
  {
  	_test_floppy_state = 106;
    setup_rw_floppy();
    return;
  }
   
  floppy_swap_interrupt = seek_swap_interrupt;
  if (private_seek_track)
  {
    
    _test_floppy_state = 107;
    // 0x0F = FD_SEEK
    output_byte_fdc(0x0F);
    output_byte_fdc(private_head << 2 | private_current_drive);
    output_byte_fdc(private_seek_track);
    
  } else {

	_test_floppy_state = 108;
    // [ FD_RECALIBRATE = 0x07 ]
    output_byte_fdc(0x07);
    output_byte_fdc(private_head << 2 | private_current_drive);
    
  }
  
  if (private_reset) {
	_test_floppy_state = 109;
    do_floppy_request();

  }

}

//
// note: called in from the filesystem,
// block request
//
//  called by readwrite_block - block_devices.c
//  CURRENT       === blk_dev[MAJOR_NR].current_request
//  CURRENT_DEV   === blk_dev[MAJOR_NR].current_request->dev
//
static void do_floppy_request(void)
{

	char buf[80];
	unsigned int _local_block = 0;

 	_test_floppy_state = 4;
	private_seek = 0;

	if (private_reset)
	{
    	reset_floppy();
 	   return;
  	}

 	if (private_recalibrate)
 	{
  
    	recalibrate_floppy();
    	return;  
	}
	
  	// clear the function - part of init_request
  	_CURRENT_BLOCK_DEV_.request_function = NULL;

  	// [ set current_drive = 0x200 = current_dev ]  
  	if (private_current_drive != _CURRENT_BLOCK_REQ->dev)
    	private_seek = 1;

  	private_current_drive = _CURRENT_BLOCK_REQ->dev;
  	_local_block = _CURRENT_BLOCK_REQ->sector;

  	private_sector = _local_block % floppy_specs_sectors;
  	_local_block /= floppy_specs_sectors;

  	private_head = _local_block % floppy_specs_heads;
  	private_track = _local_block / floppy_specs_heads;

  	private_seek_track = private_track << floppy_specs_stretch;

  	if (private_seek_track != private_current_track)
    	private_seek = 1;
  
  	private_sector++;
  	
	//
  	// select the command for the floppy interrupt
  	// decide filesystem read or write READ / WRITE
  	//
  	if (_CURRENT_BLOCK_REQ->cmd == _READ_FLAG) {

    	// note command must equal a floppy command
    	// see... alpha.h
    	// FD_READ = 0xE6
     	private_command = FD_READ;
     	
	} else if (_CURRENT_BLOCK_REQ->cmd == _WRITE_FLAG) {
    
    	//
    	// note command must equal a floppy command
    	// see alpha.h
    	// FD_WRITE = 0xC5
    	//
    	private_command = FD_WRITE;

  	} else {   
    	__asm__("nop");
  	}
  
	__sprintf(buf,  " >>>>>>>>>> seek:%d rst: %d recal:%d REQ:%d\n", 
	    private_seek, private_seek_track, private_head, _local_block);
	__puts(buf);
  
  
  	//
  	// add a timer for 2 - 50 ticks, call 	
  	// [ GO! - from floppy_on_interrupt -> to transfer_data ]
  	//
  	public_add_timer(50, floppy_on_interrupt);   

}


//
// fill in data for a fake block request
//  see test_floppy
//  see beta.h
//
static void _test_block_request(void) {

  _CURRENT_BLOCK_REQ->dev = 0x200;
  _CURRENT_BLOCK_REQ->cmd = _WRITE_FLAG;
  
  _CURRENT_BLOCK_REQ->errors = 0;
  
  _CURRENT_BLOCK_REQ->sector = 32;
  _CURRENT_BLOCK_REQ->nr_sectors = 32;

  _CURRENT_BLOCK_REQ->buffer = NULL;

  _CURRENT_BLOCK_REQ->bh = NULL;
  _CURRENT_BLOCK_REQ->next = NULL;
  
}


void __test_floppy(void)
{

  char buf[80];
  
  unsigned long _floppy_area_addr;
  unsigned long _block_dev_addr;

  // fill in a block request
  _test_block_request();

  _floppy_area_addr = (unsigned long)new_floppy_area;
  _block_dev_addr = (unsigned long)_CURRENT_BLOCK_REQ->buffer;

  __sprintf(buf, "Testing floppy rst: %d recal: %d drv: %d [%x %x ]\n", private_reset, 
	    private_recalibrate, private_current_drive, _floppy_area_addr, _block_dev_addr);
  __puts(buf);

  _test_floppy_state = 5;

  do_floppy_request();
  public_add_timer(0xff0,view_floppy_data);
  
  __sprintf(buf, "done state: [ %d ].\n", _test_floppy_state);
  __puts(buf);

}


//
// after outputting data to the floppy controller
// get some response
//
static int floppy_get_results(void)
{

  char buf[80];

  int i = 0, counter, status;  
  if (private_reset)
    return -9;

  for (counter = 0 ; counter < 10000 ; counter++)
    {
      
      // [ 0x3f4 = FD_STATUS ]       
      status = inb_p(0x3f4) & (_FDC_STATUS_DIR | 
			       _FDC_STATUS_READY | 
			     _FDC_STATUS_BUSY);
      
      if (status == _FDC_STATUS_READY)
	return i;
      
      if (status == (_FDC_STATUS_DIR | _FDC_STATUS_READY
		     | _FDC_STATUS_BUSY))
	{
	  
	  if (i >= _MAX_REPLIES)
	    break;
	  
	  // [ FD_DATA = 0x3f5 ]
	  reply_buffer[i++] = inb_p(FD_DATA);
	  
	}
      
    }
  
  private_reset = 1;
  
  __sprintf(buf, "(FDC) Get-Status timed out\n\r");
  __puts(buf);
  return -1;
  
}

static short _get_version(void)
{

  char buf[80];

  long r;
  unsigned char reply[_MAX_REPLIES];
  int _res = -1;

  // send a register dump

  // [ fd_version = 0x10 ] 
  output_byte_fdc(0x10);  
  
  _res = floppy_get_results();
  
  if (_res != 1) {
    
    __sprintf(buf, " [ Error ] failed to get floppy controller version\n");
    __puts(buf);

  } else {

    __sprintf(buf, " Floppy-Version: %x\n", reply[0]);
    __puts(buf);
    
  } // end of the if - else 
  
  return 0;


}

//***********************************************
// output byte to FD_DATA = 0x3f
//***********************************************
static void output_byte_fdc(char byte) {

  char buf[80];
  int _i;
  unsigned char status;

  if (private_reset)
    return;
  
  for (_i = 0 ; _i < 10000 ; _i++) {

    status = inb_p(FD_STATUS) & (_FDC_STATUS_READY | _FDC_STATUS_DIR);
    if (status == _FDC_STATUS_READY)
    {  	
      // FD_DATA = { 0x3f5 }  or 0x3f0 + 5
      outb(byte, 0x3f5);
      return;

    }

  }
  private_current_track = _FDC_TRACK_NO;
  private_reset = 1;
  __sprintf(buf, "Unable to send byte to FDC\n");
  __puts(buf);

}

static void setup_dma_floppy(void) {

  unsigned long _addr = 0;
  unsigned long count;
  unsigned char _tmp01 = 0;

  // prepare dma for floppy use
  unsigned char dma_code;
  char _chk_fd_read;

  // see alpha.h - DMA_READ = 0x46  
  dma_code = DMA_WRITE;
  if (private_command == FD_READ)
    dma_code = DMA_READ;
     
  count = 1024;

  // see kernelhead.S for floppy_area
  _addr = (unsigned long)new_floppy_area;  
  _disable_interrupts();

  // [ floppy - dma = 0x02 ]
  disable_dma(0x02);
  clear_dma_flipflop(0x02);

  if (private_command == FD_READ)
    _tmp01 = _DMA_MODE_READ;
  else
    _tmp01 = _DMA_MODE_WRITE;

  set_dma_mode(0x02, _tmp01);
  set_dma_addr(0x02, _addr);
  set_dma_count(0x02, count);

  enable_dma(0x02); 
  _enable_interrupts();

}


//
// an unexpected floppy interrupt
static void _unexpected_floppy_interrupt(void)
{

  int _res = 0;
  unsigned char _reply0 = 0;


  // [ sensei = 0x08 ]
  output_byte_fdc(0x08);

  _res = floppy_get_results();
  
  if (_res != 2 || 
      (_reply0 & 0xE0) == 0x60)
    {
      private_reset = 1;      
    } else {     
      private_recalibrate = 1;      
    }
  
  _test_floppy_state = 3;
   
}

static void select_floppy_drive(unsigned int nr) {

  private_seek = 1;
  // 255 = NO_TRACK
  private_current_track = 255;

  current_output_register &= 0xFC;
  current_output_register |= private_current_drive;
  
  // FD_DOR = 0x3f2
  outb(current_output_register, 0x3f2);

  _jiffy_delay_setup(10);
  _jiffy_start();
  _jiffy_delay();

 
}

// [ deprecated = code not currently being used ] 
static void deprecated_turn_floppy_on(void)
{
  
  unsigned int nr = 0x02;

  unsigned char mask = 0x10 << nr;
  
  select_floppy_drive(nr);
  
  current_output_register &= 0xfc;
  current_output_register |= mask;
  current_output_register |= nr;
  outb(current_output_register, 0x3f2);  

}

static void deprecated_turn_floppy_off(void) {

  unsigned int nr = 0x02;
  unsigned char mask = ~(0x10 << nr);
  _disable_interrupts();

  current_output_register &= mask;
  outb(current_output_register, 0x3f2);

  _enable_interrupts();

}


void __debug_floppy(void) {

}

// See hardware_interrupt06 - embedded there
//
// design taken from linux floppy code
//
void lowlevel_floppy_interrupt(void)
{
#if 1
  void (*actual_interrupt_function)(void) = floppy_swap_interrupt;
  
  // clear it out 
  floppy_swap_interrupt = NULL;  
  if (!actual_interrupt_function) {
    // default...
    actual_interrupt_function = _unexpected_floppy_interrupt;
  }
#else

#endif

}

//
// See if we can find the size and if we have an A/B/both drive
void floppy_get_drives(void)
{

  // Code from bona-fide osdev
  char buf[80];

  char *drive_type[6] = { 
    "No Floppy Drive Detected", 
    "360-KB 5.25-in. Floppy", 
    "1.2-MB 5.25-in. Floppy", 
    "720-KB 3.5-in. Floppy", 
    "1.44-MB 3.5-in. Floppy", 
    "2.88-MB 3.5-in. Floppy"
  };
  unsigned char c;

  int a = 0;
  int b = 0;

  outb(0x10, 0x70);
  c = inb(0x71);

  a = c >> 4;
  b = c & 0xF;

  __sprintf(buf, "Floppy drive A: %s\n", drive_type[a]); __puts(buf);
  __sprintf(buf, "Floppy drive B: %s\n", drive_type[b]); __puts(buf);
 
}

static void _setup_dma_floppy_area(void) {

  int i;  
  // Note: from the linux kernel code
  // this pointer must be aligned so that it is not on a 64kb region

  // - only want 1024
  new_floppy_area = (unsigned char *)0x80000;
  for (i = 0; i < 1024; i++) {
    new_floppy_area[i] = (unsigned char)0x00;

  }
}

//
// Initialize the floppy device
// see main.c - invoked at the entry point
void floppy_init(void) {

#if 1

  // block_request = current_request
  _setup_dma_floppy_area();
  _test_floppy_state = 1;

  // fill - in block device struct data
  // See block_devices.c
  // Note the floppy is major number = 2   
  public_block_devices[0x02].request_function = do_floppy_request;
   
  // data 0x0C to port { 0x3f2 }
  // enable FDC
  // Step 1.
  outb(current_output_register, 0x3f2);
 
  // Step 2.
  // and then load the interrupt
  // this probably needs to before trying to write the floppy controller  
  handle_interrupt(0x06);
  
  // Step 3.
  // 0x02 = FLOPPY_DMA  
  request_dma(0x02);  
    
  // Step 4.
  // super swap out floppy interrupt
  // set low-level function to nothing 
  floppy_swap_interrupt = _unexpected_floppy_interrupt;
  _get_version();

#else

  new_floppy_init();

#endif

}
