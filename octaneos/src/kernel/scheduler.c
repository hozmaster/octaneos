/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: scheduler.c
 *
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
 *
 * See LICENSE.OCTANE for more details
 */

#include <system/system.h>		    // super-header
#include <system/system_calls.h>	// extern function call list
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/page.h>
#include <linux/vm86.h>
#include <linux/errno.h>
#include <linux/kernel_stat.h>
#include <linux/ptrace.h>

#include <asm/io.h>
#include <linux/task_queue.h>

#define TIMER_IRQ               0
#define TIMER_LIST_REQUESTS     64

int time_status = TIME_BAD;     /* clock synchronization status */
long time_offset = 0;           /* time adjustment (us) */
long time_constant = 0;         /* pll time constant */
long time_tolerance = MAXFREQ;  /* frequency tolerance (ppm) */
long time_precision = 1; 	    /* clock precision (us) */
long time_maxerror = 0x70000000;/* maximum error */
long time_esterror = 0x70000000;/* estimated error */
long time_phase = 0;            /* phase offset (scaled us) */
long time_freq = 0;             /* frequency offset (scaled ppm) */
long time_adj = 0;              /* tick adjust (scaled 1 / HZ) */
long time_reftime = 0;          /* time at last adjustment (s) */

long time_adjust = 0;
long time_adjust_step = 0;

int need_resched = 0;

struct class_timer_list {
  long jiffies;
  void (*timer_function)();
  struct class_timer_list *next;

};

unsigned long timer_active = 0;
struct timer_struct timer_table[32];
static struct timer_list *next_timer = NULL;

extern void _set_system_gate(unsigned int, void *);

asmlinkage int system_call(void);

//@see system_calls.h = int_function_ptr = int (*int_function_ptr)()
int_function_ptr system_call_table [] = {
	system_debug,				// [ 0 ]
};

#define _GET_TSS_VECTOR(index) \
        ((((unsigned long) index) << 4) + (_FIRST_TSS << 3))


#define load_task_register(tss_offset) __asm__ ( \
                               "ltr %%ax"        \
                               :                 \
			       : "a" (_GET_TSS_VECTOR(tss_offset)))

#define __load_tss_helper(gdt_offset, tss_address, main_size, type) \
	__asm__ __volatile__( 					\
			"movw $" #main_size ",%1\n\t"		\
                        "movw %%ax, %2\n\t"                     \
                        "rorl $16,%%eax\n\t"                    \
                        "movb %%al,%3\n\t"                      \
                        "movb $" type ",%4\n\t"                 \
                        "movb $0x00,%5\n\t"                     \
                        "movb %%ah,%6\n\t"                      \
                        "rorl $16,%%eax"                        \
                        :                                       \
                        : "a" (tss_address + 0xC0000000),       \
                          "m" (*(gdt_offset)),                  \
                          "m" (*(gdt_offset + 2)),              \
                          "m" (*(gdt_offset + 4)),              \
                          "m" (*(gdt_offset + 5)),              \
                          "m" (*(gdt_offset + 6)),              \
                          "m" (*(gdt_offset + 7))               \
                         )

#define _load_tss_descriptor(n,addr)				\
	__load_tss_helper(((char *) (n)),((int)(addr)),235,"0x89")

#define _load_ldt_descriptor(n,addr,size) \
	__load_tss_helper(((char *) (n)),((int)(addr)),((size << 3) - 1),"0x82")


struct TSS_object _tss[__MAX_DEBUG_TASKS];

static inline void __sleep_on(struct wait_queue **p, int state) {

	unsigned long flags;
	struct wait_queue wait = { current, NULL };

	if (!p)
		return;

	if (current == task[0]) {
		panic("task[0] trying to sleep");
	}

	current->state = state;
	add_wait_queue(p, &wait);
	save_flags(flags);
	sti();
	schedule();
	remove_wait_queue(p, &wait);
	restore_flags(flags);
}

void interruptible_sleep_on(struct wait_queue **p) {
	__sleep_on(p,TASK_INTERRUPTIBLE);
}

void sleep_on(struct wait_queue **p) {
	__sleep_on(p,TASK_UNINTERRUPTIBLE);
}

