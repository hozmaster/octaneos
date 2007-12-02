/*---------------------------------------------------------------------------+
 |  fpu_emu.h                                                                |
 |                                                                           |
 | Copyright (C) 1992,1993,1994                                              |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail   billm@vaxc.cc.monash.edu.au    |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#ifndef _OCTANE_FPU_EMU_H_
#define _OCTANE_FPU_EMU_H_

struct fpu_reg {
	char sign;
	char tag;
	long exp;
	unsigned sigl;
	unsigned sigh;
};

/* This structure matches the layout of the data saved to the stack
   following a device-not-present interrupt, part of it saved
   automatically by the 80386/80486.
   */
struct info {
	long ___orig_eip;
	long ___ret_from_system_call;
	long ___ebx;
	long ___ecx;
	long ___edx;
	long ___esi;
	long ___edi;
	long ___ebp;
	long ___eax;
	long ___ds;
	long ___es;
	long ___fs;
	long ___gs;
	long ___orig_eax;
	long ___eip;
	long ___cs;
	long ___eflags;
	long ___esp;
	long ___ss;
	long ___vm86_es;
	long ___vm86_ds;
	long ___vm86_fs;
	long ___vm86_gs;
};

/*
 * Define DENORM_OPERAND to make the emulator detect denormals
 * and use the denormal flag of the status word. Note: this only
 * affects the flag and corresponding interrupt, the emulator
 * will always generate denormals and operate upon them as required.
 */
#define DENORM_OPERAND

/*
 * Define PECULIAR_486 to get a closer approximation to 80486 behaviour,
 * rather than behaviour which appears to be cleaner.
 * This is a matter of opinion: for all I know, the 80486 may simply
 * be complying with the IEEE spec. Maybe one day I'll get to see the
 * spec...
 */
#define PECULIAR_486

#ifdef __ASSEMBLER__
#include "fpu_asm.h"
#define	Const(x)	$##x
#else
#define	Const(x)	x
#endif

#define EXP_BIAS	Const(0)
#define EXP_OVER	Const(0x4000)    /* smallest invalid large exponent */
#define	EXP_UNDER	Const(-0x3fff)   /* largest invalid small exponent */
#define EXP_Infinity    EXP_OVER
#define EXP_NaN         EXP_OVER

#define SIGN_POS	Const(0)
#define SIGN_NEG	Const(1)

/* Keep the order TW_Valid, TW_Zero, TW_Denormal */
#define TW_Valid	Const(0)	/* valid */
#define TW_Zero		Const(1)	/* zero */
/* The following fold to 2 (Special) in the Tag Word */
/* #define TW_Denormal     Const(4) */       /* De-normal */
#define TW_Infinity	Const(5)	/* + or - infinity */
#define	TW_NaN		Const(6)	/* Not a Number */

#define TW_Empty	Const(7)	/* empty */

#endif
