//
// Berlin Brown
// - bigbinc@hotmail.com
//
// Created: 02-01-04
//
// $Id: HelpKernel.java,v 1.1 2004/04/08 18:00:41 bigbinc Exp $
//
// note; this is a redo of the orangelin scripts
//
// - this just creates the proper directories
//
// j2sdk1.4.1_06 - Mandrake Linux
//
// This is java, but this really wont work too well for Windows
// sorry?
//
//
// I recommend installing grub - 0.94 or whatever the latest is
//
//
// I have no idea why I write variables as _xyz_ sorry?
//
//
// Because as an operating systems developer, you have to look
//
// at the whole package 
//

package org.retro.gis;

import java.io.*;
import java.util.*;

import java.io.File;

public class HelpKernel {


    public static String [] _rootDirList = {		
	"bin",
	"boot",
	"dev",
	"etc",
	"lib",
	"mnt",
	"proc",
	"sbin",
	"usr",
	"boot/grub",
	"usr/bin",
	"usr/i386-linux-uclibc",
	"usr/i386-linux-uclibc/lib",
	"etc/init.d",
	"etc/rc.d"
    };

    // $IMGFILE
    public static String [] _copy_from = {	
	"$KERNELFILE",
	"$STAGE1_FILE",
	"$STAGE2_FILE",
	"$GRUB_FILE"
    };

    // $BOCHS_DIR
    public static String [] _copy_to = {
	
	"$MNTDIR",
	"${MNTDIR}/boot/grub/stage1",
	"${MNTDIR}/boot/grub/stage2",
	"${MNTDIR}/boot/grub/menu.lst"
    };

    public static void echo(PrintWriter _out, String _str) throws IOException {


	_out.println("");
	_out.println("echo \"=====================================\"");
	_out.println("echo \"" + _str + "\"");
	_out.println("echo \"=====================================\"");
	_out.println("");

    } // end of the function


    public static void cleanup(PrintWriter _out) throws IOException  {

	_out.println("");
	_out.println("\techo \"(cleanup)Performing cleanup, exiting...\"");
        _out.println("\tumount $MNTDIR");
        _out.println("\tlosetup -d $LOOPDEV");
        _out.println("\texit 1");
	_out.println("");
	_out.flush();

    } // end of the functino

    public static void goRun(PrintWriter _out) throws IOException {

	_out.println("");

	_out.println("mkdir -p mnt");
	_out.println("chown bigbinc mnt");
	_out.println("chgrp bigbinc mnt");
	_out.println("mkdir -p bochs");
	_out.println("chown bigbinc bochs");
	_out.println("chgrp bigbinc bochs");
	
	_out.println("");
	    

	_out.println("# Create the image file");
	_out.println("dd if=/dev/zero of=$IMGFILE bs=1k count=$DISKSIZE");
	_out.println("");

	
	_out.flush();

    } // end of the function

    public static void writeBochs() {
	
	PrintWriter _o = null;
	FileWriter _f = null;

	try {
	    
	    _f = new FileWriter("bochs/.bochsrc");
	    
	    _o = new PrintWriter(_f);

	    _o.println("#");
	    _o.println("#");
	    _o.println("megs: 32");
	    _o.println("romimage: file=/home/bigbinc/bochs/bochs-20040131/bios/BIOS-bochs-latest, address=0xf0000");
	    _o.println("vgaromimage: /home/bigbinc/bochs/bochs-20040131/bios/VGABIOS-elpin-2.40");  
	    _o.println("");
	    _o.println("");
	    _o.println("floppya: 1_44=orange.img, status=inserted");
	    _o.println("floppyb: 1_44=floppyb.img, status=inserted");
	    _o.println("");
	    _o.println("#diskc: file=hd10meg.img, cyl=306, heads=4, spt=17");
	    _o.println("newharddrivesupport: enabled=1");
	    _o.println("");
	    _o.println("boot: a");
	    _o.println("");
	    _o.println("");		      
	    _o.println("log: bochsout.txt");
	    _o.println("");
	    _o.println("mouse: enabled=0");
	    _o.println("");
	    _o.flush();

	    _o.close();

	} catch (IOException e) {
	    
	    e.printStackTrace();
	    
	} // end of the try - catch
	

    } // end of the function

