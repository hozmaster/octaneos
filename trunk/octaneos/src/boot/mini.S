/*
 * Berlin Brown
 *
 * bigbinc@hotmail.com
 *
 * $Id: mini.S,v 1.1 2004/03/14 03:21:39 bigbinc Exp $
 *
 *
 * Linux parts copyrighted Linus Torvalds
 *
 * Note: of course the zipped kernel gets this code
 * 	this is where a lot of problems are caused.
 */

#define __ASSEMBLY__
	
#define SIG1	0xAA55
#define SIG2	0x5A5A

INITSEG  = 0x9000 
SYSSEG   = 0x1000
SETUPSEG = 0x9020

.code16
.globl begtext, begdata, begbss, endtext, enddata, endbss

.text
begtext:
.data
begdata:
.bss
begbss:
.text

start:
	jmp	start_of_setup

start_of_setup:
	movw	$0x01500, %ax
	movb	$0x81, %dl
	int	$0x13
	
	jmp	begin_spirit

# Routine to print asciiz string at ds:si
prtstr:
	lodsb
	andb	%al, %al
	jz	fin

	call	prtchr
	jmp	prtstr

fin:	ret

# Space printing
prtsp2:	call	prtspc		# Print double space
prtspc:	movb	$0x20, %al	# Print single space (note: fall-thru)

# Part of above routine, this one just prints ascii al
prtchr:	pushw	%ax
	pushw	%cx
	xorb	%bh, %bh
	movw	$0x01, %cx
	movb	$0x0e, %ah
	int	$0x10
	popw	%cx
	popw	%ax
	ret

beep:	movb	$0x07, %al
	jmp	prtchr
	

#
# Execute setup code...
#
begin_spirit:
	movw	$0x0305, %ax
	xorw	%bx, %bx
	int	$0x16

	cli
	movb	$0x80, %al
	outb	%al, $0x70

#
# Move the system
#
	movw	$0x100, %ax
	movw	$0x1000, %bx
	cld
do_move:
	movw	%ax, %es
	incb	%ah
	cmpw	$0x9000, %ax
	jz	end_move
	movw	%bx, %ds
	addw	$0x100, %bx
	subw	%di, %di
	subw	%si, %si
	movw	$0x800, %cx
	rep
	movsw
	jmp	do_move


#
# Moving on....(blbz)
#
end_move:
	movw	$SETUPSEG, %ax
	movw	%ax, %ds

	lidt	idt_48
	lgdt	gdt_48

# enable a20
	call	empty_8042
	movb	$0xD1, %al
	outb	%al, $0x64
	call	empty_8042
	movb	$0xDF, %al
	outb	%al, $0x60
	call	empty_8042

# reset coprocessor
	xorw    %ax, %ax
	outb    %al, $0xf0
	call    delay
	outb    %al, $0xf1
	call    delay


#
# Some more A20 - stuff - reprogram interrupts
#
	movb	$0x11, %al
        outb	%al,$0x20
        call    delay
        outb	%al,$0xA0
        call	delay
        movb	$0x20, %al
        outb	%al, $0x21
        call	delay
        movb	$0x28, %al
        outb	%al,$0xA1
        call	delay
        movb	$0x04, %al
        outb	%al,$0x21
        call	delay
        movb	$0x02, %al
        outb	%al,$0xA1
        call	delay
        movb	$0x01, %al
        outb	%al,$0x21
        call	delay
        outb	%al,$0xA1
        call	delay

        movb	$0xFF, %al
        outb	%al,$0xA1
        call    delay

        movb	$0xFB, %al
        outb	%al, $0x21

#
# end big A20 segment
#	

	lidt	idt_48
	xorl	%eax, %eax
	movw	%ds, %ax

	shll	$4, %eax
	addl	$gdt, %eax
	movl	%eax, (gdt_48+2)
	lgdt	gdt_48

# reset coprocessor --
	xorw	%ax, %ax
	outb	%al, $0xf0
	call	delay

	outb	%al, $0xf1
	call	delay

#
# (blbz) we are getting closer enter 32-bit mode
#
	movw	$1, %ax
	lmsw	%ax
	jmp	flush_instr
flush_instr:

	xorw	%bx, %bx
	xorl	%esi, %esi
	movw	%cs, %si
	subw	$0x20, %si
	shll	$4, %esi


# Make jump...
	.byte 	0x66, 0xea
code32:	.long   0x1000
	.word   0x10

#
# empty_8042 - check for empty keyboard queue
#
empty_8042:
        call	delay
        inb	$0x64,%al
        testb	$1, %al
        jz	no_output
        call	delay
        inb	$0x60, %al
        jmp	empty_8042
no_output:
        testb	$2, %al
        jnz	empty_8042
        ret

#
# function: delay
#
delay:
	outb	%al, $0x80
	ret

#
# Note: toggle 0xFFFF - 4GB limit to 
# gdt and idt will make or break your OS debugging
#
gdt:
	.word   0,0,0,0
	.word   0,0,0,0
	.word   0xFFFF		# 8mb
	.word   0x0000
	.word   0x9A00
	.word   0x00CF		# granularity

	.word   0xFFFF		# 8mb
	.word   0x0000
	.word   0x9200
	.word   0x00CF		# granularity
	
idt_48:
        .word   0
        .word   0,0

gdt_48:
        .word   0x8000
        .word   0,0



# Setup signature -- must be last
setup_sig1:	.word	SIG1
setup_sig2:	.word	SIG2

modelist:

.text
endtext:
.data
enddata:
.bss
endbss:
