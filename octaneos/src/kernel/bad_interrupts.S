/*
 * Berlin Brown
 *
 * bigbinc@hotmail.com
 *
 * $Id: bad_interrupts.S,v 1.1 2004/03/15 00:55:19 bigbinc Exp $
 *
 * basically defined by linux
 *
 * default bad interrupts
 *
 * I know I could have used macros.....	
 */

#define __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/segment.h>




# use FIRST [ 0 to 7 ] [ 8 - 16 -- second ]
	
#	__bad_interrupt_00_01
#	__bad_interrupt_01_02
#	__bad_interrupt_02_04
#	__bad_interrupt_03_08
#	__bad_interrupt_04_10
#	__bad_interrupt_05_20
#	__bad_interrupt_06_40
#	__bad_interrupt_07_80
	
#	__bad_interrupt_08_01
#	__bad_interrupt_09_02
#	__bad_interrupt_10_04
#	__bad_interrupt_11_08
#	__bad_interrupt_12_10
#	__bad_interrupt_13_20
#	__bad_interrupt_14_40
#	__bad_interrupt_15_80

	
# ++ ============================= 00 ========================++	

.align 4
ENTRY(__bad_interrupt_00_01)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x01,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			
	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 01 ========================++	

.align 4
ENTRY(__bad_interrupt_01_02)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x02,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 02 ========================++	

.align 4
ENTRY(__bad_interrupt_02_04)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x04,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 03 ========================++	

.align 4
ENTRY(__bad_interrupt_03_08)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x08,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ 1 2 4 8 10 20 40 80
	
# ++ ============================= 04 ========================++	

.align 4
ENTRY(__bad_interrupt_04_10)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x10,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 05 ========================++	

.align 4
ENTRY(__bad_interrupt_05_20)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x20,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 06 ========================++	

.align 4
ENTRY(__bad_interrupt_06_40)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x40,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 07 ========================++	

.align 4
ENTRY(__bad_interrupt_07_80)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	inb $0x21,%al
	jmp 1f
1:	jmp 1f
1:	orb $0x80,SYMBOL_NAME(cache_21)		# ++++ MASK CODE +++++
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0x20
			
	
	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 08 ========================++	

.align 4
ENTRY(__bad_interrupt_08_01)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x01,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 09 ========================++	

.align 4
ENTRY(__bad_interrupt_09_02)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x02,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 10 ========================++	

.align 4
ENTRY(__bad_interrupt_10_04)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x04,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
				

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 11 ========================++	

.align 4
ENTRY(__bad_interrupt_11_08)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x08,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	
			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret

# ++ ============================= 12 ========================++	

.align 4
ENTRY(__bad_interrupt_12_10)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x10,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================

			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret


# ++ ============================= 13 ========================++	

.align 4
ENTRY(__bad_interrupt_13_20)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x20,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	

			

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret



# ++ ============================= 14 ========================++	

.align 4
ENTRY(__bad_interrupt_14_40)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es


	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x40,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	
					

	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret



# ++ ============================= 15 ========================++	

.align 4
ENTRY(__bad_interrupt_15_80)
	# [ ++ Save Most registers ++ ]
	cld
	push %es
	push %ds
	pushl %eax
	pushl %edx
	pushl %ecx
	movl $SYMBOL_NAME(__KERNEL_DS),%edx
	mov %dx,%ds
	mov %dx,%es

	# ++ SECOND ++ ================================
		
	inb $0xA1,%al
	jmp 1f
1:	jmp 1f	
1:	orb $0x80,SYMBOL_NAME(cache_A1)         # +++++ MASK CODE ++++	
	movb SYMBOL_NAME(cache_A1),%al
	outb %al,$0xA1
	jmp 1f
1:	jmp 1f
1:	movb $0x20,%al
	outb %al,$0xA0
	jmp 1f
1:	jmp 1f
1:	outb %al,$0x20

	# ++ END SECOND ++  ================================
	
	
	# [ ++ Restore ++ ]
	popl %ecx
	popl %edx
	popl %eax
	pop %ds
	pop %es
	iret





