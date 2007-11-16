/*
 *
 * Berlin Brown
 *
 * 
 * $Id: hardware_interrupts.S,v 1.6 2004/04/10 14:16:25 bigbinc Exp $
 *
 *
 * assembly wrapper functions for interrupts
 *
 * the actual work will get done elsewhere
 *
 */

#define __ASSEMBLY__
	
#include <linux/linkage.h>
#include <asm/segment.h>

#define SAVE_ALL	\
	cld;		\
	push %gs;	\
	push %fs;	\
        push %es;	\
        push %ds;	\
        pushl %eax;	\
        pushl %ebp;	\
        pushl %edi;	\
        pushl %esi;	\
        pushl %edx;	\
        pushl %ecx;	\
        pushl %ebx;	\
        movl $SYMBOL_NAME(__KERNEL_DS),%edx;	\
        mov %dx,%ds;	\
        mov %dx,%es;	\
        movl $SYMBOL_NAME(__USER_DS),%edx;	\
        mov %dx,%fs;	\
        movl $0,%edx;

#define RESTORE_ALL \
	popl %ebx; \
	popl %ecx; \
	popl %edx; \
	popl %esi; \
	popl %edi; \
	popl %ebp; \
	popl %eax; \
	pop %ds; \
	pop %es; \
	pop %fs; \
	pop %gs; \
	addl $4,%esp; \
	iret
	
		
#define PIC_CLOSE_FIRST(mask)	\
	inb $0x21,%al;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	andb $~(mask),cache_21;	\
        movb cache_21,%al;	\
        outb %al,$0x21;

#define PIC_CLOSE_SECOND(mask)	\
	inb $0xA1,%al;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	andb $~(mask),cache_A1;	\
	movb cache_A1,%al;	\
        outb %al,$0xA1;

	
#define SETUP_PIC01(mask)	 \
	inb $0x21,%al;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	orb $mask,cache_21;	\
	movb cache_21,%al;	\
	outb %al,$0x21;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	movb $0x20,%al;		\
	outb %al,$0x20;


	
#define SETUP_PIC02(mask)	\
	inb $0xA1,%al;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	orb $mask,cache_A1;	\
	movb cache_A1,%al;	\
	outb %al,$0xA1;		\
	jmp 1f;			\
1:	jmp 1f;			\
1:	movb $0x20,%al;		\
	outb %al,$0xA0;		\
	jmp 1f;		        \
1:	jmp 1f;			\
1:	outb %al,$0x20;
		
#
# [ ++ set_intr_gate prepares the actual interrupt ++ ]
#
# we just need a way back to C
#

# see docs/notes.txt

# 0 [ ..... 0x01	system_timer
# 1 [ ..... 0x02	keyboard
# 2 [       0x04    
# 3 [       0x08
# 4 [       0x10
# 5         0x20	harddrive
# 6         0x40	floppy
# 7         0x80
# -=-=--=-=- Second PIC -=-=-=-=--=--=
# 8         0x01
# 9         0x02
# 10        0x04
# 11        0x08
# 12        0x10
# 13        0x20  ....... ]
# 14        0x40  ......  ]
# 15        0x80  ......  ]

	
#
# ++================ TIMER_IRQ - 0000 =================++
#
	
ENTRY(hw_interrupt_entry_00)
	pushl $-0-2	
	SAVE_ALL
	
        SETUP_PIC01(0x01)                      # ... [ Edit Accordingly ] ....
	
	sti
	movl %esp,%ebx
	pushl %ebx
	pushl $0


	# .... see interrupts.c for timer_irq
	call SYMBOL_NAME(_public_timer_irq)	# ++ do something ++
	
	addl $8,%esp
	cli

	PIC_CLOSE_FIRST(0x01)			# ... [ Edit Accordingly ] ....

	RESTORE_ALL

#
# ++================ KEYBOARD_IRQ - 0001 =================++
#
	
ENTRY(hw_interrupt_entry_01)
	pushl $-0-2	
	SAVE_ALL
	
        SETUP_PIC01(0x02)				# ... [ Edit Accordingly ] ....
	
	sti
	movl %esp,%ebx
	pushl %ebx
	pushl $0
	
	call SYMBOL_NAME(__debug_keyboard_interrupt)	# ++ do something ++
	
	addl $8,%esp
	cli

	PIC_CLOSE_FIRST(0x02)				# ... [ Edit Accordingly ] ....

	RESTORE_ALL


#
# ++================ FLOPPY_IRQ - 0006 =================++
# see floppy.c
	
ENTRY(hw_interrupt_entry_06)
	pushl $-0-2	
	SAVE_ALL
	
        SETUP_PIC01(0x40)			# ... [ Edit Accordingly ] ....
	
	sti
	movl %esp,%ebx
	pushl %ebx
	pushl $0
	
	call SYMBOL_NAME(lowlevel_floppy_interrupt)	# ++ do something ++
	
	addl $8,%esp
	cli

	PIC_CLOSE_FIRST(0x40)			# ... [ Edit Accordingly ] ....

	RESTORE_ALL


