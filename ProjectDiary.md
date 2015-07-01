# Revived the OrangeLinux/Akita project, now OctaneOS #

12/10/2007

Bluescreen of death, caused by adding memory code at startup.  I have added most of the kernel components, now I have to actually test them and get them working.

![http://octaneos.googlecode.com/svn/trunk/octaneos/docs/media/pics/bluescreen_1.png](http://octaneos.googlecode.com/svn/trunk/octaneos/docs/media/pics/bluescreen_1.png)

11/18/2007

I have been writing small hobby operating systems since 2000 or so.  I took a break and quit around 2003-2007.  I am trying to revive my last project and recreate it under the OctaneOS project.  OctaneOS is an attempt at a hobby operating system that boots a factor image and subsequently boots factor code.

I recently made some code cleanups to the Akita/OrangeLinux project and was able to build and boot the kernel with a modern Linux host operating system (Ubuntu 7.10) shown in the image below:

![http://octaneos.googlecode.com/svn/trunk/octaneos/docs/media/pics/octaneos_1.png](http://octaneos.googlecode.com/svn/trunk/octaneos/docs/media/pics/octaneos_1.png)
**Figure 1**

# Floppy and Hard drive drivers #

I left my last project at the Floppy and Hard drive drivers.  Basically; the floppy driver is broken due to timer interrupt issues and the hard driver is non-existent.  So, I couldn't load applications.  Fixing these components will be a major task and hopefully I will be able to bootstrap Factor.