    public static void makecopy(PrintWriter _out) throws IOException { 

	_out.println("");

	echo(_out, "Copying file section [begin]");

	for (int i = 0; i < _copy_from.length; i++) {

	    echo(_out, "Copying..." + _copy_from[i]);
	    
	    _out.println("if ! cp -a " + _copy_from[i] + " " + _copy_to[i] + " ; then");
	    _out.println("\techo \"Error: copy error exiting...\"");
	    _out.println("\tumount $MNTDIR");
	    _out.println("\tlosetup -d $LOOPDEV");
	    _out.println("\texit $?");
	    _out.println("fi");
	    _out.println("");

	} // end of the for 
	_out.flush();
	
    } // end of the functino

    public static void loop(PrintWriter _out) throws IOException {

	echo(_out, "Preparing loop back device...");
	
	_out.println("");
	_out.println("# Prepare the image for loop back device thing");
	_out.println("if ! `losetup $LOOPDEV $IMGFILE` ; then");
        _out.println("\techo \"losetup failed...\"");
        _out.println("\techo \"Please make sure that you are root\"");
        _out.println("\techo \"Also make sure that you have loop device compiled in the kernel.\"");
	_out.println("");
	_out.println("\trm -f $IMGFILE");
	_out.println("\texit 0");
	_out.println("fi");
	_out.println("");	       
	_out.flush();

	echo(_out, "Creating ext2fs...");
	
	_out.println("echo \"Creating ext2 fs...\"");
	_out.println("mke2fs -m 0 $LOOPDEV");
	_out.println("if [ \"$?\" != \"0\" ] ; then");
	_out.println("\techo \"ERROR $? Making EXT2 FS\"");
	_out.println("\texit 1;");
	_out.println("fi");
	_out.println("");

	
	echo(_out, "Mounting the loopdevice....");

	_out.println("mount $LOOPDEV $MNTDIR -t ext2");
	_out.println("if [ \"$?\" != \"0\" ] ; then");
        _out.println("\techo \"ERROR $? mounting loop device.\"");
        _out.println("\tlosetup -d $LOOPDEV");
        _out.println("\texit 1;");
	_out.println("fi");
	_out.println("");

	_out.println("if [ ! -f $KERNELFILE ] ; then");
        _out.println("\techo \"The linux kernel is missing, exiting...\"");
	_out.flush();

	cleanup(_out);

	_out.println("fi");
	_out.println("");

	
	_out.println("# Check for the filesize");
	_out.println("total_size=$(du -c $KERNELFILE | tail -1 | cut -f1);");
	_out.println("");
	_out.println("echo \"TOTAL KERNEL SIZE: $total_size kb\"");
	_out.println("");
	_out.println("if [ $total_size -gt $MAX_SIZE ] ; then");
	_out.println("\techo \"** error: Your kernel is too big!!!!!\"");
	_out.println("\techo \"** You can fix this by compiling the kernel\"");
	_out.println("\techo \"** with almost nothing.\"");
	_out.println("\techo \"** or you change\"");
	_out.println("\techo \"** the MAX_SIZE, which I advise against.\"");
	_out.println("\techo \"** exiting in failure\"");
	_out.println("");
	
	cleanup(_out);

	_out.println("fi");
	_out.flush();
	_out.println("");
	     

    } // end of the functino

    public static void writeRootDir(PrintWriter _out) throws IOException {

	_out.println("");
	_out.println("echo \"Creating ROOT directory on loop device...\"");

	for (int i = 0; i < _rootDirList.length; i++) {
	    
	    _out.println("mkdir -p ${MNTDIR}/" + _rootDirList[i]);

	} // end of the fro			
	_out.println("");	
	_out.println("rm -Rf ${MNTDIR}/lost+found");
	_out.println("");

	
	_out.flush();

    } // end of the function

