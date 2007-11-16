//
//
// Berlin Brown
//
// bigbinc@hotmail.com
//
//
// $Id: fs.c,v 1.2 2004/04/06 15:41:46 bigbinc Exp $
//
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


#define _GNU_SOURCE
#include <getopt.h>

//
// getopt_long - see unistd.h

//
// see... getopt_long - unistd.h
//
//
//struct option: 
//              const char *name;
//              int has_arg;
//              int *flag;
//              int val;


static struct option long_options[] = {
      {"help",    0, NULL, 'h'},
      {"version", 0, NULL, 'v'},
      {"options", 0, NULL, 'o'},
};


static void usage(void) {

  fprintf(stderr, "usage: runfs \n");

} // end of the functino ---=-=-

int main(int argc, char **argv) {
  
  char *_tmp;

  printf(".... [ Running program ] ....\n");  

  if (getuid() != 0) {
    fprintf(stderr, "Note: You must be root to use this application!\n");
  } // end of the if --=-=-=-==
  
  while(1) {

    int c;
    int index = 0;

    c = getopt_long(argc, argv, "h:v:o:", 
		    long_options, &index);

    if (c == -1)
      break;

    switch(c) {

    case 'h':
      break;

    case 'o':
      
      _tmp = optarg;

      printf("%s\n", _tmp);

      break;

    case 'v':
      break;

    default:
      usage();
      break;

    };
    

  } // end of the while search through the options

  return 0;

} // end of the function main -=--==-==-=-=--
