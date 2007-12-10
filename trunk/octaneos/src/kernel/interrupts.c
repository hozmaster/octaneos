/*
 * interrupts.c
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
 *
 * (Also see hardware_interrupts.S and bad_interrupts.S for asm definitions
 * this file is synonymous with irq.c in the linux kernel)
 */

#include <system/system.h>
#include <asm/io.h>
#include <linux/errno.h>
#include <linux/signal.h>

#define HZ 100
#define CLOCK_TICK_RATE 1193180
#define CLOCK_TICK_FACTOR       20
#define TIMER_LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)

asmlinkage void get_all_registers(void);

extern volatile unsigned long jiffies;

unsigned long bh_active = 0;

// defined in bad_interrupts.S in assembly
asmlinkage void bad_interrupt_00(void);
asmlinkage void bad_interrupt_01(void);
asmlinkage void bad_interrupt_02(void);
asmlinkage void bad_interrupt_03(void);
asmlinkage void bad_interrupt_04(void);
asmlinkage void bad_interrupt_05(void);
asmlinkage void bad_interrupt_06(void);
asmlinkage void bad_interrupt_07(void);

asmlinkage void bad_interrupt_08(void);
asmlinkage void bad_interrupt_09(void);
asmlinkage void bad_interrupt_10(void);
asmlinkage void bad_interrupt_11(void);
asmlinkage void bad_interrupt_12(void);
asmlinkage void bad_interrupt_13(void);
asmlinkage void bad_interrupt_14(void);
asmlinkage void bad_interrupt_15(void);

// Also see hardware_interrupts.S for these general operations
asmlinkage hw_interrupt_entry_00(void);
asmlinkage hw_interrupt_entry_01(void);
asmlinkage hw_interrupt_entry_06(void);

/**
 * Array data structure for general and bad_interrupt callbacks
 */
static void (*bad_interrupt[16])(void) = {
	bad_interrupt_00, 
	bad_interrupt_01,
	bad_interrupt_02, 
	bad_interrupt_03,
	bad_interrupt_04, 
	bad_interrupt_05,
	bad_interrupt_06, 
	bad_interrupt_07,
	bad_interrupt_08, 
	bad_interrupt_09,
	bad_interrupt_10, 
	bad_interrupt_11,
	bad_interrupt_12, 
	bad_interrupt_13,
	bad_interrupt_14, 
	bad_interrupt_15
};

static void (*interrupt[16])(void) = {
	hw_interrupt_entry_00, 
	hw_interrupt_entry_01, 
	NULL, 
	NULL,
	NULL, 
	NULL, 
	hw_interrupt_entry_06, 
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
 * Initial irq handlers.
 */
static struct sigaction irq_sigaction[16] = {
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL },
	{ NULL, 0, 0, NULL }, { NULL, 0, 0, NULL }
};

static unsigned int cached_irq_mask = 0xffff;

#define __byte(x,y) 	(((unsigned char *)&(y))[x])
#define cached_21	(__byte(0,cached_irq_mask))
#define cached_A1	(__byte(1,cached_irq_mask))

// we also might want to use cache_21

unsigned char cache_21 = 0xff;
unsigned char cache_A1 = 0xff;

extern void set_intr_gate(unsigned int, void *);

extern struct TSS_object _tss;

unsigned long _get_jiffy_value(void)
{
  return jiffies;
}

void __xloop_delay(unsigned long loops)
{
  int d0;
  __asm__ __volatile__(
		       "\tjmp 1f\n"
		       ".align 16\n"
		       "1:\tjmp 2f\n"
		       ".align 16\n"
		       "2:\tdecl %0\n\tjns 2b"
		       :"=&a" (d0)
		       :"0" (loops));
}

int check_timer_irq(void)
{
  unsigned int __tmp01 =  jiffies;
  sti();    
  __xloop_delay(0xf0000); 
  return jiffies  - __tmp01;    
}

void __debug_timer_irq(void) {
  _disable_interrupts();
  jiffies++;

  //switch_task(_tss[0]);
  // turn em back on
  _enable_interrupts();
}


int request_irq(unsigned int irq, void (*handler)(int)) {
	struct sigaction signal_action;

	signal_action.sa_handler = handler;
	signal_action.sa_flags = 0;
	signal_action.sa_mask = 0;
	signal_action.sa_restorer = NULL;
	return irqaction(irq, &signal_action);
}

int irqaction(unsigned int irq, struct sigaction *new_signal_action) {
	struct sigaction *signal_action;
	unsigned long flags;

	if (irq > 15)
		return -EINVAL;

	signal_action = irq + irq_sigaction;
	if (signal_action->sa_mask)
		return -EBUSY;
	if (!new_signal_action->sa_handler)
		return -EINVAL;

	save_flags(flags);
	cli();
	*signal_action = *new_signal_action;
	signal_action->sa_mask = 1;

	//if (sa->sa_flags & SA_INTERRUPT) {
	//	set_intr_gate(0x20+irq, fast_interrupt[irq]);
	//} else {
		set_intr_gate(0x20+irq, interrupt[irq]);

	if (irq < 8) {
		cache_21 &= ~(1<<irq);
		outb(cache_21,0x21);
	} else {
		cache_21 &= ~(1<<2);
		cache_A1 &= ~(1<<(irq-8));
		outb(cache_21,0x21);
		outb(cache_A1,0xA1);
	}
	restore_flags(flags);
	return 0;
}

