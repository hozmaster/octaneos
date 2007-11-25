//
//
// Berlin Brown
//
// $Id: fs.c,v 1.2 2004/04/06 15:41:46 bigbinc Exp $
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define _GNU_SOURCE
#include <getopt.h>

// getopt_long - see unistd.h
// see... getopt_long - unistd.h
// struct option: 
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
}

int main(int argc, char **argv) {
  
  char *_tmp;
  printf(".... [ Running program ] ....\n");  

  if (getuid() != 0) {
    fprintf(stderr, "Note: You must be root to use this application!\n");
  }  
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
  }
  return 0;
}
