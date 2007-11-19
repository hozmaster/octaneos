//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: system.h,v 1.16 2005/05/26 00:06:45 bigbinc Exp $
//
// system.h

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#define SYMBOL_NAME_STR(X) #X

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif


#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif


#define SYMBOL_NAME_LABEL(X) X/**/:

#define __ALIGN .align 4,0x90
#define __ALIGN_STR ".align 4,0x90"

#define SYMBOL_NAME_STR(X) #X
#define SYMBOL_NAME(X) X


// ++++++++++++++ assembly block ++++++++++++++++++++++++
#ifdef __ASSEMBLY__

#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#define ENTRY(name) \
  .globl SYMBOL_NAME(name); \
  ALIGN; \
  SYMBOL_NAME_LABEL(name)

#endif  
// ++++++++++++++ end of assembly block ++++++++++++++++++

#define __KERNEL_CS 0x10
#define __KERNEL_DS 0x18

#define __USER_CS   0x23
#define __USER_DS   0x2B

typedef struct descr_struct {
  
  unsigned long a;
  unsigned long b;  
  
} descriptor_table[256];

//
// see the kernelhead.S for loading of the idt
//
// we could reload it again in 'C', but who has the time
//
// loaded in assembly
//

//
// enable and disable interrupt flags 
//
#define cli() __asm__ __volatile__("cli": : :"memory")
#define sti() __asm__ __volatile__("sti": : :"memory")

#define _disable_interrupts() __asm__ __volatile__("cli": : :"memory")
#define _enable_interrupts() __asm__ __volatile__("sti": : :"memory")


#define nop() __asm__ __volatile__ ("nop")

#define save_flags(x) __asm__ __volatile__("pushfl ; popl %0":"=g" (x):  :"memory")
#define restore_flags(x) __asm__ __volatile__("pushl %0 ; popfl": :"g" (x):"memory")

#define __MAX_DEBUG_TASKS           3
#define _FIRST_TSS            0x08


//
// In those rare-cases, we may need the values of the following registers
//
struct debug_registers {

  long __ebx;
  long __ecx;
  long __edx;
  long __esi;
  long __edi;
  long __ebp;
  long __eax;
  int  __ds;
  int  __es;
  int  __fs;
  int  __gs;

  long __old_eax;
  long __eip;
  int  __cs;
  long __eflags;

  long __esp;
  int  __ss;
};



struct TSS_object {

  unsigned long __esp0;
  unsigned long __ss0;
  
  unsigned long __esp1;
  unsigned long __ss1;
  
  unsigned long __esp2;
  unsigned long __ss2;
  
  unsigned long __cr3;
  unsigned long __eip;
  unsigned long __eflags;
  
  unsigned long __eax;
  unsigned long __ecx;
  unsigned long __edx;
  unsigned long __ebx;
  unsigned long __esp;
  unsigned long __ebp;
  unsigned long __esi; 
  unsigned long __edi;

  unsigned long __es;
  unsigned long __cs;
  unsigned long __ss;
  unsigned long __ds;
  unsigned long __fs;
  unsigned long __gs;

  unsigned long __ldtr;
  unsigned int  __trace;
  unsigned int  __io_bitmap_address;

  unsigned char __io_bitmap[8192];

};


// [ without the bitmap ]

struct __TSS_object {

  unsigned long __esp0;
  unsigned long __ss0;
  
  unsigned long __esp1;
  unsigned long __ss1;
  
  unsigned long __esp2;
  unsigned long __ss2;
  
  unsigned long __cr3;
  unsigned long __eip;
  unsigned long __eflags;
  
  unsigned long __eax;
  unsigned long __ecx;
  unsigned long __edx;
  unsigned long __ebx;
  unsigned long __esp;
  unsigned long __ebp;
  unsigned long __esi; 
  unsigned long __edi;

  unsigned long __es;
  unsigned long __cs;
  unsigned long __ss;
  unsigned long __ds;
  unsigned long __fs;
  unsigned long __gs;

  unsigned long __ldtr;
  unsigned int  __trace;
  unsigned int  __io_bitmap_address;

};

//////////////
// task switcher
//
// 

#define switch_task(tss)    \
    __asm__("cli\n\t"       \
            "ljmp *0x0\n\t"   \
            "sti"           \
            :               \
            :               \
            )

//
// for low-level kernel debugging
//
extern int __sprintf(char *, const char *, ...);
extern void __puts(const char *);

//
// Note once and for all -=-==-== [ NULL ]
//
#ifndef NULL
#define NULL ((void *) 0)
#endif

// - see kernel/fs_super.c - root device, typically 0x200 = floppyx
extern int ROOT_DEV;

extern void __puts(const char *);
extern int __sprintf(char * buf, const char *fmt, ...); 


extern void public_add_timer(long, void (*)(void));
extern void public_hexdump(void *, int);


//
// defined for use with hexdump -
//
#define _HEXROWS_4    32
#define _HEXROWS_8    64
#define _HEXROWS_16   128

#endif 