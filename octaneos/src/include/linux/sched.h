/*
 * Octane OS (Operating System)
 * Copyright (C) 2007 Berlin Brown
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _OCTANE_SCHED_H
#define _OCTANE_SCHED_H

#include <linux/time.h>
#include <linux/tasks.h>
#include <linux/fpu_emu.h>
#include <system/octane_types.h>
#include <linux/wait.h>

#define NEW_SWAP

typedef struct desc_struct {
	unsigned long a,b;
} desc_table[256];

extern unsigned long swapper_pg_dir[1024];
extern desc_table idt, gdt;

#define GDT_NUL 0
#define GDT_CODE 1
#define GDT_DATA 2
#define GDT_TMP 3

#define LDT_NUL 0
#define LDT_CODE 1
#define LDT_DATA 2

struct bh_struct {
	void (*routine)(void *);
	void *data;
};

extern unsigned long bh_active;
extern unsigned long bh_mask;
extern struct bh_struct bh_base[32];

/* Who gets which entry in bh_base.  Things which will occur most often
   should come first - in which case NET should be up the top with SERIAL/TQUEUE! */
   
enum {
	TIMER_BH = 0,
	CONSOLE_BH,
	TQUEUE_BH,
	SERIAL_BH,
	NET_BH,
	KEYBOARD_BH
};

extern inline void mark_bh(int nr)
{
	__asm__ __volatile__("orl %1,%0":"=m" (bh_active):"ir" (1<<nr));
}

extern inline void disable_bh(int nr)
{
	__asm__ __volatile__("andl %1,%0":"=m" (bh_mask):"ir" (~(1<<nr)));
}

extern inline void enable_bh(int nr)
{
	__asm__ __volatile__("orl %1,%0":"=m" (bh_mask):"ir" (1<<nr));
}


//************************************************
// signal defines
//************************************************
typedef unsigned int sigset_t;

#define _NSIG             32
#define NSIG	 	      _NSIG

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGUNUSED	 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22

#define SIGIO		23
#define SIGPOLL		SIGIO
#define SIGURG		SIGIO
#define SIGXCPU		24
#define SIGXFSZ		25

#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28

#define SIGPWR		30

#define SIGBUS		SIGUNUSED

/*
 * sa_flags values: SA_STACK is not currently supported, but will allow the
 * usage of signal stacks by using the (now obsolete) sa_restorer field in
 * the sigaction structure as a stack pointer. This is now possible due to
 * the changes in signal handling. LBT 010493.
 * SA_INTERRUPT is a no-op, but left due to historical reasons. Use the
 * SA_RESTART flag to get restarting signals (which were the default long ago)
 */
#define SA_NOCLDSTOP	1
#define SA_STACK	    0x08000000
#define SA_RESTART	    0x10000000
#define SA_INTERRUPT	0x20000000
#define SA_NOMASK	    0x40000000
#define SA_ONESHOT	    0x80000000

#define SIG_BLOCK          0	/* for blocking signals */
#define SIG_UNBLOCK        1	/* for unblocking signals */
#define SIG_SETMASK        2	/* for setting the signal mask */

/* Type of a signal handler.  */
typedef void (*__sighandler_t)(int);

#define SIG_DFL	((__sighandler_t)0)	/* default signal handling */
#define SIG_IGN	((__sighandler_t)1)	/* ignore signal */
#define SIG_ERR	((__sighandler_t)-1)	/* error return from signal */

struct sigaction {
	__sighandler_t sa_handler;
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
};

struct sigcontext_struct {
	unsigned short gs, __gsh;
	unsigned short fs, __fsh;
	unsigned short es, __esh;
	unsigned short ds, __dsh;
	unsigned long edi;
	unsigned long esi;
	unsigned long ebp;
	unsigned long esp;
	unsigned long ebx;
	unsigned long edx;
	unsigned long ecx;
	unsigned long eax;
	unsigned long trapno;
	unsigned long err;
	unsigned long eip;
	unsigned short cs, __csh;
	unsigned long eflags;
	unsigned long esp_at_signal;
	unsigned short ss, __ssh;
	unsigned long i387;
	unsigned long oldmask;
	unsigned long cr2;
};


