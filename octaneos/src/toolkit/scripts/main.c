//
// Berlin Brown
//
// $Id: main.c,v 1.1.1.1 2004/03/08 21:42:22 bigbinc Exp $
//
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>

extern int gonode(int argc, char **argv);

void usage(void) {

  fprintf(stderr, "usage xnode <path> { b c u p } <major> <minor>\n");
  exit(1);

}

int main(int argc, char **argv) {
  return 0;
}
