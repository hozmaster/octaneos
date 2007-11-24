//
// Berlin Brown
//
// interrupts.c
//
// $Id: interrupts.c,v 1.15 2005/05/26 00:06:53 bigbinc Exp $
//
// to load an irq
// * load the pic:
// * enable the irq
// * and based on the linux code, timer irqs can be weird
//        vector = assign_irq_vector(0);
//        set_intr_gate(vector, interrupt[0]);
//
// see io_apic.c
//

/*
  
IRQs jump to:

action->handler(irq, action->dev_id, regs);

 Note:

 startup_none,
 shutdown_none,
 enable_none,
 disable_none,
 ack_none,
 end_none
 interrupt_handler000 --->>> 
 save_regs
 
 do_IRQ --->> handler

*/

#include <system/system.h>
#include <asm/io.h>


#define HZ 100
#define CLOCK_TICK_RATE 1193180
#define CLOCK_TICK_FACTOR       20
#define TIMER_LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)

asmlinkage void get_all_registers(void);


// jiffies used a lot in scheduler.c...
volatile unsigned long _jiffies = 0;


// defined in bad_interrupts.S in assembly --

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

//==========================================================
// Function Definitions
//==========================================================

unsigned long _get_jiffy_value(void)
{
  
  return _jiffies;

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
  unsigned int __tmp01 =  _jiffies;
  sti();    
  __xloop_delay(0xf0000); 
  return _jiffies  - __tmp01;
    
}

void __disable_irq(unsigned int irq)
{
	
  unsigned int mask = 1 << irq;
  unsigned long flags;
  
  
  cached_irq_mask |= mask;
  if (irq & 8)
    outb(cached_A1,0xA1);
  else
    outb(cached_21,0x21);
  
}

void __enable_irq(unsigned int irq)
{

  unsigned int mask = ~(1 << irq);
  unsigned long flags;
  
  cached_irq_mask &= mask;
  if (irq & 8)
    outb(cached_A1,0xA1);
  else
    outb(cached_21,0x21);

}


//
// This gets called on the interrupt
//
//
// as of 4/10/2004 - deprecated
//
void __debug_timer_irq(void) {
  _disable_interrupts();

  _jiffies++;

  //switch_task(_tss[0]);

  // turn em back on....
  _enable_interrupts();

}

void _public_timer_irq(void)
{
	
  _disable_interrupts();
  _jiffies++;

  scheduler_timer_helper();

  _enable_interrupts();

}

void print_tmp_ctr(void) {

  char buf[80];
 
  __sprintf(buf, "CTR_DEBUG : %d\n", _jiffies);
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
  
} // end of the funtion +++


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
  __disable_irq(0);

  
  //.................................................................
  //
  // program the 8253 - Interval Timer
  //
  // set frequency of out timer and stuff
  //
  // http://www.cclinf.polito.it
  //
  // LATCH = 11931.8 gives to 8253 (in output) 
  // a frequency of 1193180 / 11931.8 = 100 Hz, so period = 10ms
  //
  //.................................................................

  outb_p(0x34,0x43);
  outb_p(TIMER_LATCH & 0xff, 0x40);
  outb(TIMER_LATCH >> 8 , 0x40);

  // prepare the timer_interrupt...GO. ###
  handle_interrupt(0x00);
 
}
