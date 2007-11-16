#
#
#
# Berlin Brown
# bigbinc@hotmail.com
#
#  Makefile for normal gcc applications
#
# $Id: Makefile,v 1.1.1.1 2004/03/08 21:42:22 bigbinc Exp $
#
#

CC              = gcc

CPPFLAGS	= 
DEFAULTCFLAGS	= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

LDFLAGS		=
LIBS		=

CFLAGS = $(CPPFLAGS) -Wall -Wstrict-prototypes -Wno-trigraphs -O2 \
	-fomit-frame-pointer -fno-strict-aliasing -fno-common

OBJS = gonode.o main.o

#
## Goal is xnode
#
all: xnode

#
# Default suffixes
#
.c.o:
	echo " ++ Default C-Files ++"
	$(CC) $(DEFAULTCFLAGS) -c -o $*.o $<


xnode: $(OBJS)
	echo " ++ Creating $@ application"
	$(CC) -o $@ $(DEFAULTCFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

clean:
	rm -f *~
	rm -f xnode
	rm -f *.o



