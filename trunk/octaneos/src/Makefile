###**********************************************************
### Root Makefile for ProjectAkita Operating System
###
### Copyright (c) 2005-2007 - Berlin Brown
###
### berlin.brown@gmail.com
###  
###   Version: 1.0.0
###   
###   First System Tested: Mandrake Linux 9.1 3.2.2-3mdk (2005)
###
###**********************************************************
### 
###  Typically a build increment is when I want to make a snapshot
###   - a minor release will note a lot of changes but not major
###   - a major release will be a year or 6 month milestone
###
###**********************************************************
MAJOR_VER_NO=0
MINOR_VER_NO=1
BUILD_LEVEL=18

VERS=$(MAJOR_VER_NO).$(MINOR_VER_NO).$(BUILD_LEVEL)

TOPDIR		= $(shell /bin/pwd)

AS		= as
CC		= gcc
CPP		= $(CC) -E
LD		= ld
LD386		= ld -m elf_i386
OBJCOPY		= objcopy

HPATH           = $(TOPDIR)/include
PYTHON		= python

CPPFLAGS = -D__KERNEL__ -I$(HPATH)
DEFAULTCFLAGS      = -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

OFLAGS          = -O binary -R .note -R .comment -S

CFLAGS = $(CPPFLAGS) -Wall -Wstrict-prototypes -Wno-trigraphs -O2  \
			-fomit-frame-pointer -fno-strict-aliasing -fno-common    \
			-fno-stack-protector

HPATH 		= $(TOPDIR)/include

export 	HPATH DEFAULTCFLAGS TOPDIR AS CPP LD LD386 OBJCOPY CC OFLAGS

AFLAGS 		= -D__ASSEMBLY__ $(CPPFLAGS)

export  CPPFLAGS CFLAGS CFLAGS_KERNEL AFLAGS

# ld script
LD386FLAGS	= -Ttext 0x100000


###
### Phony targets, watch out for the real file names
###
.PHONY: boot kernel linux init zBoot bootsect backup

###
### Define compile start(all)
###
all: vmlinux

###
### Should I worry about dependencies?
### 
ifeq (.depend,$(wildcard .depend))
include .depend
else
CONFIGURATION = depend
endif


ifdef CONFIGURATION
CONFIGURE = dummy
endif

###
### What .C files and objects do I compile, how about the kernel? and drivers?
### (see preZSystem)
###
CORE_FILES = kernel/kernel.o

SUBDIRS = kernel

bochs: vmlinux
	@echo
	@echo 
	@echo "## Copying file to toolkit directory"
	@echo "## now: 'cd toolkit' and type 'ant 'run'"
	@echo "##         OR just type ./Run"
	@echo
	cp -v vmlinux $(TOPDIR)/toolkit/vmlinuz

###
### ===== Misc scripts and programs
###
tools/build: tools/build.c $(CONFIGURE)
	$(CC) $(DEFAULTCFLAGS) -o $@ $<

tools/kernel_stats: tools/kernel_stats.c
	$(MAKE) -C tools all

scripts/mkdep: scripts/mkdep.c
	$(CC) $(DEFAULTCFLAGS) -o $@ $<

###*************************************
### End of Tools Compile
###*************************************

### Boot sector setup
### You should only receive one error message
###   about lcall without
boot/bootsect: boot/bootsect.o
	$(LD) -Ttext 0x0 -s --oformat binary $< -o $@

boot/bootsect.o: boot/bootsect.s
	$(AS) -o $@ $<

boot/bootsect.s: boot/bootsect.S Makefile
	$(CPP) $(CPPFLAGS) -traditional $< -o $@

# -- Setup --

