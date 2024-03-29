//
// Berlin Brown
//  bigbinc@hotmail.com
//
//  Copyright (C) 1991, 1992 - Linus Torvalds
//
//   major changes of course from original
//  The observant will say that stdout, will probably not work
//   do you know why, hehe?
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/boot.h>


#define DEFAULT_MAJOR_ROOT 0
#define DEFAULT_MINOR_ROOT 0

#define SETUP_SECTS 4

int fd;


void die(const char * str, ...) {
	va_list args;
	va_start(args, str);
	vfprintf(stderr, str, args);
	fputc('\n', stderr);
	exit(1);
} // end of the function

void usage(void) {
	die("Usage: build bootsect setup system [rootdev] [> image]");
} // end of the functino

void file_open(const char *name) {
	if ((fd = open(name, O_RDONLY, 0)) < 0)
		die("Unable to open `%s': %m", name);
} // end of the function


/**
 * Main program entry point.
 */
int main(int argc, char **argv) {

  unsigned int i;
  unsigned int c;
  unsigned int sz;
 
  unsigned int setup_sectors;
  
  unsigned long sys_size;
  
  unsigned char buf[1024];
  
  unsigned char major_root;
  unsigned char minor_root;
  
  struct stat sb;
 
	
  
  if ((argc < 4) || (argc > 5))
    usage();
  if (argc > 4) {
    
    if (!strcmp(argv[4], "CURRENT")) {
      
      if (stat("/", &sb)) {
	perror("/");
	die("Couldn't stat /");
      } // end of the if 
      
      major_root = major(sb.st_dev);
      minor_root = minor(sb.st_dev);
	    
    } else if (strcmp(argv[4], "FLOPPY")) {
      
      if (stat(argv[4], &sb)) {
	perror(argv[4]);
	      die("Couldn't stat root device.");
      } // end of the if
      
      major_root = major(sb.st_rdev);
      minor_root = minor(sb.st_rdev);
    } else {
      major_root = 0;
	    minor_root = 0;
    } // end of the if - else - inner
    
    
  } else {
    
    major_root = DEFAULT_MAJOR_ROOT;
    minor_root = DEFAULT_MINOR_ROOT;
    
  } // end of the if - else
  
  
  fprintf(stderr, "BUILD-INFO: Running build...\n\n");
  fprintf(stderr, "BUILD-INFO: Root device is (%d, %d)\n", major_root, minor_root);
  
  
  file_open(argv[1]);
  
  i = read(fd, buf, sizeof(buf));
  fprintf(stderr,"Boot sector %d bytes.\n",i);

  if (i != 512)
    die("Boot block must be exactly 512 bytes");

  if (buf[510] != 0x55 || buf[511] != 0xaa)
    die("Boot block hasn't got boot flag (0xAA55)");

  buf[508] = minor_root;
  buf[509] = major_root;

  if (write(1, buf, 512) != 512)
    die("Write call failed");
  
  close (fd);
  

  //
  // setup object --
  //

  file_open(argv[2]);

  for (i=0 ; (c=read(fd, buf, sizeof(buf)))>0 ; i+=c )
    if (write(1, buf, c) != c)
      die("Write call failed");

  
  if (c != 0)
    die("read-error on `setup'");
  close (fd);
  
  // pad unused space
  setup_sectors = (i + 511) / 512;
  
  
  if (setup_sectors < SETUP_SECTS)
    setup_sectors = SETUP_SECTS;
  
  fprintf(stderr, "BUILD-INFO: Setup is %d bytes.\n", i); 
  fprintf(stderr, "BUILD-INFO: Sectors: %d\n", setup_sectors);
	
  memset(buf, 0, sizeof(buf));

  while (i < setup_sectors * 512) {

    c = setup_sectors * 512 - i;
    if (c > sizeof(buf))
      c = sizeof(buf);

    if (write(1, buf, c) != c)
      die("Write call failed");
    i += c;
  } // end of the while

  fprintf(stderr, "BUILD-INFO: padded setup size: %d needed: %d\n",i, 0x800); 
  
  
  file_open(argv[3]);

  
  if (fstat (fd, &sb))
    die("Unable to stat `%s': %m", argv[3]);
  
  sz = sb.st_size;
  fprintf (stderr, "System is %d kB\n", sz/1024);
  
  sys_size = (sz + 15) / 16;
 
  fprintf(stderr, "BUILD-INFO: Writing system stdout...ctr: %d\n\n", sys_size);
  
  while (sz > 0) {
    int l, n;
    
    l = (sz > sizeof(buf)) ? sizeof(buf) : sz;
    if ((n=read(fd, buf, l)) != l) {
      if (n < 0)
	die("Error reading %s: %m", argv[3]);
                        else
			  die("%s: Unexpected EOF", argv[3]);
    }
    if (write(1, buf, l) != l)
      die("Write failed");
    sz -= l;
  } // end of the whlie
  close(fd);
 
  //
  // Normally, we would write the sizes at the end of the
  // bootsector, for simplicity, we wont 
  if (lseek(1, 497, SEEK_SET) != 497)
    die("Output: seek failed");

  buf[0] = setup_sectors;

  if (write(1, buf, 1) != 1)
    die("Write of setup sector count failed");

  if (lseek(1, 500, SEEK_SET) != 500)
    die("Output: seek failed");

  buf[0] = (sys_size & 0xff);
  buf[1] = ((sys_size >> 8) & 0xff);

  if (write(1, buf, 2) != 2)
    die("Write of image length failed");
    
  return 0;

}
