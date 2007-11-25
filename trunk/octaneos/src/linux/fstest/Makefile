#
# Berlin Brown
#
# Makefile for fs test

CC = gcc
CFLAGS = -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

all: runfs

# // define the objects to compile
OBJS = fs.o

# // suffixes
.c.o:
	@echo " [ compiling Standard C file ] "
	$(CC) $(CFLAGS) -c $<

runfs: $(OBJS)
	@echo " [ Compiling project ]"
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o
	rm -f runfs
