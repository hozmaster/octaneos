//
//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: traps.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//

// ++ set _gate 

#include <system/system.h>

extern char *__vidmem;

//
// see kernelhead.S for initial loading of the idt
//
extern descriptor_table _idt;

#define __set_gate(gate_addr,type,dpl,addr) \
do { \
  int __d0, __d1; \
  __asm__ __volatile__ ( \
        "movw %%dx,%%ax\n\t" \
        "movw %4,%%dx\n\t" \
        "movl %%eax,%0\n\t" \
        "movl %%edx,%1" \
        :"=m" (*((long *) (gate_addr))), \
         "=m" (*(1+(long *) (gate_addr))), "=&a" (__d0), "=&d" (__d1) \
        :"i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
         "3" ((char *) (addr)),"2" (__KERNEL_CS << 16)); \
} while (0)

// ++ end of set gate ++


asmlinkage void load_divide_error(void);
asmlinkage void load_debug_error(void);
asmlinkage void load_nmi_error(void);
  
asmlinkage void load_breakpoint_error(void);
    
asmlinkage void load_overflow_error(void);
  
asmlinkage void load_boundscheck_error(void);
    
asmlinkage void load_invalidop_error(void);
  
asmlinkage void load_device_na_error(void);
asmlinkage void load_doublefault_error(void);
  
asmlinkage void load_segmentover_error(void);
  
asmlinkage void load_invalidtss_error(void);
    
asmlinkage void load_segment_error(void);
  
asmlinkage void load_stack_error(void);
  
asmlinkage void load_protection_error(void);
  
asmlinkage void load_pagefault_error(void);
asmlinkage void load_intel_error(void);
asmlinkage void load_coprocessor_error(void);


//=========== Exception Handlers ==================


// blue screen of death
void __blue_screen(void) {

  int _x;
  int _y;
  
  char _c = ' ';


  unsigned char __attr = 0;
  __attr = 0x17;
  
  __attr ^= 0x08;		// bold
  
  _x = 0;
  _y = 0;

  for (_y = 0; _y < 25; _y++) {

    for (_x = 0; _x < 80; _x++) {
   
      __vidmem [ (( _x + (80 * _y)) * 2) + 0] = _c;
      __vidmem [ (( _x + (80 * _y)) * 2) + 1 ] = __attr;

    }
      
  }
  
}


void panic(const char *str)
{
  static char buf[1024];

  __blue_screen();

  __sprintf(buf, "======================================\n", str); __puts(buf);
  __sprintf(buf, ">> KERNEL PANIC << : %s\n", str); __puts(buf);
  __sprintf(buf, "======================================\n", str); __puts(buf);

  get_all_registers();

  for(;;);

}


// ## Exception 01
void handler_divide_error(void)
{
  panic("Divide Error");  
}


// ## Exception 02
void handler_debug_error(void)
{
  panic("Debug Exception");
  
}

// ## Exception 03
void handler_nmi_error(void)
{
  panic("NMI-Error");  
}

// ## Exception 04
void handler_breakpoint_error(void)
{
  panic("Breakpoint Found");  
}

// ## Exception 05
void handler_overflow_error(void)
{

  panic("Overflow");  
}


// ## Exception 06
void handler_bounds_error(void)
{

  panic("Bounds Check Error");
  
}


// ## Invalid Op-Code -- 7
void handler_invalid_opcode_error(void)
{
   panic("Invalid opcode");
}

// ## Exception 8
void handler_device_error(void)
{
  panic("Coprocessor Not Available");
}

// ## Exception 9
void handler_doublefault_error(void)
{
  panic("Double-Fault Exception");
}

// ## Exception 10
void handler_segmentover_error(void)
{
  panic("Coprocessor Segment Overrun Exception");
}

// ## Exception 11
void handler_tss_error(void)
{
  panic("Invalid TSS\n");
}


// ## Exception 12
void handler_segmentna_error(void) {

  panic("Segment Not Present");

}

// ## Exception 13
void handler_stackexception_error(void) {

  panic("Stack Exception");

}

// ## Exception 14
void handler_triple_error(void) {

  panic("Triple-Fault");

}

// ## Exception 15
void handler_pagefault_error(void) {

  panic("Page-Fault");

}

// ## Exception 16
void handler_intelx_error(void) {
  
  panic("Intel0002");

}

// ## Exception 17
void handler_coprocessor_error(void) {

  panic("Coprocessor Error 002");

}

//=========== (end) Exception Handlers ==================

void _set_intr_gate(unsigned int n, void *addr)
{
  __set_gate(_idt + n,14,0,addr);

}

static void __set_trap_gate(unsigned int n, void *addr)
{
  __set_gate(_idt + n,15,0,addr);

}

// why this has one downslash, Ill never know
void _set_system_gate(unsigned int n, void *addr)
{
  __set_gate(_idt + n,15,3,addr);

}

static void _set_call_gate(void *a, void *addr)
{
  __set_gate(a,12,3,addr);
}

void load_exception_table(void)
{

  // lets get those handlers working!  
  __set_trap_gate(0,&load_divide_error);
  __set_trap_gate(1,&load_debug_error);  
  __set_trap_gate(2,&load_nmi_error);  
  __set_trap_gate(3,&load_breakpoint_error);
    
  __set_trap_gate(4,&load_overflow_error);
  
  __set_trap_gate(5,&load_boundscheck_error);
    
  __set_trap_gate(6,&load_invalidop_error);
  
  __set_trap_gate(7, &load_device_na_error);
  __set_trap_gate(8, &load_doublefault_error);
  
  __set_trap_gate(9, &load_segmentover_error);
  
  __set_trap_gate(10,&load_invalidtss_error);
    
  __set_trap_gate(11, &load_segment_error);
  
  __set_trap_gate(12, &load_stack_error);
  
  __set_trap_gate(13, &load_protection_error);
  
  __set_trap_gate(14, &load_pagefault_error);
  __set_trap_gate(15, &load_intel_error);
  __set_trap_gate(16, &load_coprocessor_error);
  
}

 