    public static void writeGrub(String _grub_bin, PrintWriter _out) throws IOException { 

	echo(_out, "Running grub section...");

	_out.println("");
	_out.println(_grub_bin + " --batch --device-map=/dev/null << EOF");
	_out.println("device (fd0) $LOOPDEV");
	_out.println("root (fd0)");
	_out.println("setup (fd0)");
	_out.println("quit");
	_out.println("EOF");
	_out.println("");
	_out.flush();

    } // end of the function

    public static void writeFinish(PrintWriter _out) throws IOException {

	_out.println("");
	_out.println("#================================================");
	_out.println("# FINISH");
	_out.println("#================================================");
	_out.println("echo \"Closing down system...\"");
	_out.println("umount $MNTDIR");
	_out.println("losetup -d $LOOPDEV");
	_out.println("");
	_out.println("echo \"Waiting until is everything is written...\"");
	_out.println("sync");
	_out.println("");

	
	_out.println("if ! cp -a $IMGFILE $BOCHS_DIR ; then");
	_out.println("\techo \"Error: copy error exiting...\"");
	_out.println("fi");

	_out.println("");
	_out.println("echo \"done....\"");		
	_out.flush();

    } // end of the functino
    
    public static void writeMenu() {
	
	FileWriter _f = null;
	PrintWriter _out = null;

	System.out.println("Creating GRUB menu...menu.lst");

	try {

	    _f = new FileWriter("grub-0.94/menu.lst");
	    _out = new PrintWriter(_f);
	    	           
	    _out.println("timeout 5");
	    _out.println("password 1234");
	    _out.println("");
	    _out.println("title linux");
	    _out.println("kernel /vmlinuz root=/dev/fd0");

	    _out.close();
	    
	} catch (IOException e) {
	    
	    e.printStackTrace();
	    
	} // end of the try - catch	
	

    } // end of the functino


    public static void writeRun(String _abs_path, String _grub) {

	String _s_run = "Run";

	File _fr = new File(_s_run);
	
	if (_fr.exists()) {
	    
	    System.out.println("");
	    System.out.println("Error: the File 'Run' exists, we are not going overwrite it.");

	} else {

	    System.out.println("Writing 'Run' script...");
	    
	    FileWriter _f = null;
	    PrintWriter _out = null;
	    
	    try {

		_f = new FileWriter(_fr);
		
		_out = new PrintWriter(_f);
		
		_out.println("#!/bin/sh");
		_out.println("#");
		_out.println("#===================================================");
		_out.println("# Run - Script for OS development");
		_out.println("# Note: This script generated by java app HelpKernel");
		_out.println("#===================================================");
		_out.println("");
		
		//_out.println("CUR_DIR=" + _abs_path);
		_out.println("CUR_DIR=`pwd`");
		_out.println("MAX_SIZE=410");
		
		_out.println("");
		_out.println("LOOPDEV=/dev/loop0");
		_out.println("IMGDIR=${CUR_DIR}/tmp");
		_out.println("IMGFILE=${IMGDIR}/orange.img");
		_out.println("MNTDIR=${CUR_DIR}/mnt");
		_out.println("BOCHS_DIR=${CUR_DIR}/bochs");
		_out.println("");				
		_out.flush();

		_out.println("KERNELFILE=vmlinuz");
		_out.println("GRUB_FILE=${CUR_DIR}/grub-0.94/menu.lst");
		_out.println("");
		_out.println("");
		
		_out.println("STAGE1_FILE=${CUR_DIR}/grub-0.94/stage1");
		_out.println("STAGE2_FILE=${CUR_DIR}/grub-0.94/stage2");

		_out.println("");
		_out.println("DISKSIZE=1440");
		_out.println("");
		_out.println("");
		
		
		// of course change id accordingly
		_out.println("case \"`/bin/id`\" in");
		_out.println("\t\"uid=0(\"*) ;;");
		_out.println("\t*) echo >&2 \"Error: Invalid permissions, login as 'su'\"; exit 1;;");
		_out.println("esac");
		_out.println("");

		echo(_out, "Running....");
		
		goRun(_out);		

		loop(_out);

		writeRootDir(_out);

		makecopy(_out);
		
		writeGrub(_grub, _out);

		writeFinish(_out);
		_out.close();
		
	    } catch(IOException e) {
	    
		e.printStackTrace();
		
	    } // end of the try - catch	
	    
	} // end of the if 
	
    } // end of the function


