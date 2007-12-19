/*
 * Berlin Brown
 *
 * zboothead.S based on below
 *
 * linux/boot/head.S
 *
 * boot/setup.S should have called us
 *
 * Next we need to decompress our kernel
 *  - we are not making it here(the promise land)
 *  
 *
 *  Copyright (C) 1991, 1992, 1993  Linus Torvalds
 */

/*
 *  head.S contains the 32-bit startup code.
 */
.text

#include <linux/linkage.h>
#include <asm/segment.h>

.globl startup_32
		
startup_32:
	cld
	cli
	movl $(KERNEL_DS),%eax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs

	
	lss SYMBOL_NAME(stack_start),%esp
	xorl %eax,%eax
1:	incl %eax		# check that A20 really IS enabled
	movl %eax,0x000000	# loop forever if it isn't
	cmpl %eax,0x100000
	je 1b
	
	pushl $0
	popfl
	
	xorl %eax,%eax
	movl $SYMBOL_NAME(_edata),%edi
	movl $SYMBOL_NAME(_end),%ecx
	subl %edi,%ecx
	cld
	rep
	stosb
			
	subl $32,%esp	# pass 4 kernel args
    movl %esp,%eax

	pushl %esi	# 4th real-mode ptr, prepared in boot/setup.S	
	pushl %ds	# 3rd
	pushl %cs	# 2nd
	pushl %eax	# 1st
	
	call SYMBOL_NAME(decompress_kernel)

	# Assume - loaded high
	movl $move_routine_start,%esi
	movl $0x1000,%edi
	movl $move_routine_end,%ecx
	subl %esi,%ecx
	addl $3,%ecx
	shrl $2,%ecx
	cld
	rep
	movsl
    popl %esi
    popl %ebx
    popl %esi
    popl %ecx	
    popl %edx
    popl %eax

	#
	# We need to fake these values
	#
	movl	$0x2000, %esi	# Low-Buffer Start
	movl	$0xfff000, %ecx	# Assume kernel is really-really big
	movl	$0x18E000, %edx # High-Buffer Start
	movl	$0x0, %eax	# HCount

	# End
	
    movl $0x100000,%edi
    cli
    ljmp $(KERNEL_CS), $0x1000
	#
	# End
	#

#
# We do make it to this routine:
#
move_routine_start:
	movl %ecx,%ebp
	shrl $2,%ecx
	rep
	movsl
	movl %ebp,%ecx
	andl $3,%ecx
	rep
	movsb
	movl %edx,%esi
	movl %eax,%ecx
	addl $3,%ecx
	shrl $2,%ecx
	rep
	movsl
	movl %ebx,%esi  # Restore setup pointer
	xorl %ebx,%ebx	
	ljmp $(KERNEL_CS), $0x100000
move_routine_end:

/*
 * End of source
 */
