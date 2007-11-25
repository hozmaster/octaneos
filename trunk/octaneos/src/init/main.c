// 
// Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
// 
// $Id: main.c,v 1.30 2005/05/26 00:06:54 bigbinc Exp $
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.
//
// See LICENSE.OCTANE for more details

#include <system/system.h>
#include <asm/io.h>

#include <system/jiffies_value.h>

// Note: this is a part of buildct.c getting the build count

// generated from python scripts
extern unsigned long main_get_build_count;
extern char *main_get_version;

// debugging printk
extern void __puts(const char *);
extern int __sprintf(char * buf, const char *fmt, ...);
extern void load_misc_kprint(void);

// Exceptions
extern void load_exception_table(void);
extern void load_interrupts(void);
extern int check_timer_irq(void);

extern void load_keyboard_driver(void);

asmlinkage void get_all_registers(void);

extern void print_register_list(struct debug_registers *);
extern descriptor_table _idt;

extern unsigned char __debug_scan_code;

extern void _jiffy_delay_setup(int);
extern void _jiffy_start(void);
extern void _jiffy_delay(void);

//
// External Floppy Driver functions
extern void floppy_init(void);
extern void __debug_floppy(void);

//
// End of floppy functions

extern void block_devices_init(void);
extern void scheduler_init(void);
extern int __public_debug(int);
extern void mount_root(void);
extern void public_timer_test(void);


void __test_floppy(void);

char  *__vidmem = (char *)0xb8000;

//
// Draw Characters
//
static void _video_draw_char(const char _c, int _x, int _y) {

  // black = 0
  // blue = 1
  // green = 2
  // cyan = 3
  // red = 4
  // magenta = 5
  // brown = 6
  // white = 7
  // gray = 8
  // light blue = 9
  // light green = A
  // light cyan = B
  // light red = C
  // light mag = D
  // yellow = E
  // white = F 
	
  // attr/char
	
  // foreground colors = 0,1,2,3,4,5,6,7
  // bgcolors = 0,1,2,3,4,6,7
  unsigned char __attr = 0;
	
  // white on blue 
  __attr = 0x17;

  // bold = 0/1
  // underline = 0/1
  // reverse = 0/1
  // blink = 0/1
  // color = color

  __attr ^= 0x08;		// bold

  __vidmem [ (( _x + (80 * _y)) * 2) + 0] = _c;
  __vidmem [ (( _x + (80 * _y)) * 2) + 1 ] = __attr;
	
}

static void _draw_char(char _c,int _x, int _y) {

  __vidmem [ ( _x + 80 * _y ) * 2 ] = _c;	
	
}

static void view_PIC(void) {

  char buf[255];
  unsigned int v;
  v = inb(0xa1) << 8 | inb(0x21);

  __sprintf(buf," [ PIC  IMR ] : %04x\n", v); __puts(buf);

  v = inb(0xa0) << 8 | inb(0x20);
  __sprintf(buf," [ PIC  IRR: %04x\n", v); __puts(buf);
  
  outb(0x0b,0xa0);
  outb(0x0b,0x20);
  v = inb(0xa0) << 8 | inb(0x20);
  outb(0x0a,0xa0);
  outb(0x0a,0x20); 
  __sprintf(buf," [ PIC  ISR ] : %04x\n", v); __puts(buf);
  v = inb(0x4d1) << 8 | inb(0x4d0);
  __sprintf(buf," [ PIC ELCR ] : %04x\n", v); __puts(buf);
  
}

//
// Start Kernel
//
asmlinkage void start_kernel(void) {

  char buf[255];
  int _i = 0;
  int _diff = 0;

  _video_draw_char('S', 79,0);
  _video_draw_char('p', 79,1);
  _video_draw_char('i', 79,2);
  _video_draw_char('r', 79,3);
  _video_draw_char('i', 79,4);
  _video_draw_char('t', 79,5);
  _video_draw_char('X', 79,6);

  // Draw a banner line of the right side of the screen
  for (_i = 7; _i < 24; _i++) {
    _video_draw_char(' ', 79,_i);
  }
	
  // Setup Code

  load_misc_kprint();  
  __sprintf(buf, "=========== [ Project Akita ] ===========\n"); __puts(buf);
  __sprintf(buf, "[ Build: %ld; since { 5/15/2005 } %s]\n", 
		  	main_get_build_count, main_get_version); __puts(buf);

  // Set the root device to the FLOPPY
  // system.h to fs_super.c
  ROOT_DEV = 0x200;
    
  // now lets load the IDT
  load_exception_table();
  
  load_interrupts();
  load_keyboard_driver();

  floppy_get_drives();      
  _diff = check_timer_irq();
      
  // another check
  scheduler_init();  
  __sprintf(buf, "delay [ "); __puts(buf);
  for (_i = 0; _i < 5; _i++) {

    _jiffy_delay_setup(100);
    _jiffy_start();
    _jiffy_delay();
    __sprintf(buf, "."); __puts(buf);
    
  } // end of the for 
  __sprintf(buf, " ]\n"); __puts(buf);

  block_devices_init();
  
  /// ** deprecated while testing the new floppy driver **
  floppy_init(); 

  // ==================== LOCKDOWN ========================  
  for(;;)
  {
    _jiffy_delay_setup(40);
    _jiffy_start();
    _jiffy_delay();

    if ((_jiffies % 60000) == 0)
    {      
      __sprintf(buf, "[%d]",__debug_scan_code);
      __puts(buf);
    }

    if (__debug_scan_code != 0x00)
    {
      __sprintf(buf, "[%d]", __debug_scan_code);
      __puts(buf);
      
      __debug_scan_code = 0x00;

    }

  }
	
}

//========================================================
// End of File
//========================================================


