//
// Berlin Brown
// bigbinc@hotmail.com
//
//
// $Id: time_helpers.c,v 1.2 2005/05/26 00:06:53 bigbinc Exp $
//

#include <system/system.h>
#include <asm/io.h>


#define _MAX_JIFFY_DELAY 30000

//
// Note: only use volatile for values changed by input from hardware
// Note: a jiffy count of 10,000 should represent 100 seconds
//

//
// jiffy counters are located in 
// see interrupts.c
//
extern unsigned long _get_jiffy_value(void);
extern void __xloop_delay(unsigned long);

//
// for simple single task delays
// dos style...ewww
//
static unsigned long __jiffy_start = 0;
static unsigned long __jiffy_end = 0;
static unsigned long __jiffy_diff = 0;
static unsigned long __jiffy_count = 0;

void _jiffy_reset(void)
{

  __jiffy_start = 0;
  __jiffy_end = 0;
  __jiffy_diff = 0;
  __jiffy_count = 0;
  
}

void _jiffy_delay_setup(int jiffy_count)
{

  _jiffy_reset();  
  __jiffy_count = jiffy_count;

}

void _jiffy_start(void)
{

  __jiffy_start = _get_jiffy_value();

}

void _jiffy_delay(void)
{

  // this will hold the machine
  // i.e. dos, use only for debugging
  __jiffy_end = _get_jiffy_value();  
  __jiffy_diff = __jiffy_end - __jiffy_start;

  for(;;)
  {    
    __jiffy_end = _get_jiffy_value(); 
    __jiffy_diff = __jiffy_end - __jiffy_start;

    if (__jiffy_diff > _MAX_JIFFY_DELAY) {

      _jiffy_reset();
      
      break;

    }

    if (__jiffy_diff > __jiffy_count) {

      _jiffy_reset();

      break;

    }   
  }
}
