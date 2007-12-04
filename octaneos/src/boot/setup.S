//
// Copyright (C) 2003, 2007 Berlin Brown
//
// $Id: setup.S,v 1.1 2004/03/14 03:21:39 bigbinc Exp $, we are assuming a big kernel
//
// Original Source:
// Setup.S		Copyright (C) 1991, 1992 Linus Torvalds
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#define __ASSEMBLY__

#define SIG1	0xAA55
#define SIG2	0x5A5A

INITSEG  = 0x9000
SYSSEG   = 0x1000
SETUPSEG = 0x9020

DELTA_INITSEG = SETUPSEG - INITSEG	# 0x0020

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
	jmp	trampoline

# = cs:2 (old 0x9020:2)

		.ascii	"HdrS"
		.word	0x0201

#
# ++ Send Messages to 32-bit mode ++
#
realmode_swtch:	.word	0, 0			# 0, 2

start_sys_seg:	.word	SYSSEG			# 4
		.word	kernel_version		# 6			

type_of_loader:	.byte	0			# 8 

extra_spirit_msg:
		.word	kernel_message		# 9 
		.word	kernel_version		# 11 


loadflags:
LOADED_HIGH	= 1 
CAN_USE_HEAP	= 0x80
	
		.byte	LOADED_HIGH	

setup_move_size: .word  0x8000
	
code32_start:
		.long	0x100000 

ramdisk_image:	.long	0

ramdisk_size:	.long	0

bootsect_kludge:
		.word  bootsect_helper, SETUPSEG

heap_end_ptr:	.word	modelist+1024
	
trampoline:	call	start_of_setup
		.space	1024

start_of_setup:
	movw	$0x01500, %ax
	movb	$0x81, %dl
	int	$0x13

	movw	%cs, %ax
	movw	%ax, %ds

	cmpw	$SIG1, setup_sig1
	jne	bad_sig

	cmpw	$SIG2, setup_sig2
	jne	bad_sig

	jmp	good_sig1

# Routine to print asciiz string at ds:si
prtstr:
	lodsb
	andb	%al, %al
	jz	fin

	call	prtchr
	jmp	prtstr

fin:	ret

# Space printing
prtsp2:	call	prtspc
prtspc:	movb	$0x20, %al

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
	
no_sig_mess: .string	"No setup signature found ..."

good_sig1:
	jmp	good_sig

bad_sig:
	movw	%cs, %ax
	subw	$DELTA_INITSEG, %ax
	movw	%ax, %ds
	xorb	%bh, %bh
	movb	(497), %bl
	subw	$4, %bx
	shlw	$8, %bx
	movw	%bx, %cx
	shrw	$3, %bx
	addw	$SYSSEG, %bx
	movw	%bx, %cs:start_sys_seg

	movw	$2048, %di
	subw	%si, %si
	movw	%cs, %ax	
	movw	%ax, %es
	movw	$SYSSEG, %ax
	movw	%ax, %ds
	rep
	movsw
	movw	%cs, %ax	
	movw	%ax, %ds
	cmpw	$SIG1, setup_sig1
	jne	no_sig

	cmpw	$SIG2, setup_sig2
	jne	no_sig

	jmp	good_sig

no_sig:
	lea	no_sig_mess, %si
	call	prtstr

no_sig_loop:
	jmp	no_sig_loop

good_sig:
	movw	%cs, %ax
	subw	$DELTA_INITSEG, %ax
	movw	%ax, %ds

	testb	$LOADED_HIGH, %cs:loadflags
	jz	loader_ok			

	cmpb	$0, %cs:type_of_loader 		
						
	jnz	loader_ok			

	pushw	%cs				
	popw	%ds				
	lea	loader_panic_mess, %si
	call	prtstr

	jmp	no_sig_loop

loader_panic_mess: .string "Wrong loader, giving up..."

loader_ok:


	xorl	%eax, %eax
	movl	%eax, (0x1e0)

	movb	$0x88, %ah
	int	$0x15
	movw	%ax, (2)

	movw	$0x0305, %ax
	xorw	%bx, %bx
	int	$0x16

# Get hd0 data...
	xorw	%ax, %ax
	movw	%ax, %ds
	ldsw	(4 * 0x41), %si
	movw	%cs, %ax			# aka SETUPSEG
	subw	$DELTA_INITSEG, %ax		# aka INITSEG
	pushw	%ax
	movw	%ax, %es
	movw	$0x0080, %di
	movw	$0x10, %cx
	pushw	%cx
	cld
	rep
 	movsb
# Get hd1 data...
	xorw	%ax, %ax
	movw	%ax, %ds
	ldsw	(4 * 0x46), %si
	popw	%cx
	popw	%es
	movw	$0x0090, %di
	rep
	movsb