    public static void usage() {
	
	System.out.println("usage: java org.retro.gis.HelpKernel");

	System.exit(1);

    } // end of the function
    

    public static void writeFirstRun() {
	
	FileWriter _f_run = null;
	PrintWriter _n = null;

	try {
	    
	    _f_run = new FileWriter(".first_run");	    
	    _n = new PrintWriter(_f_run);

	    _n.print("FIRST_RUN");

	    _n.close();

	} catch(IOException e) {

	    e.printStackTrace();

	} // end of the try - catch

    } // end of the functino

    public static void main(String [] args) {
	
	System.out.println("Running Program...");	

	System.out.println("==============================");
	System.out.println("HelpKernel:");
	System.out.println(" - Create proper Kernel Directories and such");
	System.out.println("==============================");
	
	System.out.println("");

	int _run_flag = 1;

	// First check for the .first_run script
	File _f_run = new File(".first_run");
	
	if (_f_run.exists()) {

	    System.out.println("");

	    System.out.println("ErrorBot: This doesnt look right.");
	    System.out.println("ErrorBot: You are trying to re-run the java helper script, [.first_run exists]");
	    System.out.println("ErrorBot: You can do two things:");
	    System.out.println("1. delete the script .first_run (ls -la) and then run ./Clean");
	    System.out.println("2. we can allow you to continue from here, but it wont be pretty");

	    System.out.println("");

	    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));	    
	   
