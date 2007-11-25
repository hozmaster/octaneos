//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: gonode.c,v 1.1.1.1 2004/03/08 21:42:22 bigbinc Exp $
// gonode.c
//
// mknode script program
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>

extern void usage();

//
// usage
//
int gonode(int argc, char **argv) {

  int major = 0;
  int minor = 0;

  char *path;

  int mode = 0666;

  char *end;
  int args;
	
  if (argc < 3) 
    usage();

  path = argv[1];

  if (!strcmp(argv[2], "b")) {
    mode |= S_IFBLK;
    args = 5;
  } else if (!strcmp(argv[2], "c") || !strcmp(argv[2], "u")) {
    mode |= S_IFCHR;
    args = 5;
  } else if (!strcmp(argv[2], "p")) {
    mode |= S_IFIFO;
    args = 3;
  } else {
    fprintf(stderr, "unknown node type %s\n", argv[2]);
    return 1;
  } // end of the if ++

  if (argc != args) usage();

  if (args == 5) {
    major = strtol(argv[3], &end, 0);
    if (*end) {
      fprintf(stderr, "bad major number %s\n", argv[3]);
      return 1;
    }

    minor = strtol(argv[4], &end, 0);
    if (*end) {
      fprintf(stderr, "bad minor number %s\n", argv[4]);
      return 1;
    }
  } // end of the main if ++


  if (mknod(path, mode, makedev(major, minor))) {
    fprintf(stderr, "mknod failed: %s\n", strerror(errno));
    return 1;
  } // end of the if 

  return 0;
 
} // End of the Function //
