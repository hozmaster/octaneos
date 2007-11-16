#
# Berlin Brown
#  bigbinc@hotmail.com
#
# $Id: Makefile,v 1.19 2005/05/26 00:06:53 bigbinc Exp $
#
#----------------------------------------------------------

LDFLAGS = -m elf_i386


.S.s:
	$(CPP) -traditional $< -o $*.s
.c.s:
	$(CC) $(CFLAGS) -S $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	@echo " [ compiling Standard C file and gcc asm code ] "
	$(CC) $(CFLAGS) -S $< -o asm/$@.s
	$(CC) $(CFLAGS) -c $<


OBJS =	empty.o misc_print.o traps.o 		\
	exceptions.o interrupts.o    		\
	bad_interrupts.o hardware_interrupts.o	\
	lowlevel.o keyboard.o scheduler.o	\
	floppy.o time_helpers.o dma_functions.o \
	system_calls.o block_devices.o		\
	fs_super.o fs_buffer.o hexdump.o	\
	newfloppy.o

all: kernel.o

kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o kernel.o $(OBJS)
	sync

dummy:

#
## include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif


#
# ++ Make Dependencies
#  - the modern kernel uses Rules.make, we will do copy-paste
#
fastdep: dummy
	$(TOPDIR)/scripts/mkdep $(CFLAGS) $(EXTRA_CFLAGS) -- $(wildcard *.[chS]) > .depend

# 
#  End of make dependencies
#
clean:
	rm -f asm/*.s
	rm -f *~
	rm -f *.o
	rm -f .depend
