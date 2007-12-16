#################################################
### ReadMe.txt document
### Copyright (C) 2003, 2007 Berlin Brown
###
### $Id: README.txt,v 1.6 2005/05/15 15:55:54 bigbinc Exp $
###
### THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
### "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
### LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
### A PARTICULAR PURPOSE ARE DISCLAIMED.
###
### See LICENSE.OCTANE for more details
#################################################

=================================================
 * Installing/Build
=================================================
	
	Make sure to install the proper packages.  This will include
	a recent version of Bochs, Grub, Python, Possibly a recent Java JDK
	
	1. Type 'make clean' to get rid of any garbage files
	
	2. Type 'make' to build the operating system kernel
	
	3. Enter the toolkit directory and go over the "Run" script.  This will
	    create the diskimage and other misc files.  Grub and Bochs must be installed
	    for this script to run properly.
	    
	...Enter the toolkit directory and see the README.txt, This is verbatim from that
	particular file:
	
	These are HelperKernel scripts that create an environment
	to test linux/unix kernels over 'bochs'.  I normally
	test floppy kernels, but you probably could test
	cd-rom kernels or hard-drive kernels.
	
	This code is based orange-linux
	http://orangelin.sourceforge.net/
	
	Quick-Start:
	
		YOU must have 'ant' and 'java' for this program to run.
		YOU must install 'ant' and 'java' for this to run
	
		1. type 'ant run'
	
		2. make sure vmlinuz is in the root directory
	
		3. type ./Run (must be a root user)
	
		4. type 'cd bochs'
	
		5. type 'bochs'
	
		6. wait while your floppy OS runs.
	
		7.  that is all....
	
		8. (optional) Run ./WriteDisk to go to /dev/fd0
		
	Note:  you should be able to plug and play an
	grub version you want.  I have tried both 0.93 and 0.94
	that is year(2004) so by the time you read this
	they will be on version 12.0.0 and hopefully grub
	will work.
	
	Note: YOU MUST change your .bochsrc file the bochs directory.
	change the line where it asks for your BIOS and change that
	to your bochs root.
	
	Note: 'ant run' the java component only has to be run once.
	It's only function is to create the Run file and some other 
	directory creation and setup.  Just run it once.  Run ./Run
	whenever you need to update your kernel file for example
	vmlinuz.  When vmlinuz changes, run ./Run.  Clean
	is a different story.  I dont have a position on Clean.  Be
	careful with it though.
		
	
	Typical Steps: Assume root is 'Spirit'
	(assume that ant run has already been run)
	
	1. make clean
	
	2. make
	
	3. make (yes I typed it twice)
	
	4. make bochs (yea I typed it again)
	
	5. cd java
	
	6. ./Run( Error: invalid permissions, I mess up on this too!)
	
	7. su
	
	8. ./Run
	
	9. cd bochs
	
	10. bochs
	
	11. watch your operating system come to life.
	
	create a hard-drive wth bximage
	Note: you may also want to add this to your .bochsrc file
	ata0-master: type=disk, path="c.img", mode=flat, cylinders=20, heads=16, spt=63
	it adds a hard-drive
	
=================================================
 * Thanks
=================================================
	- Operating System Design and Implementation
		
	- Linux kernel developers - Especially Linus Torvalds.
		Unless noted; all linux kernel source is copyrighted to Linus Torvalds (1991-1994) under
		a GPL license.  OctaneOS is also distributed under a GPLv3 license, all source included.
	
	- Tim Robinson - His code has also help this project.
	
	- Alexei A. Frounze - same here, his code
	       is out there, good stuff.
	
	3/26/2004 - help from geist of newos.com
	geist@newos.com
	
=================================================
 * License
=================================================

	Code is provided under a GPLv3 license
	http://www.gnu.org/licenses/gpl.txt
	
	Copyright (c) 2007, Berlin Brown (Newspiritcompany.com)
=================================================
 * Contact
=================================================
	
	You can find me on IRC
	'ramza3' at irc.freenode.net on #java and #osdev
	and of course by email: berlin.brown@gmail.com

=================================================
 * Log
=================================================

4/5/2004:

I hope to add readme descriptions in all directories.

