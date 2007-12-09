#ifndef __OCTANE_GFP_H
#define __OCTANE_GFP_H

/* Zone modifiers in GFP_ZONEMASK (see linux/mmzone.h - low four bits) */
#define __GFP_DMA	    0x01
#define __GFP_HIGHMEM	0x02

/* Action modifiers - doesn't change the zoning */
#define __GFP_WAIT	0x10	 /* Can wait and reschedule? */
#define __GFP_HIGH	0x20	 /* Should access emergency pools? */
#define __GFP_IO	0x40	 /* Can start low memory physical IO? */
#define __GFP_HIGHIO	0x80 /* Can start high mem physical IO? */
#define __GFP_FS	0x100	 /* Can call down to low-level FS? */

#define GFP_NOHIGHIO	(             __GFP_WAIT | __GFP_IO)
#define GFP_NOIO	(             __GFP_WAIT)
#define GFP_NOFS	(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO)
#define GFP_ATOMIC	(__GFP_HIGH)
#define GFP_USER	(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_HIGHUSER	(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS | __GFP_HIGHMEM)
#define GFP_KERNEL	(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_NFS		(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_KSWAPD	(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)

/* Flag - indicates that the buffer will be suitable for DMA.  Ignored on some
   platforms, used as appropriate on others */

#define GFP_DMA		__GFP_DMA

// TODO: add other functions

#endif