void add_timer(struct timer_list *timer) {

	unsigned long flags;
	struct timer_list **p;   

	if (!timer) {
		return;
	}
	timer->next = NULL;
	p = &next_timer;
	save_flags(flags);
	cli();
	while (*p) {
		if ((*p)->expires > timer->expires) {
			(*p)->expires -= timer->expires;
			timer->next = *p;
			break;
		}
		timer->expires -= (*p)->expires;
		p = &(*p)->next;
	}
	*p = timer;
	restore_flags(flags);
}

int del_timer(struct timer_list *timer) {

	unsigned long flags;
	unsigned long expires = 0;
	struct timer_list **p;

	p = &next_timer;
	save_flags(flags);
	cli();
	while (*p) {
		if (*p == timer) {
			if ((*p = timer->next) != NULL)
				(*p)->expires += timer->expires;
			timer->expires += expires;
			restore_flags(flags);
			return 1;
		}
		expires += (*p)->expires;
		p = &(*p)->next;
	}
	restore_flags(flags);
	return 0;
}

/*
 * wake_up doesn't wake up stopped processes - they have to be awakened
 * with signals or similar.
 *
 * Note that this doesn't need cli-sti pairs: interrupts may not change
 * the wait-queue structures directly, but only call wake_up() to wake
 * a process. The process itself must remove the queue once it has woken.
 */
void wake_up(struct wait_queue **q) {
	struct wait_queue *tmp;
	struct task_struct * p;
	if (!q || !(tmp = *q))
		return;
	do {
		if ((p = tmp->task) != NULL) {
			if ((p->state == TASK_UNINTERRUPTIBLE) ||
			    (p->state == TASK_INTERRUPTIBLE)) {
				p->state = TASK_RUNNING;

				if (p->counter > current->counter) {
					need_resched = 1;
				}
			}
		}
		if (!tmp->next) {
			printk("wait_queue is bad (eip = %08lx)\n",((unsigned long *) q)[-1]);
			printk("        q = %p\n",q);
			printk("       *q = %p\n",*q);
			printk("      tmp = %p\n",tmp);
			break;
		}
		tmp = tmp->next;
	} while (tmp != *q);
}

/*
 * kernel variables
 */
long tick = 1000000 / HZ;               /* timer interrupt period */
volatile struct timeval xtime;		/* The current time */
int tickadj = 500/HZ;			/* microsecs */

DECLARE_TASK_QUEUE(tq_timer);

/*
 * Tell us the machine setup..
 */
int hard_math = 0;          /* set by boot/head.S */
int x86 = 0;                /* set by boot/head.S to 3 or 4 */
int ignore_irq13 = 0;		/* set if exception 16 works */
int wp_works_ok = 0;		/* set if paging hardware honours WP */ 

/*
 * Bus types ..
 */
int EISA_bus = 0;

extern int _setitimer(int, struct itimerval *, struct itimerval *);
unsigned long * prof_buffer = NULL;
unsigned long prof_len = 0;

#define _S(nr) (1<<((nr)-1))

extern void mem_use(void);

extern int timer_interrupt(void);
asmlinkage int system_call(void);


static unsigned long init_kernel_stack[1024] = { STACK_MAGIC, };
struct task_struct init_task = INIT_TASK;

unsigned long volatile jiffies = 0;

struct task_struct *current = &init_task;
struct task_struct *last_task_used_math = NULL;

struct task_struct *task[NR_TASKS] = {&init_task, };

long user_stack [ PAGE_SIZE>>2 ] = { STACK_MAGIC, };

struct {
	long * a;
	short b;
	} stack_start = { & user_stack [PAGE_SIZE>>2] , KERNEL_DS };

struct kernel_stat kstat = { 0 };

/*
 *  'math_state_restore()' saves the current math information in the
 * old math state array, and gets the new ones from the current task
 *
 * Careful.. There are problems with IBM-designed IRQ13 behaviour.
 * Don't touch unless you *really* know how it works.
 */
asmlinkage void math_state_restore(void)
{
	__asm__ __volatile__("clts");
	if (last_task_used_math == current)
		return;
	timer_table[COPRO_TIMER].expires = jiffies+50;
	timer_active |= 1<<COPRO_TIMER;	
	if (last_task_used_math)
		__asm__("fnsave %0":"=m" (last_task_used_math->tss.i387));
	else
		__asm__("fnclex");
	last_task_used_math = current;
	if (current->used_math) {
		__asm__("frstor %0": :"m" (current->tss.i387));
	} else {
		__asm__("fninit");
		current->used_math=1;
	}
	timer_active &= ~(1<<COPRO_TIMER);
}

