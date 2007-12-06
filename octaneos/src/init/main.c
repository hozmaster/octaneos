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
extern void init_interrupts(void);
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
    
  // now lets load the IDT
  load_exception_table();
  
  init_interrupts();
  load_keyboard_driver();
      
  scheduler_init();  
  floppy_init(); 

  for(;;) {  
    _jiffy_delay_setup(40);
    _jiffy_start();
    _jiffy_delay();

    if ((jiffies % 60000) == 0) {  
      __sprintf(buf, "[%d]",__debug_scan_code);
      __puts(buf);
    }
  }
}

//------------------------------------------------
// End of File
//------------------------------------------------



