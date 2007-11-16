//
// Berlin Brown
//
//
// bigbinc@hotmail.com
//
//
// $Id: keyboard.c,v 1.3 2005/05/26 00:06:53 bigbinc Exp $
//
// keyboard driver
//


#include <system/system.h>
#include <asm/io.h>

volatile unsigned char __debug_scan_code = 0x00;

void __debug_keyboard_interrupt(void) {
  
  __debug_scan_code = inb(0x60);

}

void load_keyboard_driver(void) {

  // GO with the keyboard_interrupt
  __disable_irq(0x01);

  handle_interrupt(0x01);

}

