//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: dma_functions.c,v 1.1 2004/03/29 13:44:57 bigbinc Exp $
//

#include <system/alpha.h>

static volatile unsigned int dma_chan_busy[MAX_DMA_CHANNELS] = {
  1, 0, 0, 0, 1, 0, 0, 0
};

static __inline__ unsigned int mutex_atomic_swap(volatile unsigned int *p, 
						 unsigned int newval)
{
  unsigned int semval = newval;    
  asm __volatile__ (
		    "xchgl %2, %0\n"         \
		    : "=r" (semval)          \
		    : "0" (semval), "m" (*p) \
		    );

  return semval;
}

int request_dma(unsigned int dmanr)
{
  if (dmanr >= MAX_DMA_CHANNELS)
    return -500;
  
  if (mutex_atomic_swap(&dma_chan_busy[dmanr], 1) != 0)
    return -600;
  else
    return 0;
  
} // end of the functino +++

void free_dma(unsigned int dmanr) {
  
  char buf[80];
  if (dmanr >= MAX_DMA_CHANNELS) {
    __sprintf(buf, "Trying to free DMA%d\n", dmanr);
    __puts(buf);
    return;
  }
  
  if (mutex_atomic_swap(&dma_chan_busy[dmanr], 0) == 0) {
    __sprintf(buf, "Trying to free free DMA%d\n", dmanr);
    __puts(buf);
  }

}