#ifndef CONFIG_MATH_EMULATION

asmlinkage void math_emulate(long arg)
{
  printk("math-emulation not enabled and no coprocessor found.\n");
  printk("killing %s.\n",current->comm);

  //send_sig(SIGFPE,current,1);
  schedule();
}

#endif /* CONFIG_MATH_EMULATION */

unsigned long itimer_ticks = 0;
unsigned long itimer_next = ~0;
static unsigned long lost_ticks = 0;

/*
 *  'schedule()' is the scheduler function. It's a very simple and nice
 * scheduler: it's not perfect, but certainly works for most things.
 * The one thing you might take a look at is the signal-handler code here.
 *
 *   NOTE!!  Task 0 is the 'idle' task, which gets called when no other
 * tasks can run. It can not be killed, and it cannot sleep. The 'state'
 * information in task[0] is never used.
 *
 * The "confuse_gcc" goto is used only to get better assembly code..
 * Djikstra probably hates me.
 */
asmlinkage void schedule(void) {

	int c;
	struct task_struct *p;
	struct task_struct *next;
	unsigned long ticks;

	/* check alarm, wake up any interruptible tasks that have got a signal */

	//if (intr_count) {
	//	printk("Aiee: scheduling in interrupt\n");
	//	intr_count = 0;
	//}
	cli();
	ticks = itimer_ticks;
	itimer_ticks = 0;
	itimer_next = ~0;
	sti();
	need_resched = 0;
	p = &init_task;
	for (;;) {
		if ((p = p->next_task) == &init_task)
			goto confuse_gcc1;
		if (ticks && p->it_real_value) {
			if (p->it_real_value <= ticks) {

				//send_sig(SIGALRM, p, 1);
				if (!p->it_real_incr) {
					p->it_real_value = 0;
					goto end_itimer;
				}
				do {
					p->it_real_value += p->it_real_incr;
				} while (p->it_real_value <= ticks);
			}
			p->it_real_value -= ticks;
			if (p->it_real_value < itimer_next)
				itimer_next = p->it_real_value;
		}
end_itimer:
		if (p->state != TASK_INTERRUPTIBLE)
			continue;
		if (p->signal & ~p->blocked) {
			p->state = TASK_RUNNING;
			continue;
		}
		if (p->timeout && p->timeout <= jiffies) {
			p->timeout = 0;
			p->state = TASK_RUNNING;
		}
	}
confuse_gcc1:

	c = -1000;
	next = p = &init_task;
	for (;;) {
		if ((p = p->next_task) == &init_task)
			goto confuse_gcc2;
		if (p->state == TASK_RUNNING && p->counter > c)
			c = p->counter, next = p;
	}
confuse_gcc2:
	if (!c) {
		for_each_task(p)
			p->counter = (p->counter >> 1) + p->priority;
	}
	if (current == next)
		return;
	kstat.context_swtch++;
	
	// TODO: fix switch_to
	//switch_to(next);
	/* Now maybe reload the debug registers */
	if(current->debugreg[7]){
		loaddebug(0);
		loaddebug(1);
		loaddebug(2);
		loaddebug(3);
		loaddebug(6);
	};
}

asmlinkage int sys_pause(void)
{
	current->state = TASK_INTERRUPTIBLE;
	schedule();
	return -ERESTARTNOHAND;
}


void wake_up_interruptible(struct wait_queue **q) {
	struct wait_queue *tmp;
	struct task_struct * p;

	if (!q || !(tmp = *q))
		return;
	do {
		if ((p = tmp->task) != NULL) {
			if (p->state == TASK_INTERRUPTIBLE) {
				p->state = TASK_RUNNING;
				if (p->counter > current->counter)
					need_resched = 1;
			}
		}
		if (!tmp->next) {
			printk("wait_queue is bad (eip = %08lx)\n",((unsigned long *) q)[-1]);
			printk("        q = %p\n",q);
			printk("       *q = %p\n",*q);
			printk("      tmp = %p\n",tmp);
			break;
		}
		tmp = tmp->next;
	} while (tmp != *q);
}

