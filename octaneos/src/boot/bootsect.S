/*
 * Berlin Brown
 * bigbinc@hotmail.com
 * 
 * $Id: bootsect.S,v 1.2 2005/05/21 05:36:18 bigbinc Exp $
 *
 * Created by Linus Torvalds and crew
 */


SETUPSECTS	= 4
BOOTSEG		= 0x07C0
INITSEG		= 0x9000 
SETUPSEG	= 0x9020
SYSSEG		= 0x1000 
SYSSIZE		= 0x7F00

ROOT_DEV	= 0
SWAP_DEV	= 0

#define SVGA_MODE 0xffff 
#define RAMDISK 0
#define ROOT_RDONLY 1

.code16
.text

.global _start
_start:

	movw	$BOOTSEG, %ax
	movw	%ax, %ds
	movw	$INITSEG, %ax
	movw	%ax, %es
	movw	$256, %cx
	subw	%si, %si
	subw	%di, %di
	cld
	rep
	movsw
	ljmp	$INITSEG, $go


go:	movw	$0x4000-12, %di
	movw	%ax, %ds
	movw	%ax, %ss
	movw	%di, %sp

# == Phase 1 ====

	movw	%cx, %fs
	movw	$0x78, %bx
	pushw	%ds
	ldsw	%fs:(%bx), %si
	movb	$6, %cl
	pushw	%di
	rep
	movsw
	popw	%di
	popw	%ds
	movb	$36, 0x4(%di)
	movw	%di, %fs:(%bx)
	movw	%es, %fs:2(%bx)



	movw	$disksizes, %si
probe_loop:
	lodsb
	cbtw
	movw	%ax, sectors
	cmpw	$disksizes+4, %si
	jae	got_sectors

	xchgw	%cx, %ax
	xorw	%dx, %dx
	movw	$0x0200, %bx
	movw	$0x0201, %ax
	int	$0x13
	jc	probe_loop

got_sectors:
	movb	$0x03, %ah
	xorb	%bh, %bh
	int	$0x10
	movw	$9, %cx
	movb	$0x07, %bl
				
	movw	$msg1, %bp
	movw	$0x1301, %ax
	int	$0x10


	movw	$0x0001, %ax
	movw	$sread, %si
	movw	%ax, (%si)

	xorw	%ax, %ax		# reset FDC
	xorb	%dl, %dl
	int	$0x13
	movw	$0x0200, %bx		# address = 512, in INITSEG
next_step:
	movb	setup_sects, %al
	movw	sectors, %cx
	subw	(%si), %cx		# (%si) = sread
	cmpb	%cl, %al
	jbe	no_cyl_crossing
	movw	sectors, %ax
	subw	(%si), %ax		# (%si) = sread
no_cyl_crossing:
	call	read_track
	pushw	%ax			# save it
	call	set_next
	popw	%ax			# restore
	subb	%al, setup_sects	# rest - for next step
	jnz	next_step

	pushw	$SYSSEG
	popw	%es			# %es = SYSSEG
	call	read_it
	call	kill_motor
	call	print_nl



	movw	root_dev, %ax
	orw	%ax, %ax
	jne	root_defined

	movw	sectors, %bx
	movw	$0x0208, %ax		# /dev/ps0 - 1.2Mb
	cmpw	$15, %bx
	je	root_defined

	movb	$0x1c, %al		# /dev/PS0 - 1.44Mb
	cmpw	$18, %bx
	je	root_defined

	movb	$0x20, %al		# /dev/fd0H2880 - 2.88Mb
	cmpw	$36, %bx
	je	root_defined

	movb	$0, %al			# /dev/fd0 - autodetect
root_defined:
	movw	%ax, root_dev


	ljmp	$SETUPSEG, $0


sread:	.word 0				# sectors read of current track
head:	.word 0				# current head
track:	.word 0				# current track



read_it:
	movw	%es, %ax		# %es = SYSSEG when called
	testw	$0x0fff, %ax
