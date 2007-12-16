###
### Berlin Brown
### $Id: Makefile,v 1.2 2005/05/21 06:08:10 bigbinc Exp $
###
### Makefile for 
###   C - tools
###   Note: external to the OS kernel

CC = gcc
CFLAGS = -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

all: kernel_stats

# -- [ define the objects to compile ] --
OBJS = kernel_stats.o

# -- [ suffixes ]  --
.c.o:
	@echo " [ compiling Standard C file ] "
	$(CC) $(CFLAGS) -c $<

kernel_stats: $(OBJS)
	@echo " [ Compiling project ]"
	$(CC) $(CFLAGS) -o $@ $<

clean:	
	rm -f *~ *.o
	rm -f build
	rm -f kernel_stats 

