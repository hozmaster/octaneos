/************************************************************
 * Berlin Brown
 * bigbinc@hotmail.com
 ***********************************************************
 *
 * $Id: misc.c,v 1.1 2004/03/14 03:23:59 bigbinc Exp $
 *
 * misc.c
 *
 * remember we are running a little short on space
 *
 * combination of gzip.c / vsprintf.c / misc.c / ctype
 *
 * Why didnt you do the normal thing and modularize
 *   place in headers and different files.... I am not normal...
 *
 * some code taken from gcc3.2.2 src
 *  because they have decided to hide stuff in their core engine
 *
 * - (placed printf code here for debugging)
 * 
 * This is a collection of several routines from gzip-1.0.3 
 * adapted for Linux.
 *
 * malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 * puts by Nick Holloway 1993, better puts by Martin Mares 1995
 * High loaded stuff by Hans Lermen & Werner Almesberger, Feb. 1996
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.
 *
 * See LICENSE.OCTANE for more details
 ***********************************************************
 */

#include <stdarg.h>

#include <asm/segment.h>
#include <asm/io.h>

// taken from linkage and other _STDLIB_H_ headers
#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#define INT_MAX                ((int)(~0U>>1))
#define INT_MIN                (-INT_MAX - 1)


#define _U      0x01
#define _L      0x02
#define _D      0x04
#define _C      0x08
#define _P      0x10
#define _S      0x20
#define _X      0x40
#define _SP     0x80


#define do_div(n,base) ({ \
	unsigned long __upper, __low, __high, __mod; \
	asm("":"=a" (__low), "=d" (__high):"A" (n)); \
	__upper = __high; \
	if (__high) { \
		__upper = __high % (base); \
		__high = __high / (base); \
	} \
	asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (base), "0" (__low), "1" (__upper)); \
	asm("":"=A" (n):"a" (__low),"d" (__high)); \
	__mod; \
})

//============================================================
// DEFAULT(typedefs)
//============================================================

typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;

// Just in case I dont like the compilers work
//
//  va_list/va_args/ blah are gcc dependent, try not write the code yourself?

//============================================================
//
// DEFINES
//
//============================================================
#define DEBG(x)
#define DEBG1(x)

#define slide window

#define wp outcnt
#define flush_output(w) (wp=(w),flush_window())

#define CRC_VALUE (crc ^ 0xffffffffL)


#define NEXTBYTE()  (uch)get_byte()
#define NEEDBITS(n) {while(k<(n)){b|=((ulg)NEXTBYTE())<<k;k+=8;}}
#define DUMPBITS(n) {b>>=(n);k-=(n);}


#define BMAX 16         /* maximum bit length of any code (16 for explode) */
#define N_MAX 288       /* maximum number of codes in any set */

#define ASCII_FLAG   0x01 /* bit 0 set: file probably ASCII text */
#define CONTINUATION 0x02 /* bit 1 set: continuation of multi-part gzip file */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define ENCRYPTED    0x20 /* bit 5 set: file is encrypted */
#define RESERVED     0xC0 /* bit 6,7:   reserved */

// INFLATE.C above MISC.C below

#define OF(args)  args
#define STATIC static

#undef memset
#undef memcpy

#define memzero(s, n)     memset ((s), 0, (n))
#define WSIZE 0x8000
		   
#define get_byte()  (inptr < insize ? inbuf[inptr++] : fill_inbuf())
		
#ifdef DEBUG
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif

struct screen_info {
  unsigned char  orig_x;                  /* 0x00 */
  unsigned char  orig_y;                  /* 0x01 */
  unsigned short dontuse1;                /* 0x02 -- EXT_MEM_K sits here */
  unsigned short orig_video_page;         /* 0x04 */
  unsigned char  orig_video_mode;         /* 0x06 */
  unsigned char  orig_video_cols;         /* 0x07 */
  unsigned short unused2;                 /* 0x08 */
  unsigned short orig_video_ega_bx;       /* 0x0a */
  unsigned short unused3;                 /* 0x0c */
  unsigned char  orig_video_lines;        /* 0x0e */
  unsigned char  orig_video_isVGA;        /* 0x0f */
  unsigned short orig_video_points;       /* 0x10 */
};


#define EXT_MEM_K   (*(unsigned short *)(real_mode + 0x2))
#define SCREEN_INFO (*(struct screen_info *)(real_mode+0))

#define _KERNEL_MESSAGE   (*( unsigned short *)(real_mode + 0x0B))
#define _KERNEL_VERSION   (*( unsigned short *)(real_mode + 0x0e))


#define INPLACE_MOVE_ROUTINE  0x1000
#define LOW_BUFFER_START      0x2000
#define LOW_BUFFER_MAX        0x90000
#define HEAP_SIZE             0x3000

#define ZEROPAD	1
#define SIGN	2
#define PLUS	4
#define SPACE	8
#define LEFT	16
#define SPECIAL	32
#define LARGE	64


//============================================================
//
// END Defines 
//
//============================================================


//============================================================
//
// GLOBALS
//
//============================================================

struct huft {
  uch e;                /* number of extra bits or operation */
  uch b;                /* number of bits in this code or subcode */
  union {
    ush n;              /* literal, length base, or distance base */
    struct huft *t;     /* pointer to next level of table */
  } v;
};


STATIC int huft_build OF((unsigned *, unsigned, unsigned, 
			  const ush *, const ush *, struct huft **, int *));
STATIC int huft_free OF((struct huft *));
STATIC int inflate_codes OF((struct huft *, struct huft *, int, int));
STATIC int inflate_stored OF((void));
STATIC int inflate_fixed OF((void));
STATIC int inflate_dynamic OF((void));
STATIC int inflate_block OF((int *));
STATIC int inflate OF((void));


