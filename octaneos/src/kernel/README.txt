++
++
++
++ Berlin Brown
++
++
++ $Id: README.txt,v 1.1 2004/04/05 12:34:46 bigbinc Exp $

4/5/2004:

This directory contains pretty much the entire OS kernel.
Most of the drivers, filesystem, memory modules will be located
here.


4/5/2004 Status:

This kernel doesnt do a lot.  A major milestone will include a 
floppy driver, the next milestone will include a filesystem.

Current Modules:

	+ grub configuration through the 'toolkit' folder
	
	+ boot code boots to the unzip kernel module

	+ zBoot/misc.c unzips the kernel jumps to that location
		in protected mode

	+ simple text output

	+ simple keyboard scan code debugging

	+ currently debugging the floppy driver

	+ working on simple filesystem
	
