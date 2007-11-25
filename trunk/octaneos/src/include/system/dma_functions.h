//==========================================================
// Copyright (C) 2003, 2007 Berlin Brown
//
// $Id: dma_functions.h,v 1.4 2005/05/26 00:06:45 bigbinc Exp $
//
//==========================================================
// see linux kernel dma.h for more info
//==========================================================
// second phase of header variables
//
#ifndef _DMA_FUNCTIONS_H_
#define _DMA_FUNCTIONS_H_

#include <asm/io.h>
#include <system/alpha.h>

//==========================================================
// DMA FUNCTIONS
//==========================================================
static __inline__ void enable_dma(unsigned int dmanr)
{
  if (dmanr <= 3)
    outb(dmanr,  _DMA1_MASK_REG);
  else
    outb(dmanr & 3, _DMA2_MASK_REG);
}

static __inline__ void clear_dma_flipflop(unsigned int dmanr)
{
  if (dmanr <= 3)
    outb(0,  _DMA1_CLEAR_FF_REG);
  else
    outb(0,  _DMA2_CLEAR_FF_REG);
}

static __inline__ void disable_dma(unsigned int dmanr)
{
  if (dmanr<=3)
    outb(dmanr | 4,  _DMA1_MASK_REG);
  else
    outb((dmanr & 3) | 4,  _DMA2_MASK_REG);
}

static __inline__ void set_dma_mode(unsigned int dmanr, char mode)
{
  if (dmanr<=3)
    outb(mode | dmanr,  _DMA1_MODE_REG);
  else
    outb(mode | (dmanr&3),  _DMA2_MODE_REG);
}

static __inline__ void set_dma_page(unsigned int dmanr, char pagenr)
{
  
  switch(dmanr) {
  case 0:
    outb(pagenr, _DMA_PAGE_0);
    break;
  case 1:
    outb(pagenr, _DMA_PAGE_1);
    break;
  case 2:
    outb(pagenr, _DMA_PAGE_2);
    break;
  case 3:
    outb(pagenr, _DMA_PAGE_3);
    break;
  case 5:
    outb(pagenr & 0xfe, _DMA_PAGE_5);
    break;
  case 6:
    outb(pagenr & 0xfe, _DMA_PAGE_6);
    break;
  case 7:
    outb(pagenr & 0xfe, _DMA_PAGE_7);
    break;
  }
 
}

static __inline__ void set_dma_addr(unsigned int dmanr, unsigned int a)
{
  set_dma_page(dmanr, a>>16);

  if (dmanr <= 3)  {
    outb( a & 0xff, ((dmanr&3)<<1) + _IO_DMA1_BASE );
    outb( (a>>8) & 0xff, ((dmanr&3)<<1) + _IO_DMA1_BASE );
  }  else  {
    outb( (a>>1) & 0xff, ((dmanr&3)<<2) + _IO_DMA2_BASE );
    outb( (a>>9) & 0xff, ((dmanr&3)<<2) + _IO_DMA2_BASE );
  }
}

static __inline__ void set_dma_count(unsigned int dmanr, unsigned int count)
{
  count--;
  if (dmanr <= 3)  {
    
    outb( count & 0xff, ((dmanr&3)<<1) + 1 + _IO_DMA1_BASE );
    outb( (count>>8) & 0xff, ((dmanr&3)<<1) + 1 + _IO_DMA1_BASE );
    
  } else {
    
    outb( (count>>1) & 0xff, ((dmanr&3)<<2) + 2 + _IO_DMA2_BASE );
    outb( (count>>9) & 0xff, ((dmanr&3)<<2) + 2 + _IO_DMA2_BASE );

  }

}

static __inline__ int get_dma_residue(unsigned int dmanr)
{
  unsigned int io_port = 
    (dmanr<=3)? ((dmanr&3)<<1) + 1 + _IO_DMA1_BASE
    : ((dmanr&3)<<2) + 2 + _IO_DMA2_BASE;  
  unsigned short count;  
  count = 1 + inb(io_port);
  count += inb(io_port) << 8;  
  return (dmanr<=3)? count : (count<<1);
  
}

// see dma_functions.c (kernel)
extern int request_dma(unsigned int);
extern void free_dma(unsigned int);

//=========================================================

#endif 
