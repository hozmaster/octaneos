##################################################
##
## Copyright (C) 2003, 2007 Berlin Brown
## $Id: Makefile,v 1.19 2005/05/26 00:06:53 bigbinc Exp $
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
## LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
## A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
## CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
## LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
## NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
##################################################

LDFLAGS = -m elf_i386

.S.s:
	$(CPP) -traditional $< -o $*.s
.c.s:
	$(CC) $(CFLAGS) -S $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	@echo "INFO: compiling Standard C file and gcc asm code ///"
	mkdir -p `dirname asm/$@.s`
	$(CC) $(CFLAGS) -S $< -o asm/$@.s
	$(CC) $(CFLAGS) -c $< -o $@

##################################################
## Kernel File Object Listings
##################################################
OBJS = \
	scheduler.o system_calls_interrupts.o           \
	system_calls.o                                  \
	system_calls_undef.o                            \
	empty.o misc_print.o traps.o                    \
	exceptions.o interrupts.o                       \
	bad_interrupts.o                                \
	lowlevel.o keyboard.o                           \
	floppy.o time_helpers.o dma_functions.o         \
	devices.o fs_super.o hexdump.o		            \
    inode.o fs_file_table.o block_devices.o         \
	exec_domain.o fork.o time.o memory.o            \
	filesystems_sys_setup.o							\
	generic_block.o									\
	fs_buffer.o										\
	tests/traps/test_traps_1.o						\
	tests/time/test_time_1.o						\
	tests/block/test_block_1.o

all: kernel.o

kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o kernel.o $(OBJS)
	sync

dummy:

#
# include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif

#
# Make Dependencies
fastdep: dummy
	$(TOPDIR)/scripts/mkdep $(CFLAGS) $(EXTRA_CFLAGS) -- $(wildcard *.[chS]) > .depend

# 
# End of make dependencies
clean:
	rm -f asm/*.s
	rm -f *~
	rm -f *.o
	rm -f .depend
