//
// Copyright (C) 2003, 2007 Berlin Brown
//
// $Id: misc_print.c,v 1.4 2005/05/26 00:06:53 bigbinc Exp $
//
// Printing API for debugging
//
// This should be library independent
// you need to link this with any other object
//
// needed for va stuff, have faith in GNU
//
#include <stdarg.h>

#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif


#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define _U      0x01
#define _L      0x02
#define _D      0x04
#define _C      0x08
#define _P      0x10
#define _S      0x20
#define _X      0x40
#define _SP     0x80


typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;

static char *vidmem = (char *)0xb8000;
static int vidport;
static int lines, cols;


static void *__memcpy(void *, __const void *, size_t);

unsigned char  __orig_x;
unsigned char  __orig_y;

unsigned short __orig_video_page;
unsigned char  __orig_video_mode;
unsigned char  __orig_video_cols;
unsigned char  __orig_video_lines;

#define ZEROPAD	1
#define SIGN	2
#define PLUS	4
#define SPACE	8
#define LEFT	16
#define SPECIAL	32
#define LARGE	64


#define INT_MAX                ((int)(~0U>>1))
#define INT_MIN                (-INT_MAX - 1)

// ==========## IO Code ##==================================

#define __SLOW_DOWN_IO "\noutb %%al,$0x80"
#define SLOW_DOWN_IO __SLOW_DOWN_IO
#define __FULL_SLOW_DOWN_IO __SLOW_DOWN_IO     

#define __OUT1(s,x) \
static inline void __out##s(unsigned x value, unsigned short port) {

