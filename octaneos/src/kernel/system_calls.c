//
// Copyright (C) 2003, 2007 Berlin Brown
// $Id: system_calls.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//
// Note: using asmlinkage for going down into an interrupt
//   its dangerous down there
// actual system_call function is in 
//  assembly code,  exceptions.S
//

#include <linux/linkage.h>
#include <asm/segment.h>
#include <asm/io.h>

asmlinkage int system_debug(void) {

  // note: eax input and eax output
  char buf[80];
  
  __sprintf(buf, ">>> System Call Called <<<\n");
  __puts(buf);

  return -1;

}

int __public_debug(int _in) {

  long __res;
  
  // 
  // Note: eax represents the offset in the sys_call table
  //
  // note: eax input and eax output
  //  in case you aren't fresh on your inline asm

  __asm__ volatile ("int $0x80"                    \
		    : "=a" (__res)                 \
		    : "0" (0x0),"b" ((long)(-1))   \
		    ); 
  
  return -1;

}