die:	jne	die			# %es must be at 64kB boundary
	xorw	%bx, %bx		# %bx is starting address within segment
rp_read:
	movw	%es, %ax
	subw	$SYSSEG, %ax
	movw	%bx, %cx
	shr	$4, %cx
	add	%cx, %ax		# check offset
	cmpw	syssize, %ax
	jbe	ok1_read

	ret

ok1_read:
	movw	sectors, %ax
	subw	(%si), %ax
	movw	%ax, %cx
	shlw	$9, %cx
	addw	%bx, %cx
	jnc	ok2_read

	je	ok2_read

	xorw	%ax, %ax
	subw	%bx, %ax
	shrw	$9, %ax
ok2_read:
	call	read_track
	call	set_next
	jmp	rp_read

read_track:
	pusha
	pusha	
	movw	$0xe2e, %ax
	movw	$7, %bx
 	int	$0x10
	popa		


	movw	4(%si), %dx
	movw	(%si), %cx
	incw	%cx
	movb	%dl, %ch
	movw	2(%si), %dx
	movb	%dl, %dh
	andw	$0x0100, %dx
	movb	$2, %ah
	pushw	%dx
	pushw	%cx
	pushw	%bx
	pushw	%ax
	int	$0x13
	jc	bad_rt

	addw	$8, %sp
	popa
	ret

set_next:
	movw	%ax, %cx
	addw	(%si), %ax		# (%si) = sread
	cmp	sectors, %ax
	jne	ok3_set
	movw	$0x0001, %ax
	xorw	%ax, 2(%si)		# change head
	jne	ok4_set
	incw	4(%si)			# next track
ok4_set:
	xorw	%ax, %ax
ok3_set:
	movw	%ax, (%si)		# set sread
	shlw	$9, %cx
	addw	%cx, %bx
	jnc	set_next_fin
	movw	%es, %ax
	addb	$0x10, %ah
	movw	%ax, %es
	xorw	%bx, %bx
set_next_fin:
	ret

bad_rt:
	pushw	%ax			# save error code
	call	print_all		# %ah = error, %al = read
	xorb	%ah, %ah
	xorb	%dl, %dl
	int	$0x13
	addw	$10, %sp
	popa
	jmp read_track

 
print_all:
	movw	$5, %cx
	movw	%sp, %bp
print_loop:
	pushw	%cx
	call	print_nl
	cmpb	$5, %cl
	jae	no_reg
	
	movw	$0xe05 + 'A' - 1, %ax
	subb	%cl, %al
	int	$0x10
	movb	$'X', %al
	int	$0x10
	movb	$':', %al
	int	$0x10
no_reg:
	addw	$2, %bp
	call	print_hex
	popw	%cx
	loop	print_loop
	ret

print_nl:
	movw	$0xe0d, %ax		# CR
	int	$0x10
	movb	$0xa, %al		# LF
	int 	$0x10
	ret



print_hex:
	movw	$4, %cx
	movw	(%bp), %dx
print_digit:
	rolw	$4, %dx
	movw	$0xe0f, %ax
	andb	%dl, %al
	addb	$0x90, %al
	daa
	adc	$0x40, %al
	daa
	int	$0x10
	loop	print_digit
	ret



kill_motor:
#if 1
	xorw	%ax, %ax
	xorb	%dl, %dl
	int	$0x13
#else
	movw	$0x3f2, %dx
	xorb	%al, %al
	outb	%al, %dx
#endif
	ret

sectors:	.word 0
disksizes:	.byte 36, 18, 15, 9
msg1:		.byte 13, 10
		.ascii "Loading"

# == Done ==

.org 497
setup_sects:	.byte SETUPSECTS
root_flags:	.word ROOT_RDONLY
syssize:	.word SYSSIZE
swap_dev:	.word SWAP_DEV
ram_size:	.word RAMDISK
vid_mode:	.word SVGA_MODE
root_dev:	.word ROOT_DEV
boot_flag:	.word 0xAA55