void __down(struct semaphore * sem) {

	struct wait_queue wait = { current, NULL };
	add_wait_queue(&sem->wait, &wait);
	current->state = TASK_UNINTERRUPTIBLE;
	while (sem->count <= 0) {
		schedule();
		current->state = TASK_UNINTERRUPTIBLE;
	}
	current->state = TASK_RUNNING;
	remove_wait_queue(&sem->wait, &wait);
}


/*
 * Hmm.. Changed this, as the GNU make sources (load.c) seems to
 * imply that avenrun[] is the standard name for this kind of thing.
 * Nothing else seems to be standardized: the fractional size etc
 * all seem to differ on different machines.
 */
unsigned long avenrun[3] = { 0,0,0 };

/*
 * Nr of active tasks - counted in fixed-point numbers
 */
static unsigned long count_active_tasks(void) {

	struct task_struct **p;
	unsigned long nr = 0;

	for(p = &LAST_TASK; p > &FIRST_TASK; --p)
		if (*p && ((*p)->state == TASK_RUNNING ||
			   (*p)->state == TASK_UNINTERRUPTIBLE ||
			   (*p)->state == TASK_SWAPPING))
			nr += FIXED_1;
	return nr;
}

static inline void calc_load(void) {

	unsigned long active_tasks; /* fixed-point */
	static int count = LOAD_FREQ;

	if (count-- > 0)
		return;
	count = LOAD_FREQ;
	active_tasks = count_active_tasks();
	CALC_LOAD(avenrun[0], EXP_1, active_tasks);
	CALC_LOAD(avenrun[1], EXP_5, active_tasks);
	CALC_LOAD(avenrun[2], EXP_15, active_tasks);
}

/*
 * this routine handles the overflow of the microsecond field
 *
 * The tricky bits of code to handle the accurate clock support
 * were provided by Dave Mills (Mills@UDEL.EDU) of NTP fame.
 * They were originally developed for SUN and DEC kernels.
 * All the kudos should go to Dave for this stuff.
 *
 * These were ported to Linux by Philip Gladstone.
 */
static void second_overflow(void) {

	long ltemp;
	/* last time the cmos clock got updated */
	static long last_rtc_update=0;
	extern int set_rtc_mmss(unsigned long);

	/* Bump the maxerror field */
	time_maxerror = (0x70000000-time_maxerror < time_tolerance) ?
	  0x70000000 : (time_maxerror + time_tolerance);

	/* Run the PLL */
	if (time_offset < 0) {
		ltemp = (-(time_offset+1) >> (SHIFT_KG + time_constant)) + 1;
		time_adj = ltemp << (SHIFT_SCALE - SHIFT_HZ - SHIFT_UPDATE);
		time_offset += (time_adj * HZ) >> (SHIFT_SCALE - SHIFT_UPDATE);
		time_adj = - time_adj;
	} else if (time_offset > 0) {
		ltemp = ((time_offset-1) >> (SHIFT_KG + time_constant)) + 1;
		time_adj = ltemp << (SHIFT_SCALE - SHIFT_HZ - SHIFT_UPDATE);
		time_offset -= (time_adj * HZ) >> (SHIFT_SCALE - SHIFT_UPDATE);
	} else {
		time_adj = 0;
	}

	time_adj += (time_freq >> (SHIFT_KF + SHIFT_HZ - SHIFT_SCALE))
	    + FINETUNE;

	/* Handle the leap second stuff */
	switch (time_status) {
		case TIME_INS:
		/* ugly divide should be replaced */
		if (xtime.tv_sec % 86400 == 0) {
			xtime.tv_sec--; /* !! */
			time_status = TIME_OOP;
			printk("Clock: inserting leap second 23:59:60 GMT\n");
		}
		break;

		case TIME_DEL:
		/* ugly divide should be replaced */
		if (xtime.tv_sec % 86400 == 86399) {
			xtime.tv_sec++;
			time_status = TIME_OK;
			printk("Clock: deleting leap second 23:59:59 GMT\n");
		}
		break;

		case TIME_OOP:
		time_status = TIME_OK;
		break;
	}
	if (xtime.tv_sec > last_rtc_update + 660)
	  if (set_rtc_mmss(xtime.tv_sec) == 0)
	    last_rtc_update = xtime.tv_sec;
	  else
	    last_rtc_update = xtime.tv_sec - 600; /* do it again in one min */
}

