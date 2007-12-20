/* 
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 * 
 * $Id: main.c,v 1.30 2005/05/26 00:06:54 bigbinc Exp $
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
#include <asm/io.h>
#include <system/jiffies_value.h>
#include <linux/time.h>

#include <tests/_test_traps.h>

// Note: this is a part of buildct.c getting the build count

// generated from python scripts
extern unsigned long main_get_build_count;
extern char *main_get_version;
extern char _end;

int ramdisk_size;

int root_mountflags = 0;

// debugging printk
extern void __puts(const char *);
extern int __sprintf(char * buf, const char *fmt, ...);
extern void load_misc_kprint(void);

// Exceptions
extern void load_exception_table(void);
extern void init_interrupts(void);
extern int check_timer_irq(void);

extern void load_keyboard_driver(void);

asmlinkage void get_all_registers(void);

extern void print_register_list(struct debug_registers *);

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

static unsigned long memory_start = 0;
static unsigned long memory_end = 0;
static unsigned long low_memory_start = 0;

extern void __debug_floppy_open();
extern void __debug_floppy_release();

extern struct timeval xtime;

simple_time cur_simpletime = {
	0, 0, 0, 0, 0, 0
};

//------------------------------------------------
// Init/Main Function Definitions
//------------------------------------------------


static void _video_draw_char(const char _c, int _x, int _y) {
	
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

static void _nop() {}

static void tests() {
	
	//test_traps_1();

}

//
// Start Kernel
asmlinkage void start_kernel(void) {

  char buf[255];
  int _i = 0;
  int _diff = 0;
  _video_draw_char('O', 79, 10);
  _video_draw_char('c', 79, 11);
  _video_draw_char('t', 79, 12);
  _video_draw_char('a', 79, 13);
  _video_draw_char('n', 79, 14);
  _video_draw_char('e', 79, 15);
  _video_draw_char(' ', 79, 16);
  _video_draw_char(' ', 79, 17);

  // Draw a banner line of the right side of the screen
  for (_i = 0; _i < 10; _i++)  { _video_draw_char(' ', 79,_i); }
  for (_i = 18; _i < 22; _i++) { _video_draw_char(' ', 79,_i); }  
   
  load_misc_kprint();  
  __sprintf(buf, "=========== Octane ===========\n"); __puts(buf);
  __sprintf(buf, "@INFO: Build: %ld; since (10/15/2007) %s\n", 
		  	main_get_build_count, main_get_version); __puts(buf);

  //memory_end = (1<<20) + (EXT_MEM_K<<10);
  //  memory_end &= PAGE_MASK;
  memory_end = 16*1024*1024;
  memory_start = 1024*1024;
  low_memory_start = (unsigned long) &_end;

  // now lets load the IDT
  load_exception_table();

  init_interrupts();
  sched_init();
  load_keyboard_driver();
  
  //memory_start = blk_dev_init(memory_start, memory_end);
  sti();
  //calibrate_delay();
  
  //memory_start = inode_init(memory_start, memory_end);
  //memory_start = file_table_init(memory_start, memory_end);  
  //mem_init(low_memory_start, memory_start, memory_end);
  //buffer_init();
  time_init();
 
  // Print the current time.
  printk("INFO: current time: %d\n", xtime.tv_sec);
  printk("INFO: %d/%d/%d %d:%d\n", cur_simpletime.mon,
		 cur_simpletime.day, cur_simpletime.year,
		 cur_simpletime.hour, cur_simpletime.min);

  floppy_init();
  sti();

  //__debug_floppy_open();
  //__debug_floppy_release();
  test_time_1();
  test_block_1();

  tests();
  for(;;) {  
	  _nop();
  }
}

//------------------------------------------------
// End of File
//------------------------------------------------



