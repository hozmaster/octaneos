/*
 *
 * Berlin Brown
 * bigbinc@hotmail.com
 *
 * $Id: lowlevel.S,v 1.4 2004/03/28 23:41:00 bigbinc Exp $ - misc assembly routines
 *
 * Dont forget the 'asmlinkage' in your C file
 *
 */


#define __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/segment.h>


#
# ++ Get system registers ++
#
#
#
# - we are going to push a lot of stuff onto the stack
#
# note:	 C is a really big macro assembler
#


#
#	 push %gs	 ++++++++++++++++++
#	 push %fs	 ++++++++++++++++++
#        push %es	 ++++++++++++++++++
#        push %ds	 ++++++++++++++++++
#        pushl %eax	 ++++++++++++++++++
#        pushl %ebp	 ++++++++++++++++++
#        pushl %edi	 ++++++++++++++++++
#        pushl %esi	 ++++++++++++++++++	
#        pushl %edx	 ++++++++++++++++++
#        pushl %ecx	 ++++++++++++++++++
#        pushl %ebx	 ++++++++++++++++++
	
#
# variables we put on the stack ==
#
# here placed in order they placed on it
#
	
REGI_EBX	= 0x00
REGI_ECX	= 0x04
REGI_EDX	= 0x08
	
REGI_ESI	= 0x0C	
REGI_EDI	= 0x10
REGI_EBP	= 0x14
REGI_EAX	= 0x18
REGI_DS		= 0x1C
REGI_ES		= 0x20
REGI_FS		= 0x24
REGI_GS		= 0x28

REGI_OLD_EAX	= 0x2C
REGI_EIP	= 0x30
REGI_CS		= 0x34
REGI_EFLAGS	= 0x38
	
MAX_REGS_STACK	= 0x38

# == end stack return setup ==

ENTRY(get_all_registers)
	movl $-MAX_REGS_STACK, REGI_EAX(%esp)
	pushl %gs			# 1
	pushl %fs			# 2
	pushl %es			# 3
	pushl %ds			# 4
	pushl %eax			# 5
	pushl %ebp			# 6
	pushl %edi			# 7
	pushl %esi			# 8
	pushl %edx			# 9
	pushl %ecx			# 10
	pushl %ebx			# 11
				
	call SYMBOL_NAME(print_register_list)
		
	popl %ebx			# 11
	popl %ecx			# 10
	popl %edx			# 9
	popl %esi			# 8
	popl %edi			# 7
	popl %ebp			# 6
	popl %eax			# 5
	popl %ds			# 4
	popl %es			# 3
	popl %fs			# 2
	popl %gs			# 1					
	ret





	