//************************************************
// RESOURCE LIMIT DEFINES
//************************************************

#define	RUSAGE_SELF	0
#define	RUSAGE_CHILDREN	(-1)
#define RUSAGE_BOTH	(-2)		/* sys_wait4() uses this */

struct	rusage {
	struct timeval ru_utime;	/* user time used */
	struct timeval ru_stime;	/* system time used */
	long	ru_maxrss;		/* maximum resident set size */
	long	ru_ixrss;		/* integral shared memory size */
	long	ru_idrss;		/* integral unshared data size */
	long	ru_isrss;		/* integral unshared stack size */
	long	ru_minflt;		/* page reclaims */
	long	ru_majflt;		/* page faults */
	long	ru_nswap;		/* swaps */
	long	ru_inblock;		/* block input operations */
	long	ru_oublock;		/* block output operations */
	long	ru_msgsnd;		/* messages sent */
	long	ru_msgrcv;		/* messages received */
	long	ru_nsignals;	/* signals received */
	long	ru_nvcsw;		/* voluntary context switches */
	long	ru_nivcsw;		/* involuntary " */
};

/*
 * Resource limits
 */
#define RLIMIT_CPU	    0	/* CPU time in ms */
#define RLIMIT_FSIZE	1	/* Maximum filesize */
#define RLIMIT_DATA	    2	/* max data size */
#define RLIMIT_STACK	3	/* max stack size */
#define RLIMIT_CORE	    4	/* max core file size */
#define RLIMIT_RSS	    5	/* max resident set size */

#ifdef notdef
#define RLIMIT_MEMLOCK	6		/* max locked-in-memory address space*/
#define RLIMIT_NPROC	7		/* max number of processes */
#define RLIMIT_OFILE	8		/* max number of open files */
#endif

#define RLIM_NLIMITS	6

#define RLIM_INFINITY	0x7FFFFFFF

struct rlimit {
	int	rlim_cur;
	int	rlim_max;
};

#define	PRIO_MIN	    (-99)
#define	PRIO_MAX	    14
#define	PRIO_PROCESS	0
#define	PRIO_PGRP	    1
#define	PRIO_USER	    2

#define EXEC_PAGESIZE	4096

#ifndef NGROUPS
#define NGROUPS		32
#endif

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif


/*
 * define DEBUG if you want the wait-queues to have some extra
 * debugging code. It's not normally used, but might catch some
 * wait-queue coding errors.
 *
 *  #define DEBUG
 */

#define HZ 100

/*
 * System setup flags..
 */
extern int hard_math;
extern int x86;
extern int ignore_irq13;
extern int wp_works_ok;

extern unsigned long intr_count;

#define start_bh_atomic() \
__asm__ __volatile__("incl _intr_count")

#define end_bh_atomic() \
__asm__ __volatile__("decl _intr_count")

/*
 * Bus types (default is ISA, but people can check others with these..)
 * MCA_bus hardcoded to 0 for now.
 */
extern int EISA_bus;
#define MCA_bus 0

/*
 * User space process size: 3GB. This is hardcoded into a few places,
 * so don't change it unless you know what you are doing.
 */
#define TASK_SIZE	0xc0000000

/*
 * Size of io_bitmap in longwords: 32 is ports 0-0x3ff.
 */
#define IO_BITMAP_SIZE	32

extern unsigned long avenrun[]; /* Load averages */

#define FSHIFT		11		    /* nr of bits of precision */
#define FIXED_1		(1<<FSHIFT)	/* 1.0 as fixed-point */
#define LOAD_FREQ	(5*HZ)		/* 5 sec intervals */
#define EXP_1		1884		/* 1/exp(5sec/1min) as fixed-point */
#define EXP_5		2014		/* 1/exp(5sec/5min) */
#define EXP_15		2037		/* 1/exp(5sec/15min) */

#define CALC_LOAD(load,exp,n) \
	load *= exp; \
	load += n*(FIXED_1-exp); \
	load >>= FSHIFT;

#define CT_TO_SECS(x)	((x) / HZ)
#define CT_TO_USECS(x)	(((x) % HZ) * 1000000/HZ)

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]

