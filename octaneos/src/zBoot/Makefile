#
##
# Berlin Brown
#  bigbinc@hotmail.com
#
# $Id: Makefile,v 1.2 2005/05/21 06:08:10 bigbinc Exp $
#
#  Make for gzipped style objects
# are we having fun yet?
# #
#

# Didnt care much for the name head.o
#  same as boot/head.o?
#
HEAD = zboothead.o

SYSTEM = ../preZSystem

#
# I set it up so that the gzip code is all in misc
#  - this code once again is not important to the system
#   	this code gets us to the system
#

zOBJECTS = $(HEAD) misc.o


ZLDFLAGS = -e startup_32
ZIMAGE_OFFSET = 0x100000
ZLINKFLAGS = -m elf_i386 -Ttext $(ZIMAGE_OFFSET) $(ZLDFLAGS)

#
# Completely redundant flags just to be safe
# - of course we hope header path is exported
#
#
CUR_CFLAGS = -D__KERNEL__ -I$(HPATH)
CFLAGS =  $(CUR_CFLAGS) -Wall -Wstrict-prototypes -Wno-trigraphs -O2 \
	  -fomit-frame-pointer -fno-strict-aliasing -fno-common



#
# Suffix, tell me what to do when I see
# these file types
# - in case you didnt know already
#
.c.s:
	$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	@echo "-- (zBoot) Standard C File"
	$(CC) $(CFLAGS) -c -o $*.o $<

#
# Why am I in this makefile?
#  - build a zipped kernel
#
all:	zSystem

zSystem: piggy.o $(zOBJECTS) demo.o
	@echo
	@echo "** GO! zSystem..."
	@echo
	$(LD) $(ZLINKFLAGS) -o zSystem $(zOBJECTS) piggy.o
	nm zSystem | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | sort > zPIGGY.map

#
# I changed the name from the normal head.S
#
zboothead.o: zboothead.S
	$(CC) $(AFLAGS) -traditional -c zboothead.S 


#
# debug objects
#
demo.S:
	$(CPP) $(AFLAGS) $(AFLAGS_KERNEL) -traditional -o $*.s demo.S 

demo.o: demo.s
	$(AS) -o $@ $<

demo.s: demo.S
	$(CPP) $(CPPFLAGS) -D__ASSEMBLY__ -traditional $< -o $@


#
# Looks confusing, but not really, but it is important
#  basically, we are zipping our object file and kernel and drivers
#   
#   The funky part is the SECTIONS blah... all this is finding is the
#   size of our object, that is all, see below 'input_len'
#    ...got milk...?   (is this line copyrighted?)
#    
#
piggy.o: $(SYSTEM)
	@echo "**"
	@echo "** Creating Kernel gzipped piggy-back"
	@echo "**"
	tmppiggy=_tmp_$$$$piggy; \
	rm -f $$tmppiggy $$tmppiggy.gz $$tmppiggy.lnk; \
	$(OBJCOPY) $(OFLAGS) $(SYSTEM) $$tmppiggy; \
	gzip -f -9 < $$tmppiggy > $$tmppiggy.gz; \
	echo "SECTIONS { .data : { input_len = .; LONG(input_data_end - input_data) input_data = .; *(.data) input_data_end = .; }}" > $$tmppiggy.lnk; \
	$(LD) -r -o piggy.o -b binary $$tmppiggy.gz -b elf32-i386 -T $$tmppiggy.lnk; \
	rm -f $$tmppiggy $$tmppiggy.gz $$tmppiggy.lnk


$(SYSTEM):
	@echo
	@echo "** ErrorBot:"
	@echo "** error: I cannot let you compile $(SYSTEM) from here."
	@echo "** Try from the topdir that is much better"
	@echo
	exit 1


#
# Clean - Clean garbage files
#
clean:
	rm -f *.lnk
	rm -f *.s *.o
	rm -f piggyback xtract zboothead.s
	rm -f zSystem _tmp_* zSystem.out *.map