boot/setup: boot/setup.o
	$(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $@ $<

boot/setup.o: boot/setup.s
	$(AS) -o $@ $<

boot/setup.s: boot/setup.S Makefile
	$(CPP) $(CPPFLAGS) -D__ASSEMBLY__ -traditional $< -o $@

boot/mini: boot/mini.o
	        $(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $@ $<

boot/mini.o: boot/mini.s
	        $(AS) -o $@ $<

boot/mini.s: boot/mini.S Makefile
	        $(CPP) $(CPPFLAGS) -D__ASSEMBLY__ -traditional $< -o $@

### Head and Misc
### 
### zImage or vmlinux is the final target
###  this is pretty close - must have a kernel built of course
###  
### 	once preZSystem gets out of the oven
### 	we can compile this section below
###
zBoot/zSystem: $(CONFIGURE) preZSystem
	@echo
	@echo "** Building compressed(gzip) kernel tools..."
	@echo "** just created piggy"
	@echo "** Entering zBoot directory...please standby"
	@echo
	$(MAKE) -C zBoot

kernel/kernelhead.o: kernel/kernelhead.S
	@echo
	@echo "** Compiling kernel-head.S"
	$(CC) $(AFLAGS) -traditional -c $< -o $*.o

### Note, difference from above 
### translates to (linux.1.0)tools/zSystem
### 
###  Note: the new tag to LD386
###   and also the script for preZSystem.lds
###	it is a ld script
###
### - the script contains __start for example
###
preZSystem: kernel/kernelhead.o init/main.o init/buildct.o init/getversion.o linuxsubdirs
	@echo
	@echo "==## Building Drivers ##=="
	@echo "** building drivers and kernel..."
	@echo "**"
	@echo
	$(LD386) $(LD386FLAGS) -e stext kernel/kernelhead.o \
			init/main.o       \
			init/buildct.o    \
			init/getversion.o \
		--start-group 	\
		$(CORE_FILES)	\
		--end-group	\
		-o preZSystem
	nm preZSystem | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | sort > preZSystem.map

### 
### End of bootsector setup
### 

### Suffixes: not too important
###
### an object like kernel/head.S may use a suffix like below
###   note the defined __ASSEMBLY__
###
.S.s:
	$(CPP) $(AFLAGS) $(AFLAGS_KERNEL) -traditional -o $*.s $<
.S.o:
	$(CC) $(AFLAGS) $(AFLAGS_KERNEL) -traditional -c -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $<

init/main.o: $(CONFIGURE) init/main.c
	$(CC) $(CFLAGS) -c -o $*.o $< -I$(TOPDIR)/include

init/buildct.o: $(CONFIGURE) init/buildct.c
	$(CC) $(CFLAGS) -c -o $*.o $< -I$(TOPDIR)/include

###
### head.o and init_task.o object files contain data that is 
###  needed when the kernel launches, your basic gdt idt stuff
###
### Note: head.o generated from head.S
report:
	$(TOPDIR)/scripts/count_code
	$(TOPDIR)/scripts/count_pure
	python $(TOPDIR)/scripts/find_count.py

backup:
	cd .. && tar cf - Orange | gzip > Orange-$(VERS).tar.gz 

# -- [ note you need java for this arg ] --
cvs:
	cd .. && tar cf - Orange | gzip > OrangeCVS-$(VERS).tar.gz 
	$(TOPDIR)/scripts/snapshot.sh OrangeCVS-$(VERS)

###
### Complicated stuff to find which subdirs to enter into and compile
### - see CORE_FILES for example at the top of this file
linuxsubdirs: $(patsubst %, _dir_%, $(SUBDIRS))

$(patsubst %, _dir_%, $(SUBDIRS)) : dummy
	        $(MAKE) CFLAGS="$(CFLAGS) $(CFLAGS_KERNEL)" -C $(patsubst _dir_%, %, $@)

### Dependency setup
dep-files: scripts/mkdep
	@echo 
	@echo "** Running target dep-files... +++"
	@echo
	$(TOPDIR)/scripts/mkdep -- init/*.c > .depend
	$(MAKE) $(patsubst %,_sfdep_%,$(SUBDIRS)) _FASTDEP_ALL_SUB_DIRS="$(SUBDIRS)"

###
### Main Binary Object: zImage(vmlinux)
###
vmlinux: $(CONFIGURE) boot/bootsect boot/setup scripts/mkdep zBoot/zSystem tools/build tools/kernel_stats 
	@echo
	@echo "==** Running **==========================="
	@echo "** Compiling Kernel......."
	@echo "** Target: all to : vmlinux"
	@echo
	$(OBJCOPY) $(OFLAGS) zBoot/zSystem zBoot/zSystem.out
	tools/build boot/bootsect boot/setup zBoot/zSystem.out $(ROOT_DEV) > vmlinux
	tools/kernel_stats vmlinux
	sync
	$(PYTHON) $(TOPDIR)/scripts/get_build.py
	$(PYTHON) $(TOPDIR)/scripts/print_version.py $(MAJOR_VER_NO) $(MINOR_VER_NO) $(BUILD_LEVEL)


depend dep: dep-files

$(patsubst %,_sfdep_%,$(_FASTDEP_ALL_SUB_DIRS)):
	$(MAKE) -C $(patsubst _sfdep_%,%,$@) fastdep


###
### Check on depend or configuration file
### (also run the check app script)
ifdef CONFIGURATION
..$(CONFIGURATION):
	@echo 
	@echo "====== Current Make options ================"
	@echo " 'make' and see below"
	@echo " | backup | - create a zipped version of source"
	@echo " | cvs    | - create a zippped cvs snapshot"
	@echo " | bochs  | - setup a kernel for bochs"
	@echo " | clean  | - start from scratch and remove garbage" 
	@echo " the best option is to type 'make' twice"
	@echo "============================================"
	$(TOPDIR)/scripts/check_apps
	@echo
	@echo "** You have a bad " .$(CONFIGURATION) "===="
	@echo "** You probably should have done [make $(CONFIGURATION)] first"
	@echo "** It is ok..."
	@echo
	$(MAKE) $(CONFIGURATION)
	@echo
	@echo "** [WARNING] You will need to re-type 'make' or you can use"
	@echo "** [WARNING]  'make bochs' to build/deploy the kernel"
	@echo
	exit 1

dummy: ..$(CONFIGURATION)

else

dummy:

endif

# Clean | remove all garbage
SUBDIRS_CLEAN = kernel boot include tools scripts include init zBoot toolkit \
		toolkit/java
clean:
	@echo "========================================="
	@echo " Cleaning Project"
	@echo " $(SUBDIRS_CLEAN)"
	@echo "========================================="
	@for i in $(SUBDIRS_CLEAN) ; do \
		echo "Entering SUBDIR for clean: $$i"; \
		$(MAKE) -C $$i clean ; \
	done ;
	rm -f *~ *.o
	rm -f .depend
	rm -f preZSystem preZSystem.map
	rm -f docs/*~
	rm -f vmlinux SrcReport.txt PureReport.txt TotalSrc.txt
	$(TOPDIR)/scripts/rm_squid
	@echo "== cleaning complete =="

###**********************************************************
### End of Makefile
###**********************************************************