#define TASK_RUNNING		    0
#define TASK_INTERRUPTIBLE	    1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_ZOMBIE		        3
#define TASK_STOPPED		    4
#define TASK_SWAPPING		    5

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern void sched_init(void);
extern void show_state(void);
extern void trap_init(void);

asmlinkage void schedule(void);

struct i387_hard_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long	st_space[20];	/* 8*10 bytes for each FP-reg = 80 bytes */
};

struct i387_soft_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long    top;
	struct  fpu_reg	regs[8];
	unsigned char	lookahead;
	struct  info	*info;
	unsigned long	entry_eip;
};

union i387_union {
	struct i387_hard_struct hard;
	struct i387_soft_struct soft;
};

struct tss_struct {
	unsigned short	back_link,__blh;
	unsigned long	esp0;
	unsigned short	ss0,__ss0h;
	unsigned long	esp1;
	unsigned short	ss1,__ss1h;
	unsigned long	esp2;
	unsigned short	ss2,__ss2h;
	unsigned long	cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax,ecx,edx,ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es, __esh;
	unsigned short	cs, __csh;
	unsigned short	ss, __ssh;
	unsigned short	ds, __dsh;
	unsigned short	fs, __fsh;
	unsigned short	gs, __gsh;
	unsigned short	ldt, __ldth;
	unsigned short	trace, bitmap;
	unsigned long	io_bitmap[IO_BITMAP_SIZE+1];
	unsigned long	tr;
	unsigned long	cr2, trap_no, error_code;
	union i387_union i387;
};

#define INIT_TSS  {          \
	0,0,                     \
	sizeof(init_kernel_stack) + (long) &init_kernel_stack, \
	KERNEL_DS, 0,            \
	0,0,0,0,0,0,             \
	(long) &swapper_pg_dir,  \
	0,0,0,0,0,0,0,0,0,0,     \
	USER_DS,0,USER_DS,0,USER_DS,0,USER_DS,0,USER_DS,0,USER_DS,0, \
	_LDT(0),0,               \
	0, 0x8000,               \
	{~0, }, /* ioperm */     \
	_TSS(0), 0, 0,0,         \
	{ { 0, }, }  /* 387 state */ \
}

struct files_struct {
	int         count;
	fd_set      close_on_exec;
	// TODO: add file struct
	//struct file *fd[NR_OPEN];
};

#define INIT_FILES { \
	0,               \
	{ { 0, } },      \
	{ NULL, }        \
}

struct fs_struct {
	int count;
	unsigned short umask;
	struct inode * root, * pwd;
};

#define INIT_FS { \
	0,            \
	0022,         \
	NULL, NULL    \
}

struct mm_struct {
	int count;
	unsigned long start_code, end_code, end_data;
	unsigned long start_brk, brk, start_stack, start_mmap;
	unsigned long arg_start, arg_end, env_start, env_end;
	unsigned long rss;
	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
	int swappable:1;
#ifdef NEW_SWAP
	unsigned long old_maj_flt;  /* old value of maj_flt */
	unsigned long dec_flt;		/* page fault count of the last time */
	unsigned long swap_cnt;		/* number of pages to swap on next pass */
	short swap_table;		    /* current page table */
	short swap_page;		    /* current page */
#endif
	struct vm_area_struct * mmap;
};

#define INIT_MM { \
		0, \
		0, 0, 0, \
		0, 0, 0, 0, \
		0, 0, 0, 0, \
		0, \
/* ?_flt */	0, 0, 0, 0, \
		0, \
/* swap */	0, 0, 0, 0, 0, \
		NULL }

