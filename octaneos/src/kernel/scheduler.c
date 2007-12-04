//
// Berlin Brown
//==========================================================
//
// $Id: scheduler.c,v 1.15 2005/05/26 00:06:53 bigbinc Exp $
// sti - enable interrupts
// cli - clear interrupt enable
//
//
// TSS - GDT layout
//
// 0 - null
// 1 - kernel code segment
// 2 - kernel data segment
// 3 - user code segment
// 4 - user data segment
//
// TSS layout  
// 
// 8 - TSS [ task 0 ]
// 9 - LDT [ task 0 ]
// 10 - TSS [ task 1 ]
// 11 - LDT [ task x1 ]
//
// the task_switcher just jumps to the value in 

// tss.tr
//==========================================================

#include <system/system.h>		    // super-header
#include <system/system_calls.h>	// extern function call list
#include <linux/wait.h>
#include <linux/sched.h>

extern descriptor_table _idt;
extern descriptor_table gdt;

#define TIMER_LIST_REQUESTS                 64

//int time_status = TIME_BAD;     /* clock synchronization status */
long time_offset = 0;           /* time adjustment (us) */
long time_constant = 0;         /* pll time constant */
//long time_tolerance = MAXFREQ;  /* frequency tolerance (ppm) */
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

}; // end of the struct

static struct class_timer_list private_timer_list[TIMER_LIST_REQUESTS];
static struct class_timer_list *private_next_timer = NULL;

unsigned long timer_active = 0;
struct timer_struct timer_table[32];
static struct timer_list *next_timer = NULL;

extern void _set_system_gate(unsigned int, void *);

// defined in [ exceptions.S ] 
asmlinkage int system_call(void);

//.....................................................
// system_call vector table list function pointer array

// see system_calls.h = int_function_ptr = int (*int_function_ptr)()
int_function_ptr system_call_table [] = {
	system_debug,				// [ 0 ] 
};

//.....................................................
// two tasks + the main kernel

//  [ as opposed to _TSS ]
#define _GET_TSS_VECTOR(index) \
        ((((unsigned long) index) << 4) + (_FIRST_TSS << 3))

//
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

static void public_another_test(void)
{

  char buf[80];

  __sprintf(buf, "I am the second timer test, good-bye!\n");
  __puts(buf);

}

void public_timer_test(void)
{

  char buf[80];

  __sprintf(buf, "Timer called, have a nice day!\n");
  __puts(buf);

  public_add_timer(0xf00, public_another_test);

}

//... see interrupts.c - in timer_interrupt
//       - called in jiffies increment code
void scheduler_timer_helper(void)
{

  char buf[80];

  // [ declare function pointer ]
  void (*local_call_timer_function)(void);
     
  if (private_next_timer)
  {

    //  [ decrement the timer, on zero, actually call the function ]
    private_next_timer->jiffies--;

    while (private_next_timer && 
	   private_next_timer->jiffies <= 0) {

      local_call_timer_function = private_next_timer->timer_function;
      
      private_next_timer->timer_function = NULL;
      private_next_timer = private_next_timer->next;

      
      // run the function saved function ----! GO !
      local_call_timer_function();

    }

  }

}

// Called whenever you want a to delay a function
// by a set number of jiffies
void public_add_timer(long jiffies_timer_count, void (*function_for_timer)(void)) {

  char buf[80];
  struct class_timer_list *p = NULL;

  if (!function_for_timer) {
    
    return;
    
  }
  _disable_interrupts();
  if (jiffies_timer_count <= 0)
  {

    // finally call the fnuction 
    (function_for_timer)();
    
  } else {
    
    for (p = private_timer_list; p < (private_timer_list + TIMER_LIST_REQUESTS); p++)
    {

      // we found a free slot!
      if (!p->timer_function)
      {
		break;
      }

    }
    
    if (p >= private_timer_list + TIMER_LIST_REQUESTS) {
      
      __sprintf(buf, "No free timer request slots");
      __puts(buf);
      
      return;
      
    }
    
    // assign the new timer function
    p->timer_function  = function_for_timer;      
    p->jiffies = jiffies_timer_count;
    
    // increment the list value
    // next  = NULL, but the saved equals something, hehe
    p->next = private_next_timer;          
    private_next_timer = p;
    
    while(
	  p->next &&
	  (p->next->jiffies < p->jiffies))
	{
      
      p->jiffies -= p->next->jiffies;
      function_for_timer = p->timer_function;
      
      p->timer_function = p->next->timer_function;
      p->next->timer_function = function_for_timer;
      
      jiffies_timer_count = p->jiffies;
      p->jiffies = p->next->jiffies;
      
      p->next->jiffies = jiffies_timer_count;
      p = p->next;
      
      // ok, timer value set, now use 
      // scheduler timer helper to actually use it      
    } 
  }  
  _enable_interrupts();
     
}

static void clear_timer_interrupts(void)
{  
  int i;
  private_next_timer = NULL;
  for (i = 0; i < TIMER_LIST_REQUESTS; i++) {
    private_timer_list[i].jiffies = 0;
    private_timer_list[i].timer_function = NULL;
  }
}	

//	
// called by init/main.c
//	
void scheduler_init(void) {

  int _task_offset = -1;

  clear_timer_interrupts();
    
  // set the tss memory location for 3 tasks
  // Note: 8 represents the first tss, see above
	     
  _task_offset = 0;
  _load_tss_descriptor((gdt + 0x08), &_tss[0]);

  _task_offset = 1;
  _load_tss_descriptor((gdt + ((_task_offset << 1) + _FIRST_TSS)), &_tss[1]);

  _task_offset = 2;
  _load_tss_descriptor((gdt + ((_task_offset << 1) + _FIRST_TSS)), &_tss[2]);

  //
  // set the system software interrupt
  // see: traps.c  for the actual function definition
  _set_system_gate(0x80, &system_call);
 
  // load the task register
  load_task_register(0x0);

}

static inline void __sleep_on(struct wait_queue **p, int state) {

	unsigned long flags;
	//struct wait_queue wait = { current, NULL };
	struct wait_queue wait = { NULL, NULL };

	if (!p)
		return;

	//if (current == task[0])
	//	panic("task[0] trying to sleep");
	//current->state = state;

	add_wait_queue(p, &wait);
	save_flags(flags);
	sti();
	//schedule();
	remove_wait_queue(p, &wait);
	restore_flags(flags);
}

void interruptible_sleep_on(struct wait_queue **p)
{
	__sleep_on(p,TASK_INTERRUPTIBLE);
}

void sleep_on(struct wait_queue **p)
{
	__sleep_on(p,TASK_UNINTERRUPTIBLE);
}

void add_timer(struct timer_list * timer)
{
	unsigned long flags;
	struct timer_list ** p;

	if (!timer)
		return;
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

int del_timer(struct timer_list * timer)
{
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
void wake_up(struct wait_queue **q)
{
	struct wait_queue *tmp;
	struct task_struct * p;
	if (!q || !(tmp = *q))
		return;
	do {
		if ((p = tmp->task) != NULL) {
			if ((p->state == TASK_UNINTERRUPTIBLE) ||
			    (p->state == TASK_INTERRUPTIBLE)) {
				p->state = TASK_RUNNING;

				//if (p->counter > current->counter)
				//	need_resched = 1;
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

