//
// Berlin Brown
//  bigbinc@hotmail.com
//
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

int fd;

void die(const char * str, ...)
{
  va_list args;
  va_start(args, str);
  vfprintf(stderr, str, args);
  
  fputc('\n', stderr);
  exit(1);

} // end of the function

void usage(void)
{
  die("Usage: kernel_stats kernel_image");
} // end of the functino

void file_open(const char *name)
{
  
  if ((fd = open(name, O_RDONLY, 0)) < 0)
    die("Unable to open `%s': %m", name);

} // End of Function

//================================================
// Main Entry Point
//================================================
int main(int argc, char **argv) {
  
  struct stat sb;
  unsigned long kernel_size = 0; 

  
  if (argc != 2) {
    usage();

  } else {
  
    fprintf(stdout, "\n ====== [ Kernel Stats and Size ] =========\n\n");
  
    fprintf(stdout, " ARGS[0] = %s\n", argv[0]);
    fprintf(stdout, " ARGS[1] = %s\n", argv[1]);
        
    if (stat("/", &sb)) {
      perror("/");
      die("Couldn't stat /");
    } // end of the if    
    
    if (stat("/", &sb)) {
      perror("/");
      die("Couldn't stat /");
    } // end of the if    

    if (stat(argv[1], &sb)) {
      perror(argv[1]);
      die("Couldn't stat file");
    } // end of the if    
    
    kernel_size = sb.st_size;
    fprintf (stdout, " OS-Kernel on filesystem is [ %d b ]\n", kernel_size);

    fprintf(stdout, "\n ====== [ End Stats ] ============================\n\n");

  } // end of the of the functino main
  
  return 0;

} // end of main --
