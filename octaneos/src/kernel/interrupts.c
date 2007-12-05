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
 * (Also see hardware_interrupts.S for asm definitions
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

// jiffies used a lot in scheduler.c
volatile unsigned long jiffies = 0;

// defined in bad_interrupts.S in assembly

asmlinkage void __bad_interrupt_00_01(void);
asmlinkage void __bad_interrupt_01_02(void);
asmlinkage void __bad_interrupt_02_04(void);
asmlinkage void __bad_interrupt_03_08(void);
asmlinkage void __bad_interrupt_04_10(void);
asmlinkage void __bad_interrupt_05_20(void);
asmlinkage void __bad_interrupt_06_40(void);
asmlinkage void __bad_interrupt_07_80(void);

asmlinkage void __bad_interrupt_08_01(void);
asmlinkage void __bad_interrupt_09_02(void);
asmlinkage void __bad_interrupt_10_04(void);
asmlinkage void __bad_interrupt_11_08(void);
asmlinkage void __bad_interrupt_12_10(void);
asmlinkage void __bad_interrupt_13_20(void);
asmlinkage void __bad_interrupt_14_40(void);
asmlinkage void __bad_interrupt_15_80(void);

// see.... hardware_interrupts.S
asmlinkage hw_interrupt_entry_00(void);
asmlinkage hw_interrupt_entry_01(void);
asmlinkage hw_interrupt_entry_06(void);

static unsigned int cached_irq_mask = 0xffff;

#define __byte(x,y) 	(((unsigned char *)&(y))[x])
#define cached_21	(__byte(0,cached_irq_mask))
#define cached_A1	(__byte(1,cached_irq_mask))

// we also might want to use cache_21

unsigned char cache_21 = 0xff;
unsigned char cache_A1 = 0xff;

extern void _set_intr_gate(unsigned int, void *);
extern void scheduler_timer_helper(void);

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

void _public_timer_irq(void)
{
  _disable_interrupts();
  jiffies++;
  scheduler_timer_helper();
  _enable_interrupts();

}

int irqaction(unsigned int irq, struct sigaction *new_sa)
{
	struct sigaction *sa;
	unsigned long flags;

	if (irq > 15)
		return -EINVAL;

	//sa = irq + irq_sigaction;
	if (sa->sa_mask)
		return -EBUSY;
	if (!new_sa->sa_handler)
		return -EINVAL;
	save_flags(flags);
	cli();
	*sa = *new_sa;
	sa->sa_mask = 1;

	//if (sa->sa_flags & SA_INTERRUPT)
	//	set_intr_gate(0x20+irq,fast_interrupt[irq]);
	//else
	//	set_intr_gate(0x20+irq,interrupt[irq]);

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

void enable_irq(unsigned int irq_nr)
{
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


void free_irq(unsigned int irq)
{
	//struct sigaction *sa = irq + irq_sigaction;
	unsigned long flags;
	if (irq > 15) {
		printk("Trying to free IRQ%d\n",irq);
		return;
	}
	//if (!sa->sa_mask) {
	//	printk("Trying to free free IRQ%d\n",irq);
	//	return;
	//}
	save_flags(flags);
	cli();
	if (irq < 8) {
		cache_21 |= 1 << irq;
		outb(cache_21,0x21);
	} else {
		cache_A1 |= 1 << (irq-8);
		outb(cache_A1,0xA1);
	}

	//set_intr_gate(0x20+irq, _bad_interrupt[irq]);
	//sa->sa_handler = NULL;
	//sa->sa_flags = 0;
	//sa->sa_mask = 0;
	//sa->sa_restorer = NULL;
	restore_flags(flags);
}

void print_tmp_ctr(void) {
  char buf[80];
  __sprintf(buf, "CTR_DEBUG : %d\n", jiffies);
  __puts(buf); 
}

void handle_interrupt(int _irq_no)
{
	
  char buf[80];
  unsigned long flags;

  save_flags(flags);
  cli();

  switch(_irq_no) {
  case 0:
    
    // timer -- irq
    _set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_00);
    break;

  case 1:
    
    // keyboard -- irq
    _set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_01);
    break;

  case  6:
    
    // floppy ---- irq
    _set_intr_gate(0x20 + _irq_no, hw_interrupt_entry_06);
    
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
void load_remap_controller(void)
{
        
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

void load_interrupts(void) {

  load_remap_controller();

  _set_intr_gate(0x20+0, __bad_interrupt_00_01);
  _set_intr_gate(0x20+1, __bad_interrupt_01_02);
  _set_intr_gate(0x20+2, __bad_interrupt_02_04);
  _set_intr_gate(0x20+3, __bad_interrupt_03_08);
  _set_intr_gate(0x20+4, __bad_interrupt_04_10);
  _set_intr_gate(0x20+5, __bad_interrupt_05_20);
  _set_intr_gate(0x20+6, __bad_interrupt_06_40);
  _set_intr_gate(0x20+7, __bad_interrupt_07_80);

  _set_intr_gate(0x20+8, __bad_interrupt_08_01);
  _set_intr_gate(0x20+9, __bad_interrupt_09_02);
  _set_intr_gate(0x20+10, __bad_interrupt_10_04);
  _set_intr_gate(0x20+11, __bad_interrupt_11_08);
  _set_intr_gate(0x20+12, __bad_interrupt_12_10);
  _set_intr_gate(0x20+13, __bad_interrupt_13_20);
  _set_intr_gate(0x20+14, __bad_interrupt_14_40);
  _set_intr_gate(0x20+15, __bad_interrupt_15_80);
  disable_irq(0);
  
  //.................................................................
  // program the 8253 - Interval Timer
  // Set frequency of out timer and stuff
  // http://www.cclinf.polito.it
  // LATCH = 11931.8 gives to 8253 (in output) 
  // a frequency of 1193180 / 11931.8 = 100 Hz, so period = 10ms
  //.................................................................

  outb_p(0x34,0x43);
  outb_p(TIMER_LATCH & 0xff, 0x40);
  outb(TIMER_LATCH >> 8 , 0x40);

  // prepare the timer_interrupt...GO. ###
  handle_interrupt(0x00);
 
}