	    try {
		
		String _line = null;

		System.out.print("Do you want to continue? [y/n]: ");

		_line = br.readLine();
	      
		if (_line == null) {

		    // You are probably running ANT
		    System.out.println("");
		    System.out.println("You are probably running ANT, exiting...");
		    System.exit(1);

		} else {

		    br.close();
		    
		    if (_line.equals("y")) {
			
			_run_flag = 209;
			
		    } else {
			
			System.out.println("");
			System.out.println("You made the smart move...");
			System.out.println("Exiting...");
			
		    _run_flag = -1;
		    
		    System.exit(1);
		    
		    } // end of the if - else		

		} // end of the if - else check for null

	    } catch(IOException _e) {

		_e.printStackTrace();
		_run_flag = -1;
		
		System.exit(1);

	    } // end of the try - catch
	   
	} else {

	    System.out.println("");	    
	    System.out.println("The first run script does not exist, we are starting fresh.");
	    System.out.println("");

	    _run_flag = 1;

	} // end of the if - else

	// We will just assume everything is ok at this point
	
	String _grub_check [] = new String[2];
	
	// Running grub GRUB  version 0.93

	_grub_check[0] = "/usr/sbin/grub";          // default OS installs put them here?
	_grub_check[1] = "/usr/local/sbin/grub";    // I got this from the grub install

	String _main_grub = null;
	
	// me personally I think the second one
	File _f_grub = null;
	_f_grub = new File(_grub_check[0]);

	int _chk = -1;
	if (_f_grub.exists()) {
	    _chk = 1;                     // 0 exists
	} // end of thie 
	
	_f_grub = null;
	_f_grub = new File(_grub_check[1]);
	if (_f_grub.exists()) {
	
	    if (_chk == 1)
		_chk = 3;                // 0 and 1 exists
	    else
		_chk = 2;                // only 1 exists
		
	} // end of the if - else
	
	// -1 nothing exists
	if (_chk == 1) {
	    
	    System.out.println(_grub_check[0] + " exists only");

	} else if (_chk == 2) {
	    
	    System.out.println(_grub_check[1] + " exists only");

	} else if (_chk == 3) {
	    
	    System.out.println("grub exists in both paths.");
	    System.out.println("Selecting... " + _grub_check[1]);

	    _main_grub = _grub_check[1];

	} else {
	    
	    System.out.println("Note: grub does not exist, or it is located in some strange path.");

	} // end of the if - else	     

	String _cur_dir = "";
	File _cur = new File(_cur_dir);
	
	_cur_dir = _cur.getAbsolutePath();
	System.out.println("Current Absolute Path: " + _cur_dir);	

	String _img_dir = "tmp";
	String _img_file = "tmp/orange.img";
	String _mnt_dir = "mnt";
	String _bochs_dir = "bochs";

	String _grub_stage1 = "grub-0.94/stage1";
	String _grub_stage2 = "grub-0.94/stage2";


	File _f_chk = new File(_img_dir);
	if (_f_chk.exists()) {
	    
	    System.out.println("IMG_DIR already exists, not touching [" + _img_dir + "]");

	} else {

	    
	    System.out.println("IMG_DIR does not exist, creating [" + _img_dir + "]");

	    if (_f_chk.mkdir()) {
		
		System.out.println("Creating directory...[ok]");

	    }  // end of the if 

	} // end of the if - else

	_f_chk = null;
	
	// --==# Next Directory to Create #==---

	_f_chk = new File(_mnt_dir);
	if (_f_chk.exists()) {
	    
	    System.out.println("MNT_DIR already exists, not touching [" + _mnt_dir + "]");

	} else {

	    
	    System.out.println("MNT_DIR does not exist, creating [" + _mnt_dir + "]");

	    if (_f_chk.mkdir()) {
		
		System.out.println("Creating directory...[ok]");

	    }  // end of the if 

	} // end of the if - else
	
	_f_chk = null;	
	
	_f_chk = new File(_bochs_dir);
	if (_f_chk.exists()) {
	    
	    System.out.println("BOCHS_DIR already exists, not touching [" + _bochs_dir + "]");

	} else {
	    
	    System.out.println("BOCHS_DIR does not exist, creating [" + _bochs_dir + "]");

	    if (_f_chk.mkdir()) {
		
		System.out.println("Creating directory...[ok]");

	    }  // end of the if 

	} // end of the if - else

	_f_chk = null;	

	// check for stage1 and stage2
	_f_chk = new File(_grub_stage1);
	if (_f_chk.exists()) {	    

	    System.out.println("GRUBSTAGE1 exists this is good!");

	} else {	    
	    System.out.println("GRUBSTAGE1 does not exist.");
	    System.out.println("ErrorBot: This is not good, grub_stage1 is needed.");
	    System.out.println("ErrorBot: It is a 512-byte MBR used by GRUB, read the grub docs for more info.");
	    System.out.println("");
	    System.out.println("ErrorBot: And, place stage1 and stage2 in grub-0.94 directory!");

	} // end of the if - else
	_f_chk = null;	

	_f_chk = new File(_grub_stage2);
	if (_f_chk.exists()) {	    
	    System.out.println("GRUBSTAGE2 exists this is good!");
	} else {	    
	    System.out.println("GRUBSTAGE2 does not exist.");
	    System.out.println("ErrorBot: This is not good, grub_stage2 is needed.");
	    System.out.println("");
	    System.out.println("ErrorBot: Place stage1 and stage2 in grub-0.94 directory!");

	} // end of the if - else
	_f_chk = null;	

	writeMenu();
	
	// now all I need is to write the ./Run, ./Write, and ./Run
	writeRun(_cur_dir, _main_grub);

	writeBochs();

	System.out.println("");
	System.out.println("");
	System.out.println("Note:");
	System.out.println("        In order for the scripts to work, ");
	System.out.println("    you must use ( chmod 755 Run )");
	System.out.println(" or you will get a Permission denied error");
	System.out.println("");
       
	// and finally, mark that we have done some work
	writeFirstRun();

	System.out.println("done.");
	
    } // end of the function main
    
} // end of the class
