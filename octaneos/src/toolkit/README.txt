#
#
# 4/5/2004
#
# Updated: 5/17/2005
#

Note: this file based on Spirit code so you may
need to ignore the java aspects(I took that out for the time being).

Berlin Brown
bigbinc@hotmail.com <<-- spam email, try
http://www.retroevolution.com - for contact

$Id: README.txt,v 1.3 2005/05/21 06:08:10 bigbinc Exp $

Orange-Linux: 1.0.1

2/16/2004

Disclaimer:

++ Note: These scripts/programs are very dangerous ++

They delete kernels, open up devices, write to device files...
Batch run boot loader programs...

And on top of that, I dont have the documentation to 
layout the danger.  You will have read the scripts and java
programs carefully.  Other than that, you should be fine.
Actually the scripts arent that dangerous.

++ End Disclaimer ++

These are HelperKernel scripts that create an environment
to test linux/unix kernels over 'bochs'.  I normally
test floppy kernels, but you probably could test
cd-rom kernels or hard-drive kernels.

This code is based orange-linux
http://orangelin.sourceforge.net/

Quick-Start:
	
	YOU must have 'ant' and 'java' for this program to run.
	YOU must install 'ant' and 'java' for this to run
	
	** In the parent directory, see the Makefile **
	
	A. Type 'make'
	
	B. You may need to type make again, type 'make bochs'

	C. Enter the toolkit directory
	
	D. You must login using the 'su' user, Enter 'su' and then the password

	** In the toolkit directory **

	1. type 'ant run'

	2. make sure vmlinuz is in the root directory

	3. type ./Run

	4. type 'cd bochs'

	5. type 'bochs'

	6. wait while your floppy OS runs.

	7.  that is all....

	8. oh yea!, you could run ./WriteDisk to go to /dev/fd0


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


== Good/Bad/Ugly:

This is where this system, the toolkit part, you can compile
your operating system and then run your operating system from
bochs.  There aren't simple ways to do this without bochs.  Yea
you could buy a machine and then write your OS to floppy reboot
the system.  That takes time, this system doesnt.

	