struct task_struct {
/* these are hardcoded - don't touch */
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;
	long priority;
	unsigned long signal;
	unsigned long blocked;	/* bitmap of masked signals */
	unsigned long flags;	/* per process flags, defined below */
	int errno;
	int debugreg[8];  /* Hardware debugging registers */
	struct exec_domain *exec_domain;
/* various fields */
	struct linux_binfmt *binfmt;
	struct task_struct *next_task, *prev_task;
	struct sigaction sigaction[32];
	unsigned long saved_kernel_stack;
	unsigned long kernel_stack_page;
	int exit_code, exit_signal;
	unsigned long personality;
	int dumpable:1;
	int did_exec:1;
	volatile int swapping:1;
	int pid,pgrp,session,leader;
	int	groups[NGROUPS];
	/* 
	 * pointers to (original) parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->p_pptr->pid)
	 */
	struct task_struct *p_opptr, *p_pptr, *p_cptr, *p_ysptr, *p_osptr;
	struct wait_queue *wait_chldexit;	/* for wait4() */
	unsigned short uid,euid,suid;
	unsigned short gid,egid,sgid;
	unsigned long timeout;
	unsigned long it_real_value, it_prof_value, it_virt_value;
	unsigned long it_real_incr, it_prof_incr, it_virt_incr;
	long utime, stime, cutime, cstime, start_time;
	struct rlimit rlim[RLIM_NLIMITS]; 
	unsigned short used_math;
	char comm[16];
/* virtual 86 mode stuff */
	struct vm86_struct * vm86_info;
	unsigned long screen_bitmap;
	unsigned long v86flags, v86mask, v86mode;
/* file system info */
	int link_count;
	struct tty_struct *tty; /* NULL if no tty */
	struct inode * executable;
	struct shm_desc *shm;
	struct sem_undo *semun;
/* ldt for this task - used by Wine.  If NULL, default_ldt is used */
	struct desc_struct *ldt;
/* tss for this task */
	struct tss_struct tss;
/* filesystem information */
	struct fs_struct fs[1];
/* open file information */
	struct files_struct files[1];
/* memory management info */
	struct mm_struct mm[1];
};

/*
 * Per process flags
 */
#define PF_ALIGNWARN	0x00000001	/* Print alignment warning msgs */
					/* Not implemented yet, only for 486*/
#define PF_PTRACED	0x00000010	/* set if ptrace (0) has been called. */
#define PF_TRACESYS	0x00000020	/* tracing system calls */

/*
 * cloning flags:
 */
#define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
#define COPYVM		0x00000100	/* set if VM copy desired (like normal fork()) */
#define COPYFD		0x00000200	/* set if fd's should be copied, not shared (NI) */

/*
 * INIT_TASK is used to set up the first task table,
 * Base=0, limit=0x1fffff (=2MB)
 */
#define INIT_TASK                      \
/* state etc */	{ 0,15,15,0,0,0,0,     \
/* debugregs */ { 0, },                \
/* exec domain */&default_exec_domain, \
/* binfmt */	NULL,                  \
/* schedlink */	&init_task,&init_task, \
/* signals */	{{ 0, },},             \
/* stack */	0,(unsigned long) &init_kernel_stack,     \
/* ec,brk... */	0,0,0,0,0,0,           \
/* pid etc.. */	0,0,0,0,               \
/* suppl grps*/ {NOGROUP,},            \
/* proc links*/ &init_task,&init_task,NULL,NULL,NULL,NULL,     \
/* uid etc */	0,0,0,0,0,0,           \
/* timeout */	0,0,0,0,0,0,0,0,0,0,0,0, \
/* rlimits */   { {LONG_MAX, LONG_MAX}, {LONG_MAX, LONG_MAX},  \
		  {LONG_MAX, LONG_MAX}, {LONG_MAX, LONG_MAX},  \
		  {       0, LONG_MAX}, {LONG_MAX, LONG_MAX}}, \
/* math */	0,                         \
/* comm */	"swapper",                 \
/* vm86_info */	NULL, 0, 0, 0, 0,      \
/* fs info */	0,NULL,NULL,           \
/* ipc */	NULL, NULL,                \
/* ldt */	NULL,                      \
/* tss */	INIT_TSS,                  \
/* fs */	{ INIT_FS },               \
/* files */	{ INIT_FILES },            \
/* mm */	{ INIT_MM }                \
}

extern struct task_struct init_task;
extern struct task_struct *task[NR_TASKS];
extern struct task_struct *last_task_used_math;
extern struct task_struct *current;
extern unsigned long volatile jiffies;
extern unsigned long itimer_ticks;
extern unsigned long itimer_next;

extern int need_resched;

#define CURRENT_TIME (xtime.tv_sec)