/**
 * Timer bottom half.
 *
 * @see do_timer
 * @see add_timer
 */
static void timer_bh(void *unused) {
	unsigned long mask;
	struct timer_struct *tp;

	cli();
	while (next_timer && next_timer->expires == 0) {
		void (*fn)(unsigned long) = next_timer->function;
		unsigned long data = next_timer->data;
		next_timer = next_timer->next;
		sti();
		fn(data);
		cli();
	}
	sti();
	
	for (mask = 1, tp = timer_table+0 ; mask ; tp++,mask += mask) {
		if (mask > timer_active)
			break;
		if (!(mask & timer_active))
			continue;
		if (tp->expires > jiffies)
			continue;
		timer_active &= ~mask;
		tp->fn();
		sti();
	}
}

void tqueue_bh(void * unused) {
	//run_task_queue(&tq_timer);
}

/*
 * The int argument is really a (struct pt_regs *), in case the
 * interrupt wants to know from where it was called. The timer
 * irq uses this to decide if it should update the user or system
 * times.
 *
 * @see hardware_interrupts
 * @see interrupts.c do_IRQ
 */
static void do_timer(struct pt_regs *regs) {

	unsigned long mask;
	struct timer_struct *tp;
	long ltemp;

	/* Advance the phase, once it gets to one microsecond, then
	 * advance the tick more.
	 */
	time_phase += time_adj;
	if (time_phase < -FINEUSEC) {
		ltemp = -time_phase >> SHIFT_SCALE;
		time_phase += ltemp << SHIFT_SCALE;
		xtime.tv_usec += tick + time_adjust_step - ltemp;
	} else if (time_phase > FINEUSEC) {
		ltemp = time_phase >> SHIFT_SCALE;
		time_phase -= ltemp << SHIFT_SCALE;
		xtime.tv_usec += tick + time_adjust_step + ltemp;
	} else {
		xtime.tv_usec += tick + time_adjust_step;
	}

	if (time_adjust) {  
	    /* We are doing an adjtime thing. 
	     *
	     * Modify the value of the tick for next time.
	     * Note that a positive delta means we want the clock
	     * to run fast. This means that the tick should be bigger
	     *
	     * Limit the amount of the step for *next* tick to be
	     * in the range -tickadj .. +tickadj
	     */
	     if (time_adjust > tickadj)
	       time_adjust_step = tickadj;
	     else if (time_adjust < -tickadj)
	       time_adjust_step = -tickadj;
	     else
	       time_adjust_step = time_adjust;
	     
	    /* Reduce by this step the amount of time left  */
	    time_adjust -= time_adjust_step;
	} else {
	    time_adjust_step = 0;
	}

	if (xtime.tv_usec >= 1000000) {
	    xtime.tv_usec -= 1000000;
	    xtime.tv_sec++;
	    second_overflow();
	}

	jiffies++;
	calc_load();
	if ((VM_MASK & regs->eflags) || (3 & regs->cs)) {
		current->utime++;
		if (current != task[0]) {
			if (current->priority < 15)
				kstat.cpu_nice++;
			else
				kstat.cpu_user++;
		}
		/* Update ITIMER_VIRT for current task if not in a system call */
		if (current->it_virt_value && !(--current->it_virt_value)) {
			current->it_virt_value = current->it_virt_incr;
			//send_sig(SIGVTALRM,current,1);
		}
	} else {
		current->stime++;
		if(current != task[0])
			kstat.cpu_system++;
	}
	if (current != task[0] && 0 > --current->counter) {
		current->counter = 0;
		need_resched = 1;
	}
	/* Update ITIMER_PROF for the current task */
	if (current->it_prof_value && !(--current->it_prof_value)) {
		current->it_prof_value = current->it_prof_incr;
		//send_sig(SIGPROF,current,1);
	}
	for (mask = 1, tp = timer_table+0 ; mask ; tp++,mask += mask) {
		if (mask > timer_active)
			break;
		if (!(mask & timer_active))
			continue;
		if (tp->expires > jiffies)
			continue;
		mark_bh(TIMER_BH);
	}

	cli();
	itimer_ticks++;
	if (itimer_ticks > itimer_next) {
		need_resched = 1;
	}
	if (next_timer) {
		if (next_timer->expires) {
			next_timer->expires--;
			if (!next_timer->expires) {
				mark_bh(TIMER_BH);
			}
		} else {
			lost_ticks++;
			mark_bh(TIMER_BH);
		}
	}
	if (tq_timer != &tq_last) {
		mark_bh(TQUEUE_BH);
	}
	sti();
}

