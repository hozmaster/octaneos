/*
 *
 * Berlin Brown
 * bigbinc@hotmail.com
 * $Id: kernelhead.S,v 1.4 2005/05/26 00:06:53 bigbinc Exp $
 *
 * This is exactly before, we enter into the kernel
 * We begin to get into C code
 * -- start_kernel is called and then we exit kernel/head.S
 *
 * should be compiled with ASSEMBLY defined
 * --
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 * -- 
 */

.text
#include <linux/tasks.h>	/* only tasks = 128	*/
#include <linux/linkage.h>	/* SYMBOL_NAME, etc	*/
#include <asm/segment.h>	/* KERNEL_DS, etc	*/

#define __PAGE_OFFSET           (0xC0000000)

#define BOOT_USER_PGD_PTRS (__PAGE_OFFSET >> TWOLEVEL_PGDIR_SHIFT)
#define BOOT_KERNEL_PGD_PTRS (1024-BOOT_USER_PGD_PTRS)


#define CL_MAGIC_ADDR   0x90020
#define CL_MAGIC        0xA33F
#define CL_BASE_ADDR    0x90000
#define CL_OFFSET       0x90022


startup_32:
	cld
	movl $(__KERNEL_DS), %eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs

# ++ Setup Stack ++

	lss 	stack_start,%esp

	xorl %eax, %eax
	movl $SYMBOL_NAME(_edata),%edi
	movl $SYMBOL_NAME(_end),%ecx
	subl %edi,%ecx
	cld
	rep
	stosb

# ++ Enter 32-bit setup ++
	call setup_idt
	
	xorl %eax, %eax
1:	incl %eax
	movl %eax, 0x000000
	cmpl %eax, 0x100000
	je 1b

	pushl $0
	popfl

	movl $0x90000, %esi
	movl $SYMBOL_NAME(empty_zero_page),%edi
	movl $512, %ecx
	cld
	rep
	movsl
	xorl %eax, %eax
	movl $512, %ecx
	rep
	stosl

	cmpw $(CL_MAGIC),CL_MAGIC_ADDR
	jne 1f
	movl $SYMBOL_NAME(empty_zero_page)+2048,%edi
	movzwl CL_OFFSET,%esi
	addl $(CL_BASE_ADDR),%esi
	movl $2048,%ecx
	rep
	movsb
1:

# ++ Assuming 486 or better ++
	movl $1, %eax
	.byte 0x0f, 0xa2
	andl $0xf00, %eax
	shrl $8, %eax

	movl %cr0,%eax
	andl $0x80000011,%eax
	orl $0x50022,%eax
	jmp 2f
2:								  
	movl %eax, %cr0

	call check_x87
	call setup_paging


	lgdt gdt_descr
	lidt idt_descr


# ++ Jump to start_kernel ++

# [ change the current selector with the code below ]
	ljmp $(__KERNEL_CS),$1f
1:      movl $(__KERNEL_DS),%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs
	lss stack_start,%esp

	xorl %eax,%eax
	lldt %ax

	pushl %eax
	pushl %eax
	pushl %eax
	cld
	call SYMBOL_NAME(start_kernel)
loopit:
	jmp loopit

#
# Perform x87 checks
#
check_x87:
	clts
	fninit
	fstsw %ax
ALIGN
1:
	.byte 0xDB,0xE4
	ret



/*
 * ++ Functions ++
 */
setup_idt:
	lea ignore_int,%edx
	movl $(__KERNEL_CS << 16),%eax
	movw %dx,%ax
	movw $0x8E00,%dx
	lea SYMBOL_NAME(_idt),%edi
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	dec %ecx
	jne rp_sidt
	ret
int_msg:
	.asciz "Unknown interrupt\n"
	ALIGN
ignore_int:
	cld
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ds
	pushl %es
	pushl %fs
	movl $(__KERNEL_DS),%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	pushl $int_msg
	
	#call SYMBOL_NAME(printk)

	popl %eax
	popl %fs
	popl %es
	popl %ds
	popl %edx
	popl %ecx
	popl %eax
	iret

ALIGN
setup_paging:
        movl $1024*2,%ecx
        xorl %eax,%eax
        movl $SYMBOL_NAME(swapper_pg_dir),%edi
        cld;rep;stosl

        movl $SYMBOL_NAME(pg0)+7,SYMBOL_NAME(swapper_pg_dir)

        movl $SYMBOL_NAME(pg0)+7,SYMBOL_NAME(swapper_pg_dir)+3072
        movl $SYMBOL_NAME(pg0)+4092,%edi
        movl $0x03ff007,%eax
        std
1:      stosl
        subl $0x1000,%eax
        jge 1b
        cld
        movl $SYMBOL_NAME(swapper_pg_dir),%eax
        movl %eax,%cr3
        movl %cr0,%eax
        orl $0x80000000,%eax
        movl %eax,%cr0
        ret
	
	

/*
 * ++ End of Functions and Code
 */
	
.org 0x1000
ENTRY(swapper_pg_dir)

.org 0x2000
ENTRY(pg0)

.org 0x3000
ENTRY(pg1)

.org 0x4000
ENTRY(empty_zero_page)

.org 0x5000

.org 0x6000
	
# ++ Note: These are used by the floppy block drivers 
# 	in the kernel code
# [ see... kernel/floppy.c where this is used ]
ENTRY(tmp_floppy_area)
	.fill 1024,1,0

ENTRY(floppy_track_buffer)
	.fill 512*2*18,1,0
	

# -------------------- [ begin data section ] -------------

ENTRY(stext)
ENTRY(_stext)
.data

# ++ Descriptor Tables and such ++

ALIGN
.word 0

ENTRY(idt_descr)
	.word 256*8-1
	.long 0xc0000000+SYMBOL_NAME(_idt)
	.align 4
ENTRY(_idt)
	.fill 256,8,0


ALIGN
.word 0
ENTRY(gdt_descr)
	.word (8+2*NR_TASKS)*8-1
	.long 0xc0000000+SYMBOL_NAME(gdt)

		
ALIGN
ENTRY(gdt)
	.quad 0x0000000000000000
	.quad 0x0000000000000000
	.quad 0xc0c39a000000ffff
	.quad 0xc0c392000000ffff
	.quad 0x00cbfa000000ffff
	.quad 0x00cbf2000000ffff
	.quad 0x0000000000000000
	.quad 0x0000000000000000
	.fill 2*NR_TASKS,8,0
