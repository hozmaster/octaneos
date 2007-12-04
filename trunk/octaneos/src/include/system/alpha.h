//=========================================================
// Copyright (C) 2003, 2007 Berlin Brown
//
// $Id: alpha.h,v 1.9 2005/05/26 00:06:45 bigbinc Exp $
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=========================================================

#ifndef _SYSTEM_ALPHA_H_
#define _SYSTEM_ALPHA_H_

#define _MAX_DMA_CHANNELS	 8

#define _IO_DMA1_BASE        0x00
#define _IO_DMA2_BASE	     0xC0

//==========================================================
// see dma_functions.h
//==========================================================

#define _DMA1_CMD_REG		 0x08
#define _DMA1_STAT_REG		 0x08
#define _DMA1_REQ_REG        0x09
#define _DMA1_MASK_REG		 0x0A
#define _DMA1_MODE_REG		 0x0B
#define _DMA1_CLEAR_FF_REG	 0x0C
#define _DMA1_TEMP_REG       0x0D
#define _DMA1_RESET_REG		 0x0D
#define _DMA1_CLR_MASK_REG   0x0E
#define _DMA1_MASK_ALL_REG   0x0F

#define _DMA2_CMD_REG		 0xD0
#define _DMA2_STAT_REG		 0xD0
#define _DMA2_REQ_REG        0xD2
#define _DMA2_MASK_REG		 0xD4
#define _DMA2_MODE_REG		 0xD6
#define _DMA2_CLEAR_FF_REG	 0xD8
#define _DMA2_TEMP_REG       0xDA
#define _DMA2_RESET_REG		 0xDA
#define _DMA2_CLR_MASK_REG       0xDC
#define _DMA2_MASK_ALL_REG       0xDE

#define _DMA_ADDR_0              0x00
#define _DMA_ADDR_1              0x02
#define _DMA_ADDR_2              0x04
#define _DMA_ADDR_3              0x06
#define _DMA_ADDR_4              0xC0
#define _DMA_ADDR_5              0xC4
#define _DMA_ADDR_6              0xC8
#define _DMA_ADDR_7              0xCC

#define _DMA_CNT_0               0x01
#define _DMA_CNT_1               0x03
#define _DMA_CNT_2               0x05
#define _DMA_CNT_3               0x07
#define _DMA_CNT_4               0xC2
#define _DMA_CNT_5               0xC6
#define _DMA_CNT_6               0xCA
#define _DMA_CNT_7               0xCE

#define _DMA_PAGE_0              0x87
#define _DMA_PAGE_1              0x83
#define _DMA_PAGE_2              0x81
#define _DMA_PAGE_3              0x82
#define _DMA_PAGE_5              0x8B
#define _DMA_PAGE_6              0x89
#define _DMA_PAGE_7              0x8A

#define _DMA_MODE_READ	         0x44
#define _DMA_MODE_WRITE	         0x48
#define _DMA_MODE_CASCADE        0xC0

#define DMA_READ                 0x46
#define DMA_WRITE                0x4A

#define MAX_DMA_CHANNELS         8

#define FDC_TYPE_STD             0x80
#define FDC_TYPE_82077           0x90

#endif 