# Check that there IS a hd1 :-)
	movw	$0x01500, %ax
	movb	$0x81, %dl
	int	$0x13
	jc	no_disk1
	
	cmpb	$3, %ah
	je	is_disk1

no_disk1:
	movw	%cs, %ax			# aka SETUPSEG
	subw	$DELTA_INITSEG, %ax 		# aka INITSEG
	movw	%ax, %es
	movw	$0x0090, %di
	movw	$0x10, %cx
	xorw	%ax, %ax
	cld
	rep
	stosb
is_disk1:

	movw	%cs, %ax			# aka SETUPSEG
	subw	$DELTA_INITSEG, %ax		# aka INITSEG
	movw	%ax, %ds
	xorw	%ax, %ax
	movw	%ax, (0xa0)			# set table length to 0
	movb	$0xc0, %ah
	stc
	int	$0x15
	jc	no_mca

	pushw	%ds
	movw	%es, %ax
	movw	%ax, %ds
	movw	%cs, %ax			# aka SETUPSEG
	subw	$DELTA_INITSEG, %ax		# aka INITSEG
	movw	%ax, %es
	movw	%bx, %si
	movw	$0xa0, %di
	movw	(%si), %cx
	addw	$2, %cx
	cmpw	$0x10, %cx
	jc	sysdesc_ok

	movw	$0x10, %cx
sysdesc_ok:
	rep
	movsb
	popw	%ds
no_mca:
	
	movw	%cs, %ax
	subw	$DELTA_INITSEG, %ax
	movw	%ax, %ds
	movw	$0, (0x1ff)		
	int	$0x11			
	testb	$0x04, %al		
	jz	no_psmouse

	movw	$0xAA, (0x1ff)		
no_psmouse:

	cmpw	$0, %cs:realmode_swtch
	jz	rmodeswtch_normal

	lcall	%cs:realmode_swtch

	jmp	rmodeswtch_end

rmodeswtch_normal:
        pushw	%cs
	call	default_switch

rmodeswtch_end:

	movl	%cs:code32_start, %eax
	movl	%eax, %cs:code32

	testb	$LOADED_HIGH, %cs:loadflags
	jz	do_move0			# .. then we have a normal low

	jmp	end_move			# ... and we skip moving

do_move0:
	movw	$0x100, %ax			# start of destination segment
	movw	%cs, %bp			# aka SETUPSEG
	subw	$DELTA_INITSEG, %bp		# aka INITSEG
	movw	%cs:start_sys_seg, %bx		# start of source segment
	cld
do_move:
	movw	%ax, %es			# destination segment
	incb	%ah				# instead of add ax,#0x100
	movw	%bx, %ds			# source segment
	addw	$0x100, %bx
	subw	%di, %di
	subw	%si, %si
	movw 	$0x800, %cx
	rep
	movsw
	cmpw	%bp, %bx
	
	jb	do_move

end_move:
	movw	%cs, %ax			# aka SETUPSEG
	movw	%ax, %ds

	movw	%cs, %ax
	cmpw	$SETUPSEG, %ax
	je	end_move_self

	cli		
		
	subw	$DELTA_INITSEG, %ax
	movw	%ss, %dx
	cmpw	%ax, %dx
	jb	move_self_1

	addw	$INITSEG, %dx
	subw	%ax, %dx
			
move_self_1:
	movw	%ax, %ds
	movw	$INITSEG, %ax			# real INITSEG
	movw	%ax, %es
	movw	%cs:setup_move_size, %cx
	std
		
	movw	%cx, %di
	decw	%di
	movw	%di, %si
	subw	$move_self_here+0x200, %cx
	rep
	movsb
	ljmp	$SETUPSEG, $move_self_here

move_self_here:
	movw	$move_self_here+0x200, %cx
	rep
	movsb
	movw	$SETUPSEG, %ax
	movw	%ax, %ds
	movw	%dx, %ss
end_move_self:					# now we are at the right place
	lidt	idt_48				# load idt with 0,0
	lgdt	gdt_48				# load gdt with whatever is
						# appropriate

	call	empty_8042

	movb	$0xD1, %al			# command write
	outb	%al, $0x64
	call	empty_8042

	movb	$0xDF, %al			# A20 on
	outb	%al, $0x60
	call	empty_8042

	push    %ds
	push    %es
	xorw	%ax, %ax			# segment 0x0000
	movw	%ax, %fs
	decw	%ax				# segment 0xffff (HMA)
	movw	%ax, %gs
a20_wait:
	incw	%ax
	movw	%ax, %fs:(0x200)
	cmpw	%gs:(0x210), %ax
	je	a20_wait	

	xorw	%ax, %ax
	outb	%al, $0xf0
	call	delay

	outb	%al, $0xf1
	call	delay


	movb	$0xFF, %al
	outb	%al, $0xA1
	call	delay
	
	movb	$0xFB, %al
	outb	%al, $0x21