static const unsigned border[] = {    /* Order of the bit length code lengths */
  16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static const ush cplens[] = {         /* Copy lengths for literal codes 257..285 */
  3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
  35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
/* note: see note #13 above about the 258 in this list. */
static const ush cplext[] = {         /* Extra bits for literal codes 257..285 */
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
  3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99}; /* 99==invalid */
static const ush cpdist[] = {         /* Copy offsets for distance codes 0..29 */
  1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
  257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
  8193, 12289, 16385, 24577};
static const ush cpdext[] = {         /* Extra bits for distance codes */
  0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
  7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
  12, 12, 13, 13};


static ulg crc_32_tab[256];
static ulg crc;


STATIC ulg bb;                         /* bit buffer */
STATIC unsigned bk;                    /* bits in bit buffer */

STATIC const ush mask_bits[] = {
  0x0000,
  0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
  0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};



STATIC const int lbits = 9;          /* bits in base literal/length lookup table */
STATIC const int dbits = 6;          /* bits in base distance lookup table */

STATIC unsigned hufts;         /* track memory usage */



// INFLATE.C above ++ MISC.C below ++


static void* memset(void *, int, size_t);
static void* memcpy(void *, __const void *, size_t);


static uch *inbuf;
static uch window[WSIZE];


static unsigned insize = 0;
static unsigned inptr = 0;
static unsigned outcnt = 0;


static int  fill_inbuf(void);
static void flush_window(void);
static void error(char *m);
static void gzip_mark(void **);
static void gzip_release(void **);  

static unsigned char *real_mode;


extern char input_data[];
extern int input_len;

static long bytes_out = 0;
static uch *output_data;
static unsigned long output_ptr = 0;
 
static void *malloc(int size);
static void free(void *where);
static void error(char *m);
static void gzip_mark(void **);
static void gzip_release(void **);
 
static void puts(const char *);
  
extern int end;

static long free_mem_ptr = (long)&end;
static long free_mem_end_ptr;

static unsigned int low_buffer_end, low_buffer_size;
static int high_loaded =0;
static uch *high_buffer_start;

static char *vidmem = (char *)0xb8000;
static int vidport;
static int lines, cols;

#define STACK_SIZE (4096)

long user_stack [STACK_SIZE];

struct {
  long * a;
  short b;
} stack_start = { &user_stack[STACK_SIZE], KERNEL_DS };

// blbz
static void makecrc(void);

int gunzip(void);


//============================================================
//
// END GLOBALS
//
//============================================================

int sprintf(char * buf, const char *fmt, ...);

unsigned char _ctype[] = {
  _C,_C,_C,_C,_C,_C,_C,_C,
  _C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,
  _C,_C,_C,_C,_C,_C,_C,_C,
  _C,_C,_C,_C,_C,_C,_C,_C,
  _S|_SP,_P,_P,_P,_P,_P,_P,_P,
  _P,_P,_P,_P,_P,_P,_P,_P,
  _D,_D,_D,_D,_D,_D,_D,_D,
  _D,_D,_P,_P,_P,_P,_P,_P,
  _P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,
  _U,_U,_U,_U,_U,_U,_U,_U,			
  _U,_U,_U,_U,_U,_U,_U,_U,			
  _U,_U,_U,_P,_P,_P,_P,_P,			
  _P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	
  _L,_L,_L,_L,_L,_L,_L,_L,			
  _L,_L,_L,_L,_L,_L,_L,_L,			
  _L,_L,_L,_P,_P,_P,_P,_C,			
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
  _S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,
  _P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,
  _U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,
  _U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,
  _L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,
  _L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)      ((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)      ((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)      ((__ismask(c)&(_C)) != 0)
#define isdigit(c)      ((__ismask(c)&(_D)) != 0)
#define isgraph(c)      ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)      ((__ismask(c)&(_L)) != 0)
#define isprint(c)      ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)      ((__ismask(c)&(_P)) != 0)
#define isspace(c)      ((__ismask(c)&(_S)) != 0)
#define isupper(c)      ((__ismask(c)&(_U)) != 0)
#define isxdigit(c)     ((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)


static inline unsigned char __toupper(unsigned char c)
{
        if (islower(c))
                c -= 'a'-'A';
        return c;
} // end of the function ++ 

static inline unsigned char __tolower(unsigned char c)
{
        if (isupper(c))
                c -= 'A'-'a';
        return c;
} // end of the functino


#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)


static inline size_t strnlen(const char * s, size_t count)
{
int d0;
register int __res;
__asm__ __volatile__(
        "movl %2,%0\n\t"
        "jmp 2f\n"
        "1:\tcmpb $0,(%0)\n\t"
        "je 3f\n\t"
        "incl %0\n"
        "2:\tdecl %1\n\t"
        "cmpl $-1,%1\n\t"
        "jne 1b\n"
        "3:\tsubl %2,%0"
        :"=a" (__res), "=&d" (d0)
        :"c" (s),"1" (count));
return __res;
} // end of the function ++



//============================================================
// MISC.C - code below
//============================================================

//
// Used when gcc detects corruption of the on-stack canary value
//
void __stack_chk_fail(void) {

}

static void *malloc(int size)
{
  void *p;

  if (size <0) error("Malloc error\n");
  if (free_mem_ptr <= 0) error("Memory error\n");

  free_mem_ptr = (free_mem_ptr + 3) & ~3;	/* Align */

  p = (void *)free_mem_ptr;
  free_mem_ptr += size;

  if (free_mem_ptr >= free_mem_end_ptr)
    error("\n(malloc)Out of memory\n");

  return p;
} // end of the function ++

static void free(void *where) {  }

static void gzip_mark(void **ptr)
{
  *ptr = (void *) free_mem_ptr;
}

static void gzip_release(void **ptr)
{
  free_mem_ptr = (long) *ptr;
}
 
static void scroll(void)
{
  int i;

  memcpy ( vidmem, vidmem + cols * 2, ( lines - 1 ) * cols * 2 );
  for ( i = ( lines - 1 ) * cols * 2; i < lines * cols * 2; i += 2 )
    vidmem[i] = ' ';
}

static void puts(const char *s)
{
  int x,y,pos;
  char c;

  x = (int)SCREEN_INFO.orig_x;
  y = (int)SCREEN_INFO.orig_y;

  while ( ( c = *s++ ) != '\0' ) {
    if ( c == '\n' ) {
      x = 0;
      if ( ++y >= lines ) {
	scroll();
	y--;
      }
    } else {
      vidmem [ ( x + cols * y ) * 2 ] = c; 
      if ( ++x >= cols ) {
	x = 0;
	if ( ++y >= lines ) {
	  scroll();
	  y--;
	}
      }
    }
  }

  SCREEN_INFO.orig_x = x;
  SCREEN_INFO.orig_y = y;

  pos = (x + cols * y) * 2;	/* Update cursor position */
  outb_p(14, vidport);
  outb_p(0xff & (pos >> 9), vidport+1);
  outb_p(15, vidport);
  outb_p(0xff & (pos >> 1), vidport+1);

} // end of the fucntino




static void* memset(void* s, int c, size_t n)
{
  int i;
  char *ss = (char*)s;

  for (i=0;i<n;i++) ss[i] = c;
  return s;

} // end of the functino

static void* memcpy(void* __dest, __const void* __src,
		    size_t __n)
{
  int i;
  char *d = (char *)__dest, *s = (char *)__src;

  for (i=0;i<__n;i++) d[i] = s[i];
  return __dest;

} // end of the functnio


static int fill_inbuf(void)
{
  if (insize != 0) {
	error("ran out of input data\n");
  }

  inbuf = input_data;
  insize = input_len;
  inptr = 1;
  return inbuf[0];

} // end of the functino


static void flush_window_low(void)
{
  ulg c = crc;

  unsigned n;
  uch *in, *out, ch;
    
  in = window;
  out = &output_data[output_ptr]; 
  for (n = 0; n < outcnt; n++) {
    ch = *out++ = *in++;
    c = crc_32_tab[((int)c ^ ch) & 0xff] ^ (c >> 8);
  }
  crc = c;
  bytes_out += (ulg)outcnt;
  output_ptr += (ulg)outcnt;
  outcnt = 0;

} // end of the function

static void flush_window_high(void)
{
  ulg c = crc;         /* temporary variable */
  unsigned n;
  uch *in,  ch;
  in = window;
  for (n = 0; n < outcnt; n++) {
    ch = *output_data++ = *in++;
    if ((ulg)output_data == low_buffer_end) output_data=high_buffer_start;
    c = crc_32_tab[((int)c ^ ch) & 0xff] ^ (c >> 8);
  }
  crc = c;
  bytes_out += (ulg)outcnt;
  outcnt = 0;
}

static void flush_window(void)
{
  if (high_loaded) flush_window_high();
  else flush_window_low();
} // ++ end of the function ++




static void error(char *x)
{
  puts("\n\nErrorBot: ");
  puts(x);
  puts("\n ++ System halted ++ ");

  while(1);

} // end of the function


static void setup_normal_output_buffer(void)
{

  if (EXT_MEM_K < 1024) error("Less than 2MB\n");

  output_data = (char *)0x100000; /* Points to 1M */
  free_mem_end_ptr = (long)real_mode;

} // end of the function

struct moveparams {
  uch *low_buffer_start;  int lcount;
  uch *high_buffer_start; int hcount;
};


//
// This isn't confusing or anything....
// 
static void setup_output_buffer_if_we_run_high(struct moveparams *mv)
{
  high_buffer_start = (uch *)(((ulg)&end) + HEAP_SIZE);

  if (EXT_MEM_K < (3*1024)) error("Less than 4MB.\n");

  mv->low_buffer_start = output_data = (char *)LOW_BUFFER_START;
  low_buffer_end = ((unsigned int)real_mode > LOW_BUFFER_MAX
		    ? LOW_BUFFER_MAX : (unsigned int)real_mode) & ~0xfff;
  low_buffer_size = low_buffer_end - LOW_BUFFER_START;
  
  high_loaded = 1;
  free_mem_end_ptr = (long)high_buffer_start;

  if ( (0x100000 + low_buffer_size) > ((ulg)high_buffer_start)) {

    high_buffer_start = (uch *)(0x100000 + low_buffer_size);
    mv->hcount = 0;

  } // end of the if 
  else mv->hcount = -1;
  mv->high_buffer_start = high_buffer_start;


} // end of the functino ++

static void close_output_buffer_if_we_run_high(struct moveparams *mv)
{

  char _buf[80];
	
  if (bytes_out > low_buffer_size) {
    mv->lcount = low_buffer_size;
    if (mv->hcount)
      mv->hcount = bytes_out - low_buffer_size;
  } else {
    mv->lcount = bytes_out;
    mv->hcount = 0;
  } // end of the if - else


  sprintf(_buf, "HighBufStart: %x HCount: %x _end: %x \n",
                mv->high_buffer_start, mv->hcount, &end); puts(_buf);
  sprintf(_buf, "LowBufStart: %x %x %d\n", 
		  mv->low_buffer_start, mv->lcount,0xfff000);
  puts(_buf);
				    
  
} // ++ end of the function ++

//++
//++ blbz
//++  - gunzip the kernel
//++  - added printf code for checking memory locations etc
//++
asmlinkage int decompress_kernel(unsigned int _eax, 
	unsigned int _cs, unsigned int _ds, void *rmode)
{
  real_mode = rmode;
	
  SCREEN_INFO.orig_x = 0;
  SCREEN_INFO.orig_y = 6;

  SCREEN_INFO.orig_video_page = 0;
  SCREEN_INFO.orig_video_mode = 0;
  SCREEN_INFO.orig_video_cols = 80;
  SCREEN_INFO.orig_video_lines = 25;
	
  if (SCREEN_INFO.orig_video_mode == 7) {

    vidmem = (char *) 0xb0000;
    vidport = 0x3b4;

  } else {
    vidmem = (char *) 0xb8000;
    vidport = 0x3d4;

  } // end of IF--

  lines = SCREEN_INFO.orig_video_lines;
  cols = SCREEN_INFO.orig_video_cols;

  EXT_MEM_K = (unsigned short)0xAFFF;

  struct moveparams mv;

  // ext = short(real_mode) + 2

  char buf[80];
  sprintf(buf, "[...checking memory locations...]\n"); puts(buf);
  sprintf(buf, "001 >> vidmem(mem)...0x%xh\n", (unsigned int)vidmem); puts(buf);
  sprintf(buf, "002 >> screen_info(mem)...0x%xh\n", (unsigned int)&SCREEN_INFO); puts(buf);
  sprintf(buf, "003 >> real_mode_ptr(mem)...0x%xh\n", (unsigned char *)&EXT_MEM_K); puts(buf);
	
  sprintf(buf, "004 >> EXT_MEM_K...0x%xh\n", EXT_MEM_K); puts(buf);
  
  sprintf(buf, "005 >> EAX...0x%xh\n", _eax); puts(buf);
  sprintf(buf, "006 >> CS...0x%xh\n", _cs); puts(buf);
  sprintf(buf, "007 >> DS...0x%xh\n", _ds); puts(buf);

  sprintf(buf, "008 >> KERN_MSG...%x h\n", _KERNEL_MESSAGE); puts(buf);
  sprintf(buf, "009 >> KERN_VER...%x h\n", _KERNEL_VERSION); puts(buf);

  //
  // -- prepare --
  setup_output_buffer_if_we_run_high(&mv);
  
  makecrc();

  puts("========================================\n");
  puts("INFO: Uncompressing OS-Kernel...\n");
  puts("========================================\n");
  gunzip();
  puts("Ok, booting the kernel.\n");
  close_output_buffer_if_we_run_high(&mv);  
  return high_loaded;

}

//============================================================
// END MISC.C
//============================================================


//============================================================
//
// INFLATE.C - code below
//
//============================================================



STATIC int huft_build(b, n, s, d, e, t, m)
     unsigned *b;            /* code lengths in bits (all assumed <= BMAX) */
     unsigned n;             /* number of codes (assumed <= N_MAX) */
     unsigned s;             /* number of simple-valued codes (0..s-1) */
     const ush *d;                 /* list of base values for non-simple codes */
     const ush *e;                 /* list of extra bits for non-simple codes */
     struct huft **t;        /* result: starting table */
     int *m;                 /* maximum lookup bits, returns actual */
{
  unsigned a;                   /* counter for codes of length k */
  unsigned c[BMAX+1];           /* bit length count table */
  unsigned f;                   /* i repeats in table every f entries */
  int g;                        /* maximum code length */
  int h;                        /* table level */
  register unsigned i;          /* counter, current code */
  register unsigned j;          /* counter */
  register int k;               /* number of bits in current code */
  int l;                        /* bits per table (returned in m) */
  register unsigned *p;         /* pointer into c[], b[], or v[] */
  register struct huft *q;      /* points to current table */
  struct huft r;                /* table entry for structure assignment */
  struct huft *u[BMAX];         /* table stack */
  unsigned v[N_MAX];            /* values in order of bit length */
  register int w;               /* bits before this table == (l * h) */
  unsigned x[BMAX+1];           /* bit offsets, then code stack */
  unsigned *xp;                 /* pointer into x */
  int y;                        /* number of dummy codes added */
  unsigned z;                   /* number of entries in current table */

  DEBG("huft1 ");

  /* Generate counts for each bit length */
  memzero(c, sizeof(c));
  p = b;  i = n;
  do {
    Tracecv(*p, (stderr, (n-i >= ' ' && n-i <= '~' ? "%c %d\n" : "0x%x %d\n"), 
		 n-i, *p));
    c[*p]++;                    /* assume all entries <= BMAX */
    p++;                      /* Can't combine with above line (Solaris bug) */
  } while (--i);
  if (c[0] == n)                /* null input--all zero length codes */
    {
      *t = (struct huft *)NULL;
      *m = 0;
      return 0;
    }

  DEBG("huft2 ");

  /* Find minimum and maximum length, bound *m by those */
  l = *m;
  for (j = 1; j <= BMAX; j++)
    if (c[j])
      break;
  k = j;                        /* minimum code length */
  if ((unsigned)l < j)
    l = j;
  for (i = BMAX; i; i--)
    if (c[i])
      break;
  g = i;                        /* maximum code length */
  if ((unsigned)l > i)
    l = i;
  *m = l;

  DEBG("huft3 ");

  /* Adjust last length count to fill out codes, if needed */
  for (y = 1 << j; j < i; j++, y <<= 1)
    if ((y -= c[j]) < 0)
      return 2;                 /* bad input: more codes than bits */
  if ((y -= c[i]) < 0)
    return 2;
  c[i] += y;

  DEBG("huft4 ");

  /* Generate starting offsets into the value table for each length */
  x[1] = j = 0;
  p = c + 1;  xp = x + 2;
  while (--i) {                 /* note that i == g from above */
    *xp++ = (j += *p++);
  }

  DEBG("huft5 ");

  /* Make a table of values in order of bit lengths */
  p = b;  i = 0;
  do {
    if ((j = *p++) != 0)
      v[x[j]++] = i;
  } while (++i < n);

  DEBG("h6 ");

  /* Generate the Huffman codes and for each, make the table entries */
  x[0] = i = 0;                 /* first Huffman code is zero */
  p = v;                        /* grab values in bit order */
  h = -1;                       /* no tables yet--level -1 */
  w = -l;                       /* bits decoded == (l * h) */
  u[0] = (struct huft *)NULL;   /* just to keep compilers happy */
  q = (struct huft *)NULL;      /* ditto */
  z = 0;                        /* ditto */
  DEBG("h6a ");

  /* go through the bit lengths (k already is bits in shortest code) */
  for (; k <= g; k++)
    {
      DEBG("h6b ");
      a = c[k];
      while (a--)
	{
	  DEBG("h6b1 ");
	  /* here i is the Huffman code of length k bits for value *p */
	  /* make tables up to required level */
	  while (k > w + l)
	    {
	      DEBG1("1 ");
	      h++;
	      w += l;                 /* previous table always l bits */

	      /* compute minimum size table less than or equal to l bits */
	      z = (z = g - w) > (unsigned)l ? l : z;  /* upper limit on table size */
	      if ((f = 1 << (j = k - w)) > a + 1)     /* try a k-w bit table */
		{                       /* too few codes for k-w bit table */
		  DEBG1("2 ");
		  f -= a + 1;           /* deduct codes from patterns left */
		  xp = c + k;
		  while (++j < z)       /* try smaller tables up to z bits */
		    {
		      if ((f <<= 1) <= *++xp)
			break;            /* enough codes to use up j bits */
		      f -= *xp;           /* else deduct codes from patterns */
		    }
		}
	      DEBG1("3 ");
	      z = 1 << j;             /* table entries for j-bit table */

	      /* allocate and link in new table */
	      if ((q = (struct huft *)malloc((z + 1)*sizeof(struct huft))) ==
		  (struct huft *)NULL)
		{
		  if (h)
		    huft_free(u[0]);
		  return 3;             /* not enough memory */
		}
	      DEBG1("4 ");
	      hufts += z + 1;         /* track memory usage */
	      *t = q + 1;             /* link to list for huft_free() */
	      *(t = &(q->v.t)) = (struct huft *)NULL;
	      u[h] = ++q;             /* table starts after link */

	      DEBG1("5 ");
	      /* connect to last table, if there is one */
	      if (h)
		{
		  x[h] = i;             /* save pattern for backing up */
		  r.b = (uch)l;         /* bits to dump before this table */
		  r.e = (uch)(16 + j);  /* bits in this table */
		  r.v.t = q;            /* pointer to this table */
		  j = i >> (w - l);     /* (get around Turbo C bug) */
		  u[h-1][j] = r;        /* connect to last table */
		}
	      DEBG1("6 ");
	    }
	  DEBG("h6c ");

	  /* set up table entry in r */
	  r.b = (uch)(k - w);
	  if (p >= v + n)
	    r.e = 99;               /* out of values--invalid code */
	  else if (*p < s)
	    {
	      r.e = (uch)(*p < 256 ? 16 : 15);    /* 256 is end-of-block code */
	      r.v.n = (ush)(*p);             /* simple code is just the value */
	      p++;                           /* one compiler does not like *p++ */
	    }
	  else
	    {
	      r.e = (uch)e[*p - s];   /* non-simple--look up in lists */
	      r.v.n = d[*p++ - s];
	    }
	  DEBG("h6d ");

	  /* fill code-like entries with r */
	  f = 1 << (k - w);
	  for (j = i >> w; j < z; j += f)
	    q[j] = r;

	  /* backwards increment the k-bit code i */
	  for (j = 1 << (k - 1); i & j; j >>= 1)
	    i ^= j;
	  i ^= j;

	  /* backup over finished tables */
	  while ((i & ((1 << w) - 1)) != x[h])
	    {
	      h--;                    /* don't need to update q */
	      w -= l;
	    }
	  DEBG("h6e ");
	}
      DEBG("h6f ");
    }

  DEBG("huft7 ");

  return y != 0 && g != 1;
} // end of the function




STATIC int huft_free(t)
     struct huft *t;         /* table to free */
{
  register struct huft *p, *q;


  /* Go through linked list, freeing from the malloced (t[-1]) address. */
  p = t;
  while (p != (struct huft *)NULL)
    {
      q = (--p)->v.t;
      free((char*)p);
      p = q;
    } 
  return 0;
}


STATIC int inflate_codes(tl, td, bl, bd)
     struct huft *tl, *td;   /* literal/length and distance decoder tables */
     int bl, bd;             /* number of bits decoded by tl[] and td[] */
     /* inflate (decompress) the codes in a deflated (compressed) block.
	Return an error code or zero if it all goes ok. */
{
  register unsigned e;  /* table entry flag/number of extra bits */
  unsigned n, d;        /* length and index for copy */
  unsigned w;           /* current window position */
  struct huft *t;       /* pointer to table entry */
  unsigned ml, md;      /* masks for bl and bd bits */
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */


  /* make local copies of globals */
  b = bb;                       /* initialize bit buffer */
  k = bk;
  w = wp;                       /* initialize window position */

  /* inflate the coded data */
  ml = mask_bits[bl];           /* precompute masks for speed */
  md = mask_bits[bd];
  for (;;)                      /* do until end of block */
    {
      NEEDBITS((unsigned)bl)
	if ((e = (t = tl + ((unsigned)b & ml))->e) > 16)
	  do {
	    if (e == 99)
	      return 1;
	    DUMPBITS(t->b)
	      e -= 16;
	    NEEDBITS(e)
	      } while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
      DUMPBITS(t->b)
	if (e == 16)                /* then it's a literal */
	  {
	    slide[w++] = (uch)t->v.n;
	    Tracevv((stderr, "%c", slide[w-1]));
	    if (w == WSIZE)
	      {
		flush_output(w);
		w = 0;
	      }
	  }
	else                        /* it's an EOB or a length */
	  {
	    /* exit if end of block */
	    if (e == 15)
	      break;

	    /* get length of block to copy */
	    NEEDBITS(e)
	      n = t->v.n + ((unsigned)b & mask_bits[e]);
	    DUMPBITS(e);

	    /* decode distance of block to copy */
	    NEEDBITS((unsigned)bd)
	      if ((e = (t = td + ((unsigned)b & md))->e) > 16)
		do {
		  if (e == 99)
		    return 1;
		  DUMPBITS(t->b)
		    e -= 16;
		  NEEDBITS(e)
		    } while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
	    DUMPBITS(t->b)
	      NEEDBITS(e)
	      d = w - t->v.n - ((unsigned)b & mask_bits[e]);
	    DUMPBITS(e)
	      Tracevv((stderr,"\\[%d,%d]", w-d, n));

	    /* do the copy */
	    do {
	      n -= (e = (e = WSIZE - ((d &= WSIZE-1) > w ? d : w)) > n ? n : e);
#if !defined(NOMEMCPY) && !defined(DEBUG)
	      if (w - d >= e)         /* (this test assumes unsigned comparison) */
		{
		  memcpy(slide + w, slide + d, e);
		  w += e;
		  d += e;
		}
	      else                      /* do it slow to avoid memcpy() overlap */
#endif /* !NOMEMCPY */
		do {
		  slide[w++] = slide[d++];
		  Tracevv((stderr, "%c", slide[w-1]));
		} while (--e);
	      if (w == WSIZE)
		{
		  flush_output(w);
		  w = 0;
		}
	    } while (n);
	  }
    }


  /* restore the globals from the locals */
  wp = w;                       /* restore global window pointer */
  bb = b;                       /* restore global bit buffer */
  bk = k;

  /* done */
  return 0;
} // end of the fucntion


STATIC int inflate_stored()
{
  unsigned n;           /* number of bytes in block */
  unsigned w;           /* current window position */
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */

  DEBG("<stor");

  /* make local copies of globals */
  b = bb;                       /* initialize bit buffer */
  k = bk;
  w = wp;                       /* initialize window position */


  /* go to byte boundary */
  n = k & 7;
  DUMPBITS(n);


  /* get the length and its complement */
  NEEDBITS(16)
    n = ((unsigned)b & 0xffff);
  DUMPBITS(16)
    NEEDBITS(16)
    if (n != (unsigned)((~b) & 0xffff))
      return 1;                   /* error in compressed data */
  DUMPBITS(16)


    /* read and output the compressed data */
    while (n--)
      {
	NEEDBITS(8)
	  slide[w++] = (uch)b;
	if (w == WSIZE)
	  {
	    flush_output(w);
	    w = 0;
	  }
	DUMPBITS(8)
	  }


  /* restore the globals from the locals */
  wp = w;                       /* restore global window pointer */
  bb = b;                       /* restore global bit buffer */
  bk = k;

  DEBG(">");
  return 0;
} // end of the functino



STATIC int inflate_fixed()
{
  int i;                /* temporary variable */
  struct huft *tl;      /* literal/length code table */
  struct huft *td;      /* distance code table */
  int bl;               /* lookup bits for tl */
  int bd;               /* lookup bits for td */
  unsigned l[288];      /* length list for huft_build */

  DEBG("<fix");

  /* set up literal table */
  for (i = 0; i < 144; i++)
    l[i] = 8;
  for (; i < 256; i++)
    l[i] = 9;
  for (; i < 280; i++)
    l[i] = 7;
  for (; i < 288; i++)          /* make a complete, but wrong code set */
    l[i] = 8;
  bl = 7;
  if ((i = huft_build(l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
    return i;


  /* set up distance table */
  for (i = 0; i < 30; i++)      /* make an incomplete code set */
    l[i] = 5;
  bd = 5;
  if ((i = huft_build(l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
    {
      huft_free(tl);

      DEBG(">");
      return i;
    }


  /* decompress until an end-of-block code */
  if (inflate_codes(tl, td, bl, bd))
    return 1;


  /* free the decoding tables, return */
  huft_free(tl);
  huft_free(td);

  return 0;
} // end of the functino



STATIC int inflate_dynamic()
     /* decompress an inflated type 2 (dynamic Huffman codes) block. */
{
  int i;                /* temporary variables */
  unsigned j;
  unsigned l;           /* last length */
  unsigned m;           /* mask for bit lengths table */
  unsigned n;           /* number of lengths to get */
  struct huft *tl;      /* literal/length code table */
  struct huft *td;      /* distance code table */
  int bl;               /* lookup bits for tl */
  int bd;               /* lookup bits for td */
  unsigned nb;          /* number of bit length codes */
  unsigned nl;          /* number of literal/length codes */
  unsigned nd;          /* number of distance codes */
#ifdef PKZIP_BUG_WORKAROUND
  unsigned ll[288+32];  /* literal/length and distance code lengths */
#else
  unsigned ll[286+30];  /* literal/length and distance code lengths */
#endif
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */

  DEBG("<dyn");

  /* make local bit buffer */
  b = bb;
  k = bk;


  /* read in table lengths */
  NEEDBITS(5)
    nl = 257 + ((unsigned)b & 0x1f);      /* number of literal/length codes */
  DUMPBITS(5)
    NEEDBITS(5)
    nd = 1 + ((unsigned)b & 0x1f);        /* number of distance codes */
  DUMPBITS(5)
    NEEDBITS(4)
    nb = 4 + ((unsigned)b & 0xf);         /* number of bit length codes */
  DUMPBITS(4)
#ifdef PKZIP_BUG_WORKAROUND
    if (nl > 288 || nd > 32)
#else
      if (nl > 286 || nd > 30)
#endif
	return 1;                   /* bad lengths */

  DEBG("dyn1 ");

  /* read in bit-length-code lengths */
  for (j = 0; j < nb; j++)
    {
      NEEDBITS(3)
	ll[border[j]] = (unsigned)b & 7;
      DUMPBITS(3)
	}
  for (; j < 19; j++)
    ll[border[j]] = 0;

  DEBG("dyn2 ");

  /* build decoding table for trees--single level, 7 bit lookup */
  bl = 7;
  if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
    {
      if (i == 1)
	huft_free(tl);
      return i;                   /* incomplete code set */
    }

  DEBG("dyn3 ");

  /* read in literal and distance code lengths */
  n = nl + nd;
  m = mask_bits[bl];
  i = l = 0;
  while ((unsigned)i < n)
    {
      NEEDBITS((unsigned)bl)
	j = (td = tl + ((unsigned)b & m))->b;
      DUMPBITS(j)
	j = td->v.n;
      if (j < 16)                 /* length of code in bits (0..15) */
	ll[i++] = l = j;          /* save last length in l */
      else if (j == 16)           /* repeat last length 3 to 6 times */
	{
	  NEEDBITS(2)
	    j = 3 + ((unsigned)b & 3);
	  DUMPBITS(2)
	    if ((unsigned)i + j > n)
	      return 1;
	  while (j--)
	    ll[i++] = l;
	}
      else if (j == 17)           /* 3 to 10 zero length codes */
	{
	  NEEDBITS(3)
	    j = 3 + ((unsigned)b & 7);
	  DUMPBITS(3)
	    if ((unsigned)i + j > n)
	      return 1;
	  while (j--)
	    ll[i++] = 0;
	  l = 0;
	}
      else                        /* j == 18: 11 to 138 zero length codes */
	{
	  NEEDBITS(7)
	    j = 11 + ((unsigned)b & 0x7f);
	  DUMPBITS(7)
	    if ((unsigned)i + j > n)
	      return 1;
	  while (j--)
	    ll[i++] = 0;
	  l = 0;
	}
    }

  DEBG("dyn4 ");

  /* free decoding table for trees */
  huft_free(tl);

  DEBG("dyn5 ");

  /* restore the global bit buffer */
  bb = b;
  bk = k;

  DEBG("dyn5a ");

  /* build the decoding tables for literal/length and distance codes */
  bl = lbits;
  if ((i = huft_build(ll, nl, 257, cplens, cplext, &tl, &bl)) != 0)
    {
      DEBG("dyn5b ");
      if (i == 1) {
	error(" incomplete literal tree\n");
	huft_free(tl);
      }
      return i;                   /* incomplete code set */
    }
  DEBG("dyn5c ");
  bd = dbits;
  if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
    {
      DEBG("dyn5d ");
      if (i == 1) {
	error(" incomplete distance tree\n");
#ifdef PKZIP_BUG_WORKAROUND
	i = 0;
      }
#else
      huft_free(td);
    }
  huft_free(tl);
  return i;                   /* incomplete code set */
#endif
}

DEBG("dyn6 ");

/* decompress until an end-of-block code */
if (inflate_codes(tl, td, bl, bd))
     return 1;

     DEBG("dyn7 ");

     /* free the decoding tables, return */
     huft_free(tl);
     huft_free(td);

     DEBG(">");
     return 0;

     } // end of the functino



STATIC int inflate_block(e)
     int *e;                 /* last block flag */
     /* decompress an inflated block */
{
unsigned t;           /* block type */
register ulg b;       /* bit buffer */
register unsigned k;  /* number of bits in bit buffer */

DEBG("<blk");

/* make local bit buffer */
b = bb;
k = bk;


/* read in last block bit */
NEEDBITS(1)
     *e = (int)b & 1;
     DUMPBITS(1)


     /* read in block type */
     NEEDBITS(2)
     t = (unsigned)b & 3;
     DUMPBITS(2)


     /* restore the global bit buffer */
     bb = b;
     bk = k;

     /* inflate that block type */
     if (t == 2)
     return inflate_dynamic();
     if (t == 0)
     return inflate_stored();
     if (t == 1)
     return inflate_fixed();

     DEBG(">");

     /* bad block type */
     return 2;
     }



STATIC int inflate()
     /* decompress an inflated entry */
{
  int e;                /* last block flag */
  int r;                /* result code */
  unsigned h;           /* maximum struct huft's malloc'ed */
  void *ptr;

  /* initialize window, bit buffer */
  wp = 0;
  bk = 0;
  bb = 0;


  /* decompress until the last block */
  h = 0;
  do {
    hufts = 0;
    gzip_mark(&ptr);
    if ((r = inflate_block(&e)) != 0) {
      gzip_release(&ptr);	    
      return r;
    }
    gzip_release(&ptr);
    if (hufts > h)
      h = hufts;
  } while (!e);

  /* Undo too much lookahead. The next read will be byte aligned so we
   * can discard unused bits in the last meaningful byte.
   */
  while (bk >= 8) {
    bk -= 8;
    inptr--;
  }

  /* flush out slide */
  flush_output(wp);


  /* return success */
#ifdef DEBUG
  fprintf(stderr, "<%u> ", h);
#endif /* DEBUG */

  return 0;

} // end of the functino


static void makecrc(void)
{
  unsigned long c;
  unsigned long e;
  int i;
  int k;

  static const int p[] = { 0,1,2,4,5,7,8,10,11,12,16,22,23,26 };

  e = 0;
  for (i = 0; i < sizeof(p)/sizeof(int); i++)
    e |= 1L << (31 - p[i]);

  crc_32_tab[0] = 0;

  for (i = 1; i < 256; i++)
    {
      c = 0;

      for (k = i | 256; k != 1; k >>= 1)
	{
	  c = c & 1 ? (c >> 1) ^ e : c >> 1;
	  if (k & 1)
	    c ^= e;
	} // end of the for ++

      crc_32_tab[i] = c;

    } // end of the for  ++

  crc = (ulg)0xffffffffL;

} // end of the functin


//
// ++ gunzip ++
//
int gunzip(void)
{
  uch flags;
  unsigned char magic[2];
  char method;
  ulg orig_crc = 0;
  ulg orig_len = 0;
  int res;
  char _buf[80];

  magic[0] = (unsigned char)get_byte();
  magic[1] = (unsigned char)get_byte();
  method = (unsigned char)get_byte();

  if (magic[0] != 037 ||
      ((magic[1] != 0213) && (magic[1] != 0236))) {
    error("bad gzip magic numbers");
    return -1;
  } // end of the if 
	
  sprintf(_buf, "MagicNumber: %x %x [ %x %x ]\n", 
		  magic[0], magic[1], 037, 0213); puts(_buf);
  

  if (method != 8)  {
    error("internal error, invalid method");
    return -1;
  }

  flags  = (uch)get_byte();
  if ((flags & ENCRYPTED) != 0) {
    error("Input is encrypted\n");
    return -1;
  }
  if ((flags & CONTINUATION) != 0) {
    error("Multi part input\n");
    return -1;
  }
  if ((flags & RESERVED) != 0) {
    error("Input has invalid flags\n");
    return -1;
  }
  (ulg)get_byte();	/* Get timestamp */
  ((ulg)get_byte()) << 8;
  ((ulg)get_byte()) << 16;
  ((ulg)get_byte()) << 24;

  (void)get_byte();  /* Ignore extra flags for the moment */
  (void)get_byte();  /* Ignore OS type for the moment */

  if ((flags & EXTRA_FIELD) != 0) {

    unsigned len = (unsigned)get_byte();
    len |= ((unsigned)get_byte())<<8;
    while (len--) (void)get_byte();

  } // end of the if == 

  if ((flags & ORIG_NAME) != 0) {

    while (get_byte() != 0);

  } // end of the if 
  
  
  if ((flags & COMMENT) != 0) {
    while (get_byte() != 0);
  } // end of the if 

  
  if ((res = inflate())) {
    switch (res) {
    case 0:
      break;
    case 1:
      error("invalid compressed format (err=1)");
      break;
    case 2:
      error("invalid compressed format (err=2)");
      break;
    case 3:
      error("out of memory");
      break;
    default:
      error("invalid compressed format (other)");
    }
    return -1;
  } // end of teh if == 
	    
  orig_crc = (ulg) get_byte();
  orig_crc |= (ulg) get_byte() << 8;
  orig_crc |= (ulg) get_byte() << 16;
  orig_crc |= (ulg) get_byte() << 24;
    
  orig_len = (ulg) get_byte();
  orig_len |= (ulg) get_byte() << 8;
  orig_len |= (ulg) get_byte() << 16;
  orig_len |= (ulg) get_byte() << 24;
    
  if (orig_crc != CRC_VALUE) {
    error("crc error");
    return -1;
  } else {
    puts("(gunzip) CRC-Check...[ok]\n");
  } // end of teh if


  if (orig_len != bytes_out) {
    error("length error");
    return -1;
  } else {
    sprintf(_buf,"(gunzip) Length-Check...[ %x, %d k ]  [ok]\n", 
		    bytes_out, bytes_out); puts(_buf);
  } // end of the if - else

  return 0;

} // end of the function ++

//==========================================================
//
// END INFLATE.C
//
//==========================================================


//==========================================================
//
// START() - VSPRINTF.C - 
//
//==========================================================



unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((*cp == 'x') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} // end of the if ++

	while (isxdigit(*cp) &&
	       (value = isdigit(*cp) ? *cp-'0' : toupper(*cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	} // end of the while +_+

	if (endp)
		*endp = (char *)cp;
	return result;

} // end of the function

long simple_strtol(const char *cp,char **endp,unsigned int base)
{

	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);

} // end of the function


unsigned long long simple_strtoull(const char *cp,char **endp,unsigned int base)
{
	unsigned long long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((*cp == 'x') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
} // end of the function

long long simple_strtoll(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoull(cp+1,endp,base);
	return simple_strtoull(cp,endp,base);
} // end of the function

static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
} // end of the function

static char *number(char * buf, char * end, long long num, int base, int size, int precision, int type)
{
	char c,sign,tmp[66];
	const char *digits;
	const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT))) {
		while(size-->0) {
			if (buf <= end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign) {
		if (buf <= end)
			*buf = sign;
		++buf;
	}
	if (type & SPECIAL) {
		if (base==8) {
			if (buf <= end)
				*buf = '0';
			++buf;
		} else if (base==16) {
			if (buf <= end)
				*buf = '0';
			++buf;
			if (buf <= end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & LEFT)) {
		while (size-- > 0) {
			if (buf <= end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--) {
		if (buf <= end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0) {
		if (buf <= end)
			*buf = tmp[i];
		++buf;
	}

	while (size-- > 0) {
		if (buf <= end)
			*buf = ' ';
		++buf;
	} // end of the while 

	return buf;

} // end of the functino ++


int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	char *str, *end, c;
	const char *s;

	int flags;
	int field_width;
	int precision;
	int qualifier;		       

	str = buf;
	end = buf + size - 1;

	if (end < buf - 1) {
		end = ((void *) -1);
		size = end - buf + 1;
	}

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			if (str <= end)
				*str = *fmt;
			++str;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt =='Z') {
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l') {
				qualifier = 'L';
				++fmt;
			}
		}

		/* default base */
		base = 10;

		switch (*fmt) {
			case 'c':
				if (!(flags & LEFT)) {
					while (--field_width > 0) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				c = (unsigned char) va_arg(args, int);
				if (str <= end)
					*str = c;
				++str;
				while (--field_width > 0) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 's':
				s = va_arg(args, char *);
				if (!s)
					s = "<NULL>";

				len = strnlen(s, precision);

				if (!(flags & LEFT)) {
					while (len < field_width--) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				for (i = 0; i < len; ++i) {
					if (str <= end)
						*str = *s;
					++str; ++s;
				}
				while (len < field_width--) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 'p':
				if (field_width == -1) {
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = number(str, end,
						(unsigned long) va_arg(args, void *),
						16, field_width, precision, flags);
				continue;


			case 'n':

				if (qualifier == 'l') {
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} else if (qualifier == 'Z') {
					size_t * ip = va_arg(args, size_t *);
					*ip = (str - buf);
				} else {
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;

			case '%':
				if (str <= end)
					*str = '%';
				++str;
				continue;

				/* integer number formats - set up the flags and "break" */
			case 'o':
				base = 8;
				break;

			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;

			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;

			default:
				if (str <= end)
					*str = '%';
				++str;
				if (*fmt) {
					if (str <= end)
						*str = *fmt;
					++str;
				} else {
					--fmt;
				}
				continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long long);
		else if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z') {
			num = va_arg(args, size_t);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, end, num, base,
				field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		*end = '\0';

	return str-buf;

} // end of the function ++

int snprintf(char * buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsnprintf(buf,size,fmt,args);
	va_end(args);
	return i;
} // end of the functionx


int vsprintf(char *buf, const char *fmt, va_list args)
{
	return vsnprintf(buf, 0xFFFFFFFFUL, fmt, args);
} // end of the functino

int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
} // end of the functino

int vsscanf(const char * buf, const char * fmt, va_list args)
{
	const char *str = buf;
	char *next;
	int num = 0;
	int qualifier;
	int base;
	int field_width = -1;
	int is_sign = 0;

	while(*fmt && *str) {
		
		if (isspace(*fmt)) {
			while (isspace(*fmt))
				++fmt;
			while (isspace(*str))
				++str;
		}

		/* anything that is not a conversion must match exactly */
		if (*fmt != '%' && *fmt) {
			if (*fmt++ != *str++)
				break;
			continue;
		}

		if (!*fmt)
			break;
		++fmt;
		
		if (*fmt == '*') {
			while (!isspace(*fmt) && *fmt)
				fmt++;
			while (!isspace(*str) && *str)
				str++;
			continue;
		}

		/* get field width */
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);

		/* get conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z') {
			qualifier = *fmt;
			fmt++;
		}
		base = 10;
		is_sign = 0;

		if (!*fmt || !*str)
			break;

		switch(*fmt++) {
		case 'c':
		{
			char *s = (char *) va_arg(args,char*);
			if (field_width == -1)
				field_width = 1;
			do {
				*s++ = *str++;
			} while(field_width-- > 0 && *str);
			num++;
		}
		continue;
		case 's':
		{
			char *s = (char *) va_arg(args, char *);
			if (field_width == -1)
			  field_width = INT_MAX;

			while (isspace(*str))
				str++;

			/* now copy until next white space */
			while (*str && !isspace(*str) && field_width--) {
				*s++ = *str++;
			}
			*s = '\0';
			num++;
		}
		continue;
		case 'n':
			/* return number of characters read so far */
		{
			int *i = (int *)va_arg(args,int*);
			*i = str - buf;
		}
		continue;
		case 'o':
			base = 8;
			break;
		case 'x':
		case 'X':
			base = 16;
			break;
		case 'd':
		case 'i':
			is_sign = 1;
		case 'u':
			break;
		case '%':
			/* looking for '%' in str */
			if (*str++ != '%') 
				return num;
			continue;
		default:
			/* invalid format; stop here */
			return num;
		}

	
		while (isspace(*str))
			str++;

		if (!*str || !isdigit(*str))
			break;

		switch(qualifier) {
		case 'h':
			if (is_sign) {
				short *s = (short *) va_arg(args,short *);
				*s = (short) simple_strtol(str,&next,base);
			} else {
				unsigned short *s = (unsigned short *) va_arg(args, unsigned short *);
				*s = (unsigned short) simple_strtoul(str, &next, base);
			}
			break;
		case 'l':
			if (is_sign) {
				long *l = (long *) va_arg(args,long *);
				*l = simple_strtol(str,&next,base);
			} else {
				unsigned long *l = (unsigned long*) va_arg(args,unsigned long*);
				*l = simple_strtoul(str,&next,base);
			}
			break;
		case 'L':
			if (is_sign) {
				long long *l = (long long*) va_arg(args,long long *);
				*l = simple_strtoll(str,&next,base);
			} else {
				unsigned long long *l = (unsigned long long*) va_arg(args,unsigned long long*);
				*l = simple_strtoull(str,&next,base);
			}
			break;
		case 'Z':
		{
			size_t *s = (size_t*) va_arg(args,size_t*);
			*s = (size_t) simple_strtoul(str,&next,base);
		}
		break;
		default:
			if (is_sign) {
				int *i = (int *) va_arg(args, int*);
				*i = (int) simple_strtol(str,&next,base);
			} else {
				unsigned int *i = (unsigned int*) va_arg(args, unsigned int*);
				*i = (unsigned int) simple_strtoul(str,&next,base);
			}
			break;
		}
		num++;

		if (!next)
			break;
		str = next;
	}
	return num;
} // end of the function


int sscanf(const char * buf, const char * fmt, ...)
{
	va_list args;
	int i;

	va_start(args,fmt);
	i = vsscanf(buf,fmt,args);
	va_end(args);
	return i;
} // end of the functino


//==========================================================
// END() - VSPRINTF.C
//==========================================================