extern void sleep_on(struct wait_queue **p);
extern void interruptible_sleep_on(struct wait_queue ** p);
extern void wake_up(struct wait_queue ** p);
extern void wake_up_interruptible(struct wait_queue ** p);

extern void notify_parent(struct task_struct * tsk);
extern int send_sig(unsigned long sig,struct task_struct * p,int priv);
extern int in_group_p(gid_t grp);

extern int request_irq(unsigned int irq,void (*handler)(int));
extern void free_irq(unsigned int irq);
extern int irqaction(unsigned int irq,struct sigaction * sa);

/*
 * Entry into gdt where to find first TSS. GDT layout:
 *   0 - nul
 *   1 - kernel code segment
 *   2 - kernel data segment
 *   3 - user code segment
 *   4 - user data segment
 * ...
 *   8 - TSS #0
 *   9 - LDT #0
 *  10 - TSS #1
 *  11 - LDT #1
 */
#define FIRST_TSS_ENTRY 8
#define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
#define _TSS(n) ((((unsigned long) n)<<4)+(FIRST_TSS_ENTRY<<3))
#define _LDT(n) ((((unsigned long) n)<<4)+(FIRST_LDT_ENTRY<<3))
#define load_TR(n) __asm__("ltr %%ax": /* no output */ :"a" (_TSS(n)))
#define load_ldt(n) __asm__("lldt %%ax": /* no output */ :"a" (_LDT(n)))
#define store_TR(n) \
__asm__("str %%ax\n\t" \
	"subl %2,%%eax\n\t" \
	"shrl $4,%%eax" \
	:"=a" (n) \
	:"0" (0),"i" (FIRST_TSS_ENTRY<<3))
/*
 *	switch_to(n) should switch tasks to task nr n, first
 * checking that n isn't the current task, in which case it does nothing.
 * This also clears the TS-flag if the task we switched to has used
 * tha math co-processor latest.
 */
#define switch_to(tsk) \
__asm__("cli\n\t" \
	"xchgl %%ecx,_current\n\t" \
	"ljmp %0\n\t" \
	"sti\n\t" \
	"cmpl %%ecx,_last_task_used_math\n\t" \
	"jne 1f\n\t" \
	"clts\n" \
	"1:" \
	: /* no output */ \
	:"m" (*(((char *)&tsk->tss.tr)-4)), \
	 "c" (tsk) \
	:"cx")

#define _set_base(addr,base) \
__asm__("movw %%dx,%0\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %%dl,%1\n\t" \
	"movb %%dh,%2" \
	: /* no output */ \
	:"m" (*((addr)+2)), \
	 "m" (*((addr)+4)), \
	 "m" (*((addr)+7)), \
	 "d" (base) \
	:"dx")

#define _set_limit(addr,limit) \
__asm__("movw %%dx,%0\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %1,%%dh\n\t" \
	"andb $0xf0,%%dh\n\t" \
	"orb %%dh,%%dl\n\t" \
	"movb %%dl,%1" \
	: /* no output */ \
	:"m" (*(addr)), \
	 "m" (*((addr)+6)), \
	 "d" (limit) \
	:"dx")

#define set_base(ldt,base) _set_base( ((char *)&(ldt)) , base )
#define set_limit(ldt,limit) _set_limit( ((char *)&(ldt)) , (limit-1)>>12 )

/*
 * The wait-queues are circular lists, and you have to be *very* sure
 * to keep them correct. Use only these two functions to add/remove
 * entries in the queues.
 */
extern inline void add_wait_queue(struct wait_queue ** p, struct wait_queue * wait)
{
	unsigned long flags;

#ifdef DEBUG
	if (wait->next) {
		unsigned long pc;
		__asm__ __volatile__("call 1f\n"
			"1:\tpopl %0":"=r" (pc));
		printk("add_wait_queue (%08x): wait->next = %08x\n",pc,(unsigned long) wait->next);
	}
#endif
	save_flags(flags);
	cli();
	if (!*p) {
		wait->next = wait;
		*p = wait;
	} else {
		wait->next = (*p)->next;
		(*p)->next = wait;
	}
	restore_flags(flags);
}