#
# Move to protected mode
#

	movw	$1, %ax				# protected mode (PE) bit
	lmsw	%ax				# This is it!
	jmp	flush_instr

flush_instr:
	xorw	%bx, %bx			# Flag to indicate a boot

# ++ save real-mode pointer ++
	xorl    %esi, %esi
	movw    %cs, %si
	subw    $DELTA_INITSEG, %si
	shll    $4, %esi
# ++ end save real-mode pointer ++

	.byte 0x66, 0xea			# prefix + jmpi-opcode
code32:	.long	0x1000				# will be set to 0x100000
						# for big kernels
	.word	0x10

# Here's a bunch of information about your current kernel..
kernel_version:	.ascii	"0.0.0"
		.ascii	" ("
		.ascii	"bigbinc"
		.ascii	"@"
		.ascii	"gcc-3.2"
		.ascii	") "
		.ascii	"0.0.0"
		.byte	0

kernel_message:	.ascii "(RM-MSG-Go!)"
		.byte 	0

default_switch:
	cli					# no interrupts allowed !
	movb	$0x80, %al			# disable NMI for bootup
						# sequence
	outb	%al, $0x70
	lret

bootsect_helper:
	cmpw	$0, %cs:bootsect_es
	jnz	bootsect_second

	movb	$0x20, %cs:type_of_loader
	movw	%es, %ax
	shrw	$4, %ax
	movb	%ah, %cs:bootsect_src_base+2
	movw	%es, %ax
	movw	%ax, %cs:bootsect_es
	subw	$SYSSEG, %ax
	lret					# nothing else to do for now

bootsect_second:
	pushw	%cx
	pushw	%si
	pushw	%bx
	testw	%bx, %bx
	jne	bootsect_ex

	movw	$0x8000, %cx
	pushw	%cs
	popw	%es
	movw	$bootsect_gdt, %si
	movw	$0x8700, %ax
	int	$0x15
	jc	bootsect_panic

	movw	%cs:bootsect_es, %es		# we reset %es to always point
	incb	%cs:bootsect_dst_base+2		# to 0x10000
bootsect_ex:
	movb	%cs:bootsect_dst_base+2, %ah
	shlb	$4, %ah				# we now have the number of
						# moved frames in %ax
	xorb	%al, %al
	popw	%bx
	popw	%si
	popw	%cx
	lret

bootsect_gdt:
	.word	0, 0, 0, 0
	.word	0, 0, 0, 0

bootsect_src:
	.word	0xffff

bootsect_src_base:
	.byte	0x00, 0x00, 0x01		# base = 0x010000
	.byte	0x93				# typbyte
	.word	0				# limit16,base24 =0

bootsect_dst:
	.word	0xffff

bootsect_dst_base:
	.byte	0x00, 0x00, 0x10
	.byte	0x93		
	.word	0		
	.word	0, 0, 0, 0	
	.word	0, 0, 0, 0	

bootsect_es:
	.word	0

bootsect_panic:
	pushw	%cs
	popw	%ds
	cld
	leaw	bootsect_panic_mess, %si
	call	prtstr
	
bootsect_panic_loop:
	jmp	bootsect_panic_loop

bootsect_panic_mess:
	.string	"INT15 refuses to access high mem, giving up."

empty_8042:
	pushl	%ecx
	movl	$0x00FFFFFF, %ecx

empty_8042_loop:
	decl	%ecx
	jz	empty_8042_end_loop

	call	delay

	inb	$0x64, %al
	testb	$1, %al
	jz	no_output

	call	delay
	inb	$0x60, %al
	jmp	empty_8042_loop

no_output:
	testb	$2, %al
	jnz	empty_8042_loop
empty_8042_end_loop:
	popl	%ecx
	ret

# Read the cmos clock. Return the seconds in al
gettime:
	pushw	%cx
	movb	$0x02, %ah
	int	$0x1a
	movb	%dh, %al
	andb	$0x0f, %al
	movb	%dh, %ah
	movb	$0x04, %cl
	shrb	%cl, %ah
	aad
	popw	%cx
	ret

delay:
	jmp	.+2
	ret

# Descriptor tables
gdt:
	.word	0, 0, 0, 0
	.word	0, 0, 0, 0

	.word	0xFFFF				# 4Gb - (0x100000*0x1000 = 4Gb)
	.word	0
	.word	0x9A00
	.word	0x00CF				# granularity = 4096, 386

	.word	0xFFFF				# 4Gb - (0x100000*0x1000 = 4Gb)
	.word	0
	.word	0x9200
	.word	0x00CF
	
idt_48:
	.word	0				# idt limit = 0
	.word	0, 0				# idt base = 0L
gdt_48:
	.word	0x8000				# gdt limit=2048,
						#  256 GDT entries

	.word	512+gdt, 0x9

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