void disable_irq(unsigned int irq_nr) {

	unsigned long flags;
	unsigned char mask;
	mask = 1 << (irq_nr & 7);
	save_flags(flags);
	if (irq_nr < 8) {
		cli();
		cache_21 |= mask;
		outb(cache_21,0x21);
		restore_flags(flags);
		return;
	}
	cli();
	cache_A1 |= mask;
	outb(cache_A1,0xA1);
	restore_flags(flags);
}

void enable_irq(unsigned int irq_nr) {

	unsigned long flags;
	unsigned char mask;

	mask = ~(1 << (irq_nr & 7));
	save_flags(flags);
	if (irq_nr < 8) {
		cli();
		cache_21 &= mask;
		outb(cache_21,0x21);
		restore_flags(flags);
		return;
	}
	cli();
	cache_A1 &= mask;
	outb(cache_A1,0xA1);
	restore_flags(flags);
}


void free_irq(unsigned int irq) {

	struct sigaction *signal_action = irq + irq_sigaction;
	unsigned long flags;
	if (irq > 15) {
		printk("Trying to free IRQ%d\n",irq);
		return;
	}
	if (!signal_action->sa_mask) {
		printk("Trying to free free IRQ%d\n",irq);
		return;
	}
	save_flags(flags);
	cli();
	if (irq < 8) {
		cache_21 |= 1 << irq;
		outb(cache_21,0x21);
	} else {
		cache_A1 |= 1 << (irq-8);
		outb(cache_A1,0xA1);
	}

	set_intr_gate(0x20+irq, bad_interrupt[irq]);
	signal_action->sa_handler = NULL;
	signal_action->sa_flags = 0;
	signal_action->sa_mask = 0;
	signal_action->sa_restorer = NULL;
	restore_flags(flags);
}

asmlinkage void do_IRQ(int irq, struct pt_regs *regs) {
	struct sigaction *signal_action = NULL;
	//kstat.interrupts[irq]++;
	printk("IRQ called");
	signal_action = irq + irq_sigaction;
	signal_action->sa_handler((int) regs);
}

void print_tmp_ctr(void) {
  printk("CTR_DEBUG : %d\n", jiffies);
}

void handle_interrupt(int _irq_no) {
	
  char buf[80];
  unsigned long flags;

  save_flags(flags);
  cli();

  switch(_irq_no) {
  case 0:
    
    // timer -- irq
    set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_00);
    break;

  case 1:
    
    // keyboard -- irq
    set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_01);
    break;

  case  6:
    
    // floppy ---- irq
    set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_06);    
    break;

  default:
    break;
  };
    
  if (_irq_no < 8)
    {
      
      cache_21 &= ~(1<<_irq_no);      
      outb(cache_21,0x21);

    } else {
      
      cache_21 &= ~(1<<2);
      cache_A1 &= ~(1<<(_irq_no-8));
      
      outb(cache_21,0x21);
      outb(cache_A1,0xA1);
      
    }
  restore_flags(flags);

}

//
// Load the 8259 PIC
void load_remap_controller(void) {
        
  outb(0xff, 0x21);
  outb(0xff, 0xA1);
  
  outb_p(0x11, 0x20);
  outb_p(0x20 + 0, 0x21);
  outb_p(0x04, 0x21);
  
  outb_p(0x01, 0x21);
	
  outb_p(0x11, 0xA0);
  outb_p(0x20 + 8, 0xA1);

  outb_p(0x02, 0xA1);
  outb_p(0x01, 0xA1);

  __xloop_delay(100);  
  outb(cached_21, 0x21);
  outb(cached_A1, 0xA1);
  
} // End of the funtion 

static void math_error_irq(int cpl) {
	outb(0, 0xF0);
	panic("MATH ERROR");
}

static void no_action(int cpl) { }

static struct sigaction ignore_IRQ = {
	no_action,
	0,
	SA_INTERRUPT,
	NULL
};

void print_register_list(struct debug_registers *check_registers) {

  char buf[255];
  __sprintf(buf, " >> Checking registers <<\n"); __puts(buf);
  __sprintf(buf, "    EAX: %x    ECX: %x    EBX: %x\n", check_registers->__eax,
	    check_registers->__ecx, check_registers->__ebx);
  __puts(buf);
  __sprintf(buf, "    DS: %x    ES: %x    FS: %x\n", check_registers->__ds,
	    check_registers->__es, check_registers->__fs);
  __puts(buf);
  __sprintf(buf, "    EIP: %x    CS: %x    OLDAX: %x\n", check_registers->__eip,
	    check_registers->__cs, check_registers->__old_eax);
  __puts(buf);

}

void init_interrupts(void) {
	
	int i = 0;	
	load_remap_controller();
	
	// Set the 16 interrupts initially to (default) bad_interrupts
	for (i = 0; i < 16 ; i++) {
		set_intr_gate(0x20+i, bad_interrupt[i]);
	}

	if (irqaction(2,&ignore_IRQ))
		printk("Unable to get IRQ2 for cascade\n");
	if (request_irq(13, math_error_irq))
		printk("Unable to get IRQ13 for math-error handler\n");

	disable_irq(0);
	
	//**********************************************
	// program the 8253 - Interval Timer
	// Set frequency of our timer
	// LATCH = 11931.8 gives to 8253 (in output) 
	// a frequency of 1193180 / 11931.8 = 100 Hz, so period = 10ms
	//**********************************************
	outb_p(0x34,0x43);
	outb_p(TIMER_LATCH & 0xff, 0x40);
	outb(TIMER_LATCH >> 8 , 0x40);
	
	// prepare the timer_interrupt
	handle_interrupt(0x00);	
	handle_interrupt(0x01);
}
