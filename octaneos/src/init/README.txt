++
++
++
++ This directory contains main.c
++
++ $Id: README.txt,v 1.1 2004/04/05 12:40:17 bigbinc Exp $
++ 

main.c for lack of a better file name gets the ball rolling
in terms of kernel initialization.

start_kernel is the first thing that gets called.

from this function, traps get setup, the scheduler gets setup.

the two other files contain variables for the version
and the build count.