asmlinkage int sys_alarm(long seconds) {

	struct itimerval it_new, it_old;
	it_new.it_interval.tv_sec = it_new.it_interval.tv_usec = 0;
	it_new.it_value.tv_sec = seconds;
	it_new.it_value.tv_usec = 0;

	//_setitimer(ITIMER_REAL, &it_new, &it_old);
	return(it_old.it_value.tv_sec + (it_old.it_value.tv_usec / 1000000));
}

asmlinkage int sys_getpid(void) {
	return current->pid;
}

asmlinkage int sys_getppid(void) {
	return current->p_opptr->pid;
}

asmlinkage int sys_getuid(void) {
	return current->uid;
}

asmlinkage int sys_geteuid(void) {
	return current->euid;
}

asmlinkage int sys_getgid(void) {
	return current->gid;
}

asmlinkage int sys_getegid(void) {
	return current->egid;
}

asmlinkage int sys_nice(long increment) {
	int newprio;

	//if (increment < 0 && !suser())
	//	return -EPERM;

	newprio = current->priority - increment;
	if (newprio < 1)
		newprio = 1;
	if (newprio > 35)
		newprio = 35;
	current->priority = newprio;
	return 0;
}

static void show_task(int nr, struct task_struct *p) {

	unsigned long free;
	static char * stat_nam[] = { "R", "S", "D", "Z", "T", "W" };

	printk("%-8s %3d ", p->comm, (p == current) ? -nr : nr);
	if (((unsigned) p->state) < sizeof(stat_nam)/sizeof(char *))
		printk(stat_nam[p->state]);
	else
		printk(" ");
	if (p == current)
		printk(" current  ");
	else
		printk(" %08lX ", ((unsigned long *)p->tss.esp)[3]);
	for (free = 1; free < 1024 ; free++) {
		if (((unsigned long *)p->kernel_stack_page)[free])
			break;
	}
	printk("%5lu %5d %6d ", free << 2, p->pid, p->p_pptr->pid);
	if (p->p_cptr)
		printk("%5d ", p->p_cptr->pid);
	else
		printk("      ");
	if (p->p_ysptr)
		printk("%7d", p->p_ysptr->pid);
	else
		printk("       ");
	if (p->p_osptr)
		printk(" %5d\n", p->p_osptr->pid);
	else
		printk("\n");
}

void show_state(void) {

	int i;
	printk("                         free                        sibling\n");
	printk("  task             PC    stack   pid father child younger older\n");
	for (i=0 ; i<NR_TASKS ; i++)
		if (task[i])
			show_task(i,task[i]);
}

void sched_init(void) {

	int i;
	struct desc_struct *p;
   
	bh_base[TIMER_BH].routine = timer_bh;
	bh_base[TQUEUE_BH].routine = tqueue_bh;

	if (sizeof(struct sigaction) != 16) {
		panic("Struct sigaction MUST be 16 bytes");
	}

	//set_tss_desc(gdt+FIRST_TSS_ENTRY,&init_task.tss);
	//set_ldt_desc(gdt+FIRST_LDT_ENTRY,&default_ldt,1);
	//_set_system_gate(0x80, &system_call);

	//p = gdt + 2 + FIRST_TSS_ENTRY;
	//for(i=1; i < NR_TASKS; i++) {
	//	task[i] = NULL;
	//	p->a = p->b = 0;
	//	p++;
	//	p->a=p->b=0;
	//	p++;
	//}

	/* Clear NT, so that we won't have troubles with that later on */
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	//load_TR(0);
	//load_ldt(0);

	outb_p(0x34,0x43);              // binary, mode 2, LSB/MSB, ch 0 
	outb_p(LATCH & 0xff , 0x40);	// LSB
	outb(LATCH >> 8 , 0x40);	    // MSB

	if (request_irq(TIMER_IRQ,(void (*)(int)) do_timer) != 0) {
		panic("Could not allocate timer IRQ!");
	}
}
