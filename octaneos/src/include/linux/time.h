/*
 * time.h
 *
 * Copyright (C) 2003, 2007 Berlin Brown
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _OCTANE_TIME_H
#define _OCTANE_TIME_H

//**********************************************************
//
// Timer Defines
//
//**********************************************************
#define BLANK_TIMER	0
#define BEEP_TIMER	1
#define RS_TIMER	2
#define HD_TIMER	16
#define FLOPPY_TIMER	17
#define SCSI_TIMER 	18
#define NET_TIMER	19
#define SOUND_TIMER	20
#define COPRO_TIMER	21
#define TAPE_QIC02_TIMER	22
#define MCD_TIMER	23
#define HD_TIMER2	24

struct timer_struct {
	unsigned long expires;
	void (*fn)(void);
};

extern unsigned long timer_active;
extern struct timer_struct timer_table[32];

/*
 * This is completely separate from the above, and is the
 * "new and improved" way of handling timers more dynamically.
 * Hopefully efficient and general enough for most things.
 *
 * The "hardcoded" timers above are still useful for well-
 * defined problems, but the timer-list is probably better
 * when you need multiple outstanding timers or similar.
 *
 * The "data" field is in case you want to use the same
 * timeout function for several timeouts. You can use this
 * to distinguish between the different invocations.
 */
struct timer_list {
	struct timer_list *next;
	struct timer_list *prev;
	unsigned long expires;
	unsigned long data;
	void (*function)(unsigned long);
};


struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
};

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};

#define NFDBITS			__NFDBITS

#define FD_SETSIZE		    __FD_SETSIZE
#define FD_SET(fd,fdsetp)	__FD_SET(fd,fdsetp)
#define FD_CLR(fd,fdsetp)	__FD_CLR(fd,fdsetp)
#define FD_ISSET(fd,fdsetp)	__FD_ISSET(fd,fdsetp)
#define FD_ZERO(fdsetp)		__FD_ZERO(fdsetp)

/*
 * Names of the interval timers, and structure
 * defining a timer setting.
 */
#define	ITIMER_REAL	    0
#define	ITIMER_VIRTUAL	1
#define	ITIMER_PROF	    2

struct	itimerval {
	struct	timeval it_interval;
	struct	timeval it_value;
};

#define HD_TIMER2	24

/*
 * phase-lock loop variables
 */
extern int time_status;		/* clock synchronization status */
extern long time_offset;	/* time adjustment (us) */
extern long time_constant;	/* pll time constant */
extern long time_tolerance;	/* frequency tolerance (ppm) */
extern long time_precision;	/* clock precision (us) */
extern long time_maxerror;	/* maximum error */
extern long time_esterror;	/* estimated error */
extern long time_phase;		/* phase offset (scaled us) */
extern long time_freq;		/* frequency offset (scaled ppm) */
extern long time_adj;		/* tick adjust (scaled 1 / HZ) */
extern long time_reftime;	/* time at last adjustment (s) */

extern long time_adjust;	/* The amount of adjtime left */

extern long tick;                      /* timer interrupt period */
extern int tickadj;			/* amount of adjustment per tick */
extern volatile struct timeval xtime;		/* The current time */

/*
 * Mode codes (timex.mode) 
 */
#define ADJ_OFFSET		0x0001	/* time offset */
#define ADJ_FREQUENCY		0x0002	/* frequency offset */
#define ADJ_MAXERROR		0x0004	/* maximum time error */
#define ADJ_ESTERROR		0x0008	/* estimated time error */
#define ADJ_STATUS		0x0010	/* clock status */
#define ADJ_TIMECONST		0x0020	/* pll time constant */
#define ADJ_TICK		0x4000	/* tick value */
#define ADJ_OFFSET_SINGLESHOT	0x8001	/* old-fashioned adjtime */

#define SHIFT_HZ 7		/* log2(HZ) */

#define SHIFT_KG 8		/* shift for phase increment */
#define SHIFT_KF 20		/* shift for frequency increment */
#define MAXTC 6			/* maximum time constant (shift) */

#define SHIFT_SCALE 24		/* shift for phase scale factor */
#define SHIFT_UPDATE (SHIFT_KG + MAXTC) /* shift for offset scale factor */
#define FINEUSEC (1 << SHIFT_SCALE) /* 1 us in scaled units */

#define MAXPHASE 128000         /* max phase error (us) */
#define MAXFREQ 100             /* max frequency error (ppm) */
#define MINSEC 16               /* min interval between updates (s) */
#define MAXSEC 1200             /* max interval between updates (s) */

#define CLOCK_TICK_RATE	1193180 /* Underlying HZ */
#define CLOCK_TICK_FACTOR	20	/* Factor of both 1000000 and CLOCK_TICK_RATE */
#define LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)	/* For divider */

#define FINETUNE (((((LATCH * HZ - CLOCK_TICK_RATE) << SHIFT_HZ) * \
	(1000000/CLOCK_TICK_FACTOR) / (CLOCK_TICK_RATE/CLOCK_TICK_FACTOR)) \
		<< (SHIFT_SCALE-SHIFT_HZ)) / HZ)

#define TIME_OK		0	/* clock synchronized */
#define TIME_INS	1	/* insert leap second */
#define TIME_DEL	2	/* delete leap second */
#define TIME_OOP	3	/* leap second in progress */
#define TIME_BAD	4	/* clock not synchronized */


/*
 * syscall interface - used (mainly by NTP daemon)
 * to discipline kernel clock oscillator
 */
struct timex {
	int mode;		/* mode selector */
	long offset;		/* time offset (usec) */
	long frequency;		/* frequency offset (scaled ppm) */
	long maxerror;		/* maximum error (usec) */
	long esterror;		/* estimated error (usec) */
	int status;		/* clock command/status */
	long time_constant;	/* pll time constant */
	long precision;		/* clock precision (usec) (read only) */
	long tolerance;		/* clock frequency tolerance (ppm)
				 * (read only)
				 */
	struct timeval time;	/* (read only) */
	long tick;		/* (modified) usecs between clock ticks */
};


#endif