#define __OUT2(s,s1,s2) \
__asm__ __volatile__ ("out" #s " %" s1 "0,%" s2 "1"	

#define __OUT(s,s1,x) \
__OUT1(s,x) __OUT2(s,s1,"w") : : "a" (value), "Nd" (port)); } \
	__OUT1(s##_p,x) __OUT2(s,s1,"w") __FULL_SLOW_DOWN_IO : : "a" (value), "Nd" (port));}


#define __IN1(s) \
	static inline RETURN_TYPE __in##s(unsigned short port) { RETURN_TYPE _v;

#define __IN2(s,s1,s2) \
		__asm__ __volatile__ ("in" #s " %" s2 "1,%" s1 "0"



#define __IN(s,s1,i...) \
__IN1(s) __IN2(s,s1,"w") : "=a" (_v) : "Nd" (port) ,##i ); return _v; } \
__IN1(s##_p) __IN2(s,s1,"w") __FULL_SLOW_DOWN_IO : "=a" (_v) : "Nd" (port) ,##i ); return _v; }

#define __INS(s) \
static inline void __ins##s(unsigned short port, void * addr, unsigned long count) \
{ __asm__ __volatile__ ("rep ; ins" #s \
: "=D" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }

#define __OUTS(s) \
static inline void __outs##s(unsigned short port, const void * addr, unsigned long count) \
{ __asm__ __volatile__ ("rep ; outs" #s \
: "=S" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }



#define RETURN_TYPE unsigned char
__IN(b,"")
#undef RETURN_TYPE
#define RETURN_TYPE unsigned short
__IN(w,"")
#undef RETURN_TYPE
#define RETURN_TYPE unsigned int
__IN(l,"")
#undef RETURN_TYPE


__OUT(b,"b",char)
__OUT(w,"w",short)
__OUT(l,,int)

__INS(b)
__INS(w)
__INS(l)

__OUTS(b)
__OUTS(w)
__OUTS(l)


// ==========++ IO Code ++=====================


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

extern void __puts(const char *);
extern int __sprintf(char * buf, const char *fmt, ...);

static char *__number(char *, char *, long long, int, int, int, int);
static int __sscanf(const char *, const char *, ...);
static int __vsscanf(const char *, const char *, va_list);
static int __skip_atoi(const char **s);

//==========================================================
//
// Public
//
//==========================================================
void load_misc_kprint(void)
{

  char buf[80];

  __orig_x = 0;
  __orig_y = 23;

  __orig_video_page = 0;
  __orig_video_mode = 0;
  __orig_video_cols = 80;
  __orig_video_lines = 25;
  
  vidmem = (char *) 0xb8000;
  vidport = 0x3d4;
  
  lines = __orig_video_lines;
  cols = __orig_video_cols;
 
  __sprintf(buf, "[debug kprinting loaded]\n"); __puts(buf);
 
}



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


//==========================================================
//
// START() - VSPRINTF.C - 
//
//==========================================================

static void __scroll(void)
{
  int i;

  __memcpy ( vidmem, vidmem + cols * 2, ( lines - 1 ) * cols * 2 );

  for ( i = ( lines - 1 ) * cols * 2; i < lines * cols * 2; i += 2 )
    vidmem[i] = ' ';
} // end of the functino 

void __puts(const char *s)
{
  int x,y,pos;
  char c;

  x = (int)__orig_x;
  y = (int)__orig_y;

  while ( ( c = *s++ ) != '\0' ) {
    if ( c == '\n' ) {
      x = 0;
      if ( ++y >= lines ) {
	__scroll();
	y--;
      }
    } else {
      vidmem [ ( x + cols * y ) * 2 ] = c; 
      if ( ++x >= cols ) {
	x = 0;
	if ( ++y >= lines ) {
	  __scroll();
	  y--;
	}
      }
    }
  }

  __orig_x = x;
  __orig_y = y;

  pos = (x + cols * y) * 2;	/* Update cursor position */
  __outb_p(14, vidport);
  __outb_p(0xff & (pos >> 9), vidport+1);
  __outb_p(15, vidport);
  __outb_p(0xff & (pos >> 1), vidport+1);

}

static void* __memset(void* s, int c, size_t n)
{
  int i;
  char *ss = (char*)s;

  for (i=0;i<n;i++) ss[i] = c;
  return s;

}

static void* __memcpy(void* __dest, __const void* __src, size_t __n)
{
  int i;
  char *d = (char *)__dest, *s = (char *)__src;

  for (i=0;i<__n;i++) d[i] = s[i];
  return __dest;

}

static unsigned long __simple_strtoul(const char *cp,char **endp,unsigned int base)
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

}

static long __simple_strtol(const char *cp,char **endp,unsigned int base)
{

	if(*cp=='-')
	  return -__simple_strtoul(cp+1,endp,base);

	return __simple_strtoul(cp,endp,base);

}


static unsigned long long __simple_strtoull(const char *cp,char **endp,unsigned int base)
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
}

static long long __simple_strtoll(const char *cp,char **endp,unsigned int base)
{
  if(*cp=='-')
    return -__simple_strtoull(cp+1,endp,base);
  return __simple_strtoull(cp,endp,base);
}

static int __skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
} // end of the function

static char *__number(char * buf, char * end, long long num, int base, int size, int precision, int type)
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
	if (type & SPECIAL)
	  {
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
	if (!(type & LEFT))
	  {
	    while (size-- > 0)
	      {
		if (buf <= end)
		  *buf = c;
		++buf;
	      }
	  }

	while (i < precision--)
	  {
		if (buf <= end)
			*buf = '0';
		++buf;
	  }

	while (i-- > 0)
	  {
	    if (buf <= end)
	      *buf = tmp[i];
	    ++buf;
	  }
	
	while (size-- > 0)
	  {
	    if (buf <= end)
	      *buf = ' ';
	    ++buf;
	  }

	return buf;

}


static int __vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
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
			field_width = __skip_atoi(&fmt);
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
				precision = __skip_atoi(&fmt);
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
				str = __number(str, end,
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
		str = __number(str, end, num, base,
				field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		*end = '\0';

	return str-buf;

} // end of the function ++

static int __snprintf(char * buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=__vsnprintf(buf,size,fmt,args);
	va_end(args);
	return i;
} // end of the functionx


static int __vsprintf(char *buf, const char *fmt, va_list args)
{
	return __vsnprintf(buf, 0xFFFFFFFFUL, fmt, args);
} // end of the functino

int __sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=__vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}

static int __vsscanf(const char * buf, const char * fmt, va_list args)
{
	const char *str = buf;
	char *next;
	int num = 0;
	int qualifier;
	int base;
	int field_width = -1;
	int is_sign = 0;

	while(*fmt && *str)
	  {
		
		if (isspace(*fmt)) {
			while (isspace(*fmt))
				++fmt;
			while (isspace(*str))
				++str;
		}

		/* anything that is not a conversion must match exactly */
		if (*fmt != '%' && *fmt)
		  {
			if (*fmt++ != *str++)
				break;
			continue;
		  }

		if (!*fmt)
			break;
		++fmt;
		
		if (*fmt == '*')
		  {
			while (!isspace(*fmt) && *fmt)
				fmt++;
			while (!isspace(*str) && *str)
				str++;
			continue;
		  }

		/* get field width */
		if (isdigit(*fmt))
			field_width = __skip_atoi(&fmt);

		/* get conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z')
		  {
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
				*s = (short) __simple_strtol(str,&next,base);
			} else {
				unsigned short *s = (unsigned short *) va_arg(args, unsigned short *);
				*s = (unsigned short) __simple_strtoul(str, &next, base);
			}
			break;
		case 'l':
			if (is_sign) {
				long *l = (long *) va_arg(args,long *);
				*l = __simple_strtol(str,&next,base);
			} else {
				unsigned long *l = (unsigned long*) va_arg(args,unsigned long*);
				*l = __simple_strtoul(str,&next,base);
			}
			break;
		case 'L':
			if (is_sign) {
				long long *l = (long long*) va_arg(args,long long *);
				*l = __simple_strtoll(str,&next,base);
			} else {
				unsigned long long *l = (unsigned long long*) va_arg(args,unsigned long long*);
				*l = __simple_strtoull(str,&next,base);
			}
			break;
		case 'Z':
		{
			size_t *s = (size_t*) va_arg(args,size_t*);
			*s = (size_t) __simple_strtoul(str,&next,base);
		}
		break;
		default:
			if (is_sign) {
				int *i = (int *) va_arg(args, int*);
				*i = (int) __simple_strtol(str,&next,base);
			} else {
				unsigned int *i = (unsigned int*) va_arg(args, unsigned int*);
				*i = (unsigned int) __simple_strtoul(str,&next,base);
			}
			break;
		}
		num++;
		
		if (!next)
		  break;
		str = next;
	  }
	return num;
}


static int __sscanf(const char * buf, const char * fmt, ...)
{
	va_list args;
	int i;

	va_start(args,fmt);
	i = __vsscanf(buf,fmt,args);
	va_end(args);
	return i;
}

//==========================================================
//
// END() - VSPRINTF.C - 
//
//==========================================================