extern inline void remove_wait_queue(struct wait_queue ** p, struct wait_queue * wait)
{
	unsigned long flags;
	struct wait_queue * tmp;
#ifdef DEBUG
	unsigned long ok = 0;
#endif

	save_flags(flags);
	cli();
	if ((*p == wait) &&
#ifdef DEBUG
	    (ok = 1) &&
#endif
	    ((*p = wait->next) == wait)) {
		*p = NULL;
	} else {
		tmp = wait;
		while (tmp->next != wait) {
			tmp = tmp->next;
#ifdef DEBUG
			if (tmp == *p)
				ok = 1;
#endif
		}
		tmp->next = wait->next;
	}
	wait->next = NULL;
	restore_flags(flags);
#ifdef DEBUG
	if (!ok) {
		printk("removed wait_queue not on list.\n");
		printk("list = %08x, queue = %08x\n",(unsigned long) p, (unsigned long) wait);
		__asm__("call 1f\n1:\tpopl %0":"=r" (ok));
		printk("eip = %08x\n",ok);
	}
#endif
}

extern inline void select_wait(struct wait_queue ** wait_address, select_table * p)
{
	struct select_table_entry *entry;

	if (!p || !wait_address)
		return;
	if (p->nr >= __MAX_SELECT_TABLE_ENTRIES)
		return;
 	entry = p->entry + p->nr;
	entry->wait_address = wait_address;
	//entry->wait.task = current;
	entry->wait.next = NULL;
	add_wait_queue(wait_address,&entry->wait);
	p->nr++;
}

extern void __down(struct semaphore * sem);

/*
 * These are not yet interrupt-safe
 */
extern inline void down(struct semaphore * sem)
{
	if (sem->count <= 0)
		__down(sem);
	sem->count--;
}

extern inline void up(struct semaphore * sem)
{
	sem->count++;
	wake_up(&sem->wait);
}	

static inline unsigned long _get_base(char * addr)
{
	unsigned long __base;
	__asm__("movb %3,%%dh\n\t"
		"movb %2,%%dl\n\t"
		"shll $16,%%edx\n\t"
		"movw %1,%%dx"
		:"=&d" (__base)
		:"m" (*((addr)+2)),
		 "m" (*((addr)+4)),
		 "m" (*((addr)+7)));
	return __base;
}

#define get_base(ldt) _get_base( ((char *)&(ldt)) )

static inline unsigned long get_limit(unsigned long segment)
{
	unsigned long __limit;
	__asm__("lsll %1,%0"
		:"=r" (__limit):"r" (segment));
	return __limit+1;
}

#define REMOVE_LINKS(p) do { unsigned long flags; \
	save_flags(flags) ; cli(); \
	(p)->next_task->prev_task = (p)->prev_task; \
	(p)->prev_task->next_task = (p)->next_task; \
	restore_flags(flags); \
	if ((p)->p_osptr) \
		(p)->p_osptr->p_ysptr = (p)->p_ysptr; \
	if ((p)->p_ysptr) \
		(p)->p_ysptr->p_osptr = (p)->p_osptr; \
	else \
		(p)->p_pptr->p_cptr = (p)->p_osptr; \
	} while (0)

#define SET_LINKS(p) do { unsigned long flags; \
	save_flags(flags); cli(); \
	(p)->next_task = &init_task; \
	(p)->prev_task = init_task.prev_task; \
	init_task.prev_task->next_task = (p); \
	init_task.prev_task = (p); \
	restore_flags(flags); \
	(p)->p_ysptr = NULL; \
	if (((p)->p_osptr = (p)->p_pptr->p_cptr) != NULL) \
		(p)->p_osptr->p_ysptr = p; \
	(p)->p_pptr->p_cptr = p; \
	} while (0)

#define for_each_task(p) \
	for (p = &init_task ; (p = p->next_task) != &init_task ; )

/*
 * This is the ldt that every process will get unless we need
 * something other than this.
 */
extern struct desc_struct default_ldt;

#define loaddebug(register)                    \
		__asm__("movl %0,%%edx\n\t"            \
			"movl %%edx,%%db" #register "\n\t" \
			: /* no output */                  \
			:"m" (current->debugreg[register]) \
			:"dx");

// End of Sched.h
#endif
