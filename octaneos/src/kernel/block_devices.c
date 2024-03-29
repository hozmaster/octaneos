/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: block_devices.c (see ll_rw_block in linux kernel)
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

#include <system/system.h>
#include <linux/errno.h>
#include <linux/page.h>
#include <linux/kernel_stat.h>

#include <system/major_devices.h>
#include <linux/block_devices.h>

#include <linux/string.h>
#include <linux/signal.h>
#include <linux/locks.h>
#include <asm/segment_fs.h>

#define NBUF 64

// This specifies how many sectors to read ahead on the disk.
int read_ahead[MAX_BLKDEV] = {0, };

struct blk_dev_struct blk_dev[MAX_BLKDEV] = {
	{ NULL, NULL },		/* no_dev */
	{ NULL, NULL },		/* dev mem */
	{ NULL, NULL },		/* dev fd */
	{ NULL, NULL },		/* dev hd */
	{ NULL, NULL },		/* dev ttyx */
	{ NULL, NULL },		/* dev tty */
	{ NULL, NULL },		/* dev lp */
	{ NULL, NULL },		/* dev pipes */
	{ NULL, NULL },		/* dev sd */
	{ NULL, NULL }		/* dev st */
};

int *blk_size[MAX_BLKDEV] = { NULL, NULL, };

/*
 * blksize_size contains the size of all block-devices:
 *
 * blksize_size[MAJOR][MINOR]
 *
 * if (!blksize_size[MAJOR]) then 1024 bytes is assumed.
 */
int *blksize_size[MAX_BLKDEV] = { NULL, NULL, };


int block_write(struct inode *inode, struct file *filp, 
				char *buf, int count) {
	int blocksize, blocksize_bits, i, j;
	int block, blocks;
	int offset;
	int chars;
	int written = 0;
	int cluster_list[4];
	struct buffer_head * bhlist[NBUF];
	int blocks_per_cluster;
	unsigned int size;
	unsigned int dev;
	struct buffer_head * bh;
	register char * p;

	dev = inode->i_rdev;
	blocksize = BLOCK_SIZE;
	if (blksize_size[MAJOR(dev)] && blksize_size[MAJOR(dev)][MINOR(dev)])
		blocksize = blksize_size[MAJOR(dev)][MINOR(dev)];

	i = blocksize;
	blocksize_bits = 0;
	while(i != 1) {
		blocksize_bits++;
		i >>= 1;
	}

	blocks_per_cluster = PAGE_SIZE / blocksize;

	block = filp->f_pos >> blocksize_bits;
	offset = filp->f_pos & (blocksize-1);

	if (blk_size[MAJOR(dev)])
		size = (blk_size[MAJOR(dev)][MINOR(dev)] << BLOCK_SIZE_BITS) >> blocksize_bits;
	else
		size = INT_MAX;
	while (count>0) {
		if (block >= size)
			return written;
		chars = blocksize - offset;
		if (chars > count)
			chars=count;

		for(i=0; i<blocks_per_cluster; i++) cluster_list[i] = block+i;
		if((block % blocks_per_cluster) == 0)
		  generate_cluster(dev, cluster_list, blocksize);
		bh = getblk(dev, block, blocksize);

		if (chars != blocksize && !bh->b_uptodate) {
		  if(!filp->f_reada ||
		     !read_ahead[MAJOR(dev)]) {
		    /* We do this to force the read of a single buffer */
		    brelse(bh);
		    bh = bread(dev,block,blocksize);
		  } else {
		    /* Read-ahead before write */
		    blocks = read_ahead[MAJOR(dev)] / (blocksize >> 9) / 2;
		    if (block + blocks > size) blocks = size - block;
		    if (blocks > NBUF) blocks=NBUF;
		    blocks -= (block % blocks_per_cluster);
		    if(!blocks) blocks = 1;
		    bhlist[0] = bh;
		    for(i=1; i<blocks; i++){
		      if(((i+block) % blocks_per_cluster) == 0) {
			for(j=0; j<blocks_per_cluster; j++) cluster_list[j] = block+i+j;
			generate_cluster(dev, cluster_list, blocksize);
		      };
		      bhlist[i] = getblk (dev, block+i, blocksize);
		      if(!bhlist[i]){
			while(i >= 0) brelse(bhlist[i--]);
			return written? written: -EIO;
		      };
		    };
		    ll_rw_block(READ, blocks, bhlist);
		    for(i=1; i<blocks; i++) brelse(bhlist[i]);
		      
		  };
		};

		block++;
		if (!bh)
			return written?written:-EIO;
		p = offset + bh->b_data;
		offset = 0;
		filp->f_pos += chars;
		written += chars;
		count -= chars;
		memcpy_fromfs(p,buf,chars);
		p += chars;
		buf += chars;
		bh->b_uptodate = 1;
		
		// TODO:
		//mark_buffer_dirty(bh, 0);
		brelse(bh);
	}
	filp->f_reada = 1;
	return written;
}

int block_read(struct inode *inode, struct file *filp, 
			   char *buf, int count) {
	unsigned int block;
	unsigned int offset;
	int blocksize;
	int blocksize_bits, i;
	unsigned int blocks, rblocks, left;
	int bhrequest, uptodate;
	int cluster_list[4];
	int blocks_per_cluster;
	struct buffer_head **bhb, **bhe;
	struct buffer_head *buflist[NBUF];
	struct buffer_head *bhreq[NBUF];
	unsigned int chars;
	unsigned int size;
	unsigned int dev;
	int read;

	dev = inode->i_rdev;
	blocksize = BLOCK_SIZE;
	if (blksize_size[MAJOR(dev)] && blksize_size[MAJOR(dev)][MINOR(dev)])
		blocksize = blksize_size[MAJOR(dev)][MINOR(dev)];
	i = blocksize;
	blocksize_bits = 0;
	while (i != 1) {
		blocksize_bits++;
		i >>= 1;
	}

	offset = filp->f_pos;
	if (blk_size[MAJOR(dev)])
		size = blk_size[MAJOR(dev)][MINOR(dev)] << BLOCK_SIZE_BITS;
	else
		size = INT_MAX;

	blocks_per_cluster = PAGE_SIZE / blocksize;
	if (offset > size)
		left = 0;
	else
		left = size - offset;
	if (left > count)
		left = count;
	if (left <= 0)
		return 0;
	read = 0;
	block = offset >> blocksize_bits;
	offset &= blocksize-1;
	size >>= blocksize_bits;
	rblocks = blocks = (left + offset + blocksize - 1) >> blocksize_bits;
	bhb = bhe = buflist;
	if (filp->f_reada) {
	        if(blocks < read_ahead[MAJOR(dev)] / (blocksize >> 9))
		  blocks = read_ahead[MAJOR(dev)] / (blocksize >> 9);
		if (block + blocks > size)
			blocks = size - block;
		blocks -= (block % blocks_per_cluster);
		if(rblocks > blocks) blocks = rblocks;
		
	}

	/* We do this in a two stage process.  We first try and request
	   as many blocks as we can, then we wait for the first one to
	   complete, and then we try and wrap up as many as are actually
	   done.  This routine is rather generic, in that it can be used
	   in a filesystem by substituting the appropriate function in
	   for getblk.

	   This routine is optimized to make maximum use of the various
	   buffers and caches. */

	do {
		bhrequest = 0;
		uptodate = 1;
		while (blocks) {
			--blocks;
#if 1
			if((block % blocks_per_cluster) == 0) {
			  for(i=0; i<blocks_per_cluster; i++) cluster_list[i] = block+i;
			  generate_cluster(dev, cluster_list, blocksize);
			}
#endif
			*bhb = getblk(dev, block++, blocksize);
			if (*bhb && !(*bhb)->b_uptodate) {
				uptodate = 0;
				bhreq[bhrequest++] = *bhb;
			}

			if (++bhb == &buflist[NBUF])
				bhb = buflist;

			/* If the block we have on hand is uptodate, go ahead
			   and complete processing. */
			if (uptodate)
				break;
			if (bhb == bhe)
				break;
		}

		/* Now request them all */
		if (bhrequest) {
			ll_rw_block(READ, bhrequest, bhreq);
			refill_freelist(blocksize);
		}

		do { /* Finish off all I/O that has actually completed */
			if (*bhe) {
				wait_on_buffer(*bhe);
				if (!(*bhe)->b_uptodate) {	/* read error? */
				        brelse(*bhe);
					if (++bhe == &buflist[NBUF])
					  bhe = buflist;
					left = 0;
					break;
				}
			}			
			if (left < blocksize - offset)
				chars = left;
			else
				chars = blocksize - offset;
			filp->f_pos += chars;
			left -= chars;
			read += chars;
			if (*bhe) {
				memcpy_tofs(buf,offset+(*bhe)->b_data,chars);
				brelse(*bhe);
				buf += chars;
			} else {
				while (chars-->0)
					put_fs_byte(0,buf++);
			}
			offset = 0;
			if (++bhe == &buflist[NBUF])
				bhe = buflist;
		} while (left > 0 && bhe != bhb && (!*bhe || !(*bhe)->b_lock));
	} while (left > 0);

	/* Release the read-ahead blocks */
	while (bhe != bhb) {
		brelse(*bhe);
		if (++bhe == &buflist[NBUF])
			bhe = buflist;
	};
	if (!read)
		return -EIO;
	filp->f_reada = 1;
	return read;
}

int block_fsync(struct inode *inode, struct file *filp) {
	return fsync_dev (inode->i_rdev);
}


/*
 * The request-struct contains all necessary data
 * to load a nr of sectors into memory
 */
static struct request all_requests[NR_REQUEST];

/*
 * used to wait on when there are no free requests
 */
struct wait_queue *wait_for_request = NULL;

/*
 * look for a free request in the first N entries.
 * NOTE: interrupts must be disabled on the way in, and will still
 *       be disabled on the way out.
 */
static inline struct request *get_request(int n, int dev) {

	static struct request *prev_found = NULL, *prev_limit = NULL;
	register struct request *req, *limit;

	if (n <= 0)
		panic("get_request(%d): impossible!\n", n);

	limit = all_requests + n;
	if (limit != prev_limit) {
		prev_limit = limit;
		prev_found = all_requests;
	}
	req = prev_found;
	for (;;) {
		req = ((req > all_requests) ? req : limit) - 1;
		if (req->dev < 0)
			break;
		if (req == prev_found)
			return NULL;
	}
	prev_found = req;
	req->dev = dev;
	return req;
}

/*
 * wait until a free request in the first N entries is available.
 * NOTE: interrupts must be disabled on the way in, and will still
 *       be disabled on the way out.
 */
static inline struct request *get_request_wait(int n, int dev) {

	register struct request *req;
	while ((req = get_request(n, dev)) == NULL) {
		sleep_on(&wait_for_request);
	}

	return req;
}

/* RO fail safe mechanism */

static long ro_bits[MAX_BLKDEV][8];

int is_read_only(int dev) {
	int minor,major;

	major = MAJOR(dev);
	minor = MINOR(dev);
	if (major < 0 || major >= MAX_BLKDEV) return 0;
	return ro_bits[major][minor >> 5] & (1 << (minor & 31));
}

void set_device_ro(int dev,int flag) {
	int minor,major;

	major = MAJOR(dev);
	minor = MINOR(dev);
	if (major < 0 || major >= MAX_BLKDEV) return;
	if (flag) ro_bits[major][minor >> 5] |= 1 << (minor & 31);
	else ro_bits[major][minor >> 5] &= ~(1 << (minor & 31));
}

/*
 * add-request adds a request to the linked list.
 * It disables interrupts so that it can muck with the
 * request-lists in peace.
 */
static void add_request(struct blk_dev_struct *dev, struct request *req) {

	struct request * tmp;
	req->next = NULL;
	cli();

	//if (req->bh)
	//	mark_buffer_clean(req->bh);

	if (!(tmp = dev->current_request)) {
		dev->current_request = req;
		(dev->request_fn)();

		sti();
		return;
	}
	for ( ; tmp->next ; tmp = tmp->next) {

		//if ((IN_ORDER(tmp,req) ||
		//    !IN_ORDER(tmp,tmp->next)) &&
		///   IN_ORDER(req,tmp->next))
		//	break;
	}
	req->next = tmp->next;
	tmp->next = req;
	sti();
}

static void make_request(int major,int rw, struct buffer_head *bh) {

	unsigned int sector, count;
	struct request *req;
	int rw_ahead, max_req;

	// WRITEA/READA is special case - it is not really needed, so if the
	// buffer is locked, we just forget about it, else it's a normal read
	rw_ahead = (rw == READA || rw == WRITEA);
	if (rw_ahead) {
		if (bh->b_lock)
			return;
		if (rw == READA)
			rw = READ;
		else
			rw = WRITE;
	}
	if (rw!=READ && rw!=WRITE) {
		printk("Bad block dev command, must be R/W/RA/WA\n");
		return;
	}
	count = bh->b_size >> 9;
	sector = bh->b_blocknr * count;
	if (blk_size[major])
		if (blk_size[major][MINOR(bh->b_dev)] < (sector + count)>>1) {
			bh->b_dirt = bh->b_uptodate = 0;
			return;
		}

	//lock_buffer(bh);
	if ((rw == WRITE && !bh->b_dirt) || (rw == READ && bh->b_uptodate)) {
		//unlock_buffer(bh);
		return;
	}

	/* we don't allow the write-requests to fill up the queue completely:
	 * we want some room for reads: they take precedence. The last third
	 * of the requests are only for reads.
	 */
	max_req = (rw == READ) ? NR_REQUEST : ((NR_REQUEST*2)/3);

/* big loop: look for a free request. */

repeat:
	cli();

	/* The scsi disk drivers completely remove the request from the queue when
	 * they start processing an entry.  For this reason it is safe to continue
	 * to add links to the top entry for scsi devices.
	 */
	if ((major == HD_MAJOR
	     || major == SCSI_DISK_MAJOR
	     || major == SCSI_CDROM_MAJOR)
	    && (req = blk_dev[major].current_request))
	{
	        if (major == HD_MAJOR)
			req = req->next;
		while (req) {
			if (req->dev == bh->b_dev &&
			    !req->waiting &&
			    req->cmd == rw &&
			    req->sector + req->nr_sectors == sector &&
			    req->nr_sectors < 244)
			{
				req->bhtail->b_reqnext = bh;
				req->bhtail = bh;
				req->nr_sectors += count;

				//mark_buffer_clean(bh);
				sti();
				return;
			}

			if (req->dev == bh->b_dev &&
			    !req->waiting &&
			    req->cmd == rw &&
			    req->sector - count == sector &&
			    req->nr_sectors < 244)
			{
			    	req->nr_sectors += count;
			    	bh->b_reqnext = req->bh;
			    	req->buffer = bh->b_data;
			    	req->current_nr_sectors = count;
			    	req->sector = sector;

					//mark_buffer_clean(bh);
			    	req->bh = bh;
			    	sti();
			    	return;
			}    

			req = req->next;
		}
	}

	/* find an unused request. */
	req = get_request(max_req, bh->b_dev);

	/* if no request available: if rw_ahead, forget it; otherwise try again. */
	if (! req) {
		if (rw_ahead) {
			sti();
			//unlock_buffer(bh);
			return;
		}
		sleep_on(&wait_for_request);
		sti();
		goto repeat;
	}

	/* we found a request. */
	sti();

	/* fill up the request-info, and add it to the queue */
	req->cmd = rw;
	req->errors = 0;
	req->sector = sector;
	req->nr_sectors = count;
	req->current_nr_sectors = count;
	req->buffer = bh->b_data;
	req->waiting = NULL;
	req->bh = bh;
	req->bhtail = bh;
	req->next = NULL;
	add_request(major+blk_dev,req);
}

void ll_rw_page(int rw, int dev, int page, char *buffer) {

	struct request *req;
	unsigned int major = MAJOR(dev);

	if (major >= MAX_BLKDEV || !(blk_dev[major].request_fn)) {
		printk("Trying to read nonexistent block-device %04x (%d)\n",dev,page*8);
		return;
	}
	if (rw!=READ && rw!=WRITE) {
		panic("Bad block dev command, must be R/W");
	}

	if (rw == WRITE && is_read_only(dev)) {
		printk("Can't page to read-only device 0x%X\n",dev);
		return;
	}
	cli();
	req = get_request_wait(NR_REQUEST, dev);
	sti();

	/* fill up the request-info, and add it to the queue */
	req->cmd = rw;
	req->errors = 0;
	req->sector = page<<3;
	req->nr_sectors = 8;
	req->current_nr_sectors = 8;
	req->buffer = buffer;
	req->waiting = current;
	req->bh = NULL;
	req->next = NULL;

	current->swapping = 1;
	current->state = TASK_SWAPPING;
	add_request(major+blk_dev, req);
	/* The I/O may have inadvertently chagned the task state.
	   Make sure we really wait until the I/O is done */
	if (current->swapping) {
		current->state = TASK_SWAPPING;
	}
	schedule();
}

/** 
 * This function can be used to request a number of buffers from a block
 * device. Currently the only restriction is that all buffers must belong to
 * the same device 
 */
void ll_rw_block(int rw, int nr, struct buffer_head *bh[]) {

	unsigned int major;
	struct request plug;
	int plugged;
	int correct_size;
	struct blk_dev_struct *dev;
	int i;

	/* Make sure that the first block contains something reasonable */
	while (!*bh) {
		bh++;
		if (--nr <= 0)
			return;
	};

	dev = NULL;
	if ((major = MAJOR(bh[0]->b_dev)) < MAX_BLKDEV) {
		dev = blk_dev + major;
	}

	if (!dev || !dev->request_fn) {
		printk("ll_rw_block: Trying to read nonexistent block-device %04lX (%ld)\n",
		       (unsigned long) bh[0]->b_dev, bh[0]->b_blocknr);
		goto sorry;
	}

	/* Determine correct block size for this device.  */
	correct_size = BLOCK_SIZE;
	if (blksize_size[major]) {
		i = blksize_size[major][MINOR(bh[0]->b_dev)];
		if (i)
			correct_size = i;
	}

	/* Verify requested block sizees.  */
	for (i = 0; i < nr; i++) {
		if (bh[i] && bh[i]->b_size != correct_size) {
			printk(
			"ll_rw_block: only %d-char blocks implemented (%lu)\n",
			       correct_size, bh[i]->b_size);
			goto sorry;
		}
	}

	if ((rw == WRITE || rw == WRITEA) && is_read_only(bh[0]->b_dev)) {
		printk("Can't write to read-only device 0x%X\n",bh[0]->b_dev);
		goto sorry;
	}

	/* If there are no pending requests for this device, then we insert
	   a dummy request for that device.  This will prevent the request
	   from starting until we have shoved all of the blocks into the
	   queue, and then we let it rip.  */

	plugged = 0;
	cli();
	if (!dev->current_request && nr > 1) {
		dev->current_request = &plug;
		plug.dev = -1;
		plug.next = NULL;
		plugged = 1;
	}
	sti();
	for (i = 0; i < nr; i++) {
		if (bh[i]) {
			bh[i]->b_req = 1;
			make_request(major, rw, bh[i]);
			if (rw == READ || rw == READA)
				kstat.pgpgin++;
			else
				kstat.pgpgout++;
		}
	}
	if (plugged) {
		cli();
		dev->current_request = plug.next;
		(dev->request_fn)();
		sti();
	}
	return;

      sorry:
	for (i = 0; i < nr; i++) {
		if (bh[i])
			bh[i]->b_dirt = bh[i]->b_uptodate = 0;
	}
	return;
}

void ll_rw_swap_file(int rw, int dev, unsigned int *b, int nb, char *buf) {

	int i;
	int buffersize;
	struct request * req;
	unsigned int major = MAJOR(dev);

	if (major >= MAX_BLKDEV || !(blk_dev[major].request_fn)) {
		printk("ll_rw_swap_file: trying to swap nonexistent block-device\n");
		return;
	}

	if (rw!=READ && rw!=WRITE) {
		printk("ll_rw_swap: bad block dev command, must be R/W");
		return;
	}
	if (rw == WRITE && is_read_only(dev)) {
		printk("Can't swap to read-only device 0x%X\n",dev);
		return;
	}
	
	buffersize = PAGE_SIZE / nb;

	for (i=0; i<nb; i++, buf += buffersize)
	{
		cli();
		req = get_request_wait(NR_REQUEST, dev);
		sti();
		req->cmd = rw;
		req->errors = 0;
		req->sector = (b[i] * buffersize) >> 9;
		req->nr_sectors = buffersize >> 9;
		req->current_nr_sectors = buffersize >> 9;
		req->buffer = buf;
		req->waiting = current;
		req->bh = NULL;
		req->next = NULL;
		current->swapping = 1;
		current->state = TASK_UNINTERRUPTIBLE;
		add_request(major+blk_dev,req);
		/* The I/O may have inadvertently chagned the task state.
		   Make sure we really wait until the I/O is done */
		if (current->swapping) current->state = TASK_UNINTERRUPTIBLE;
		schedule();
	}
}

long blk_dev_init(long mem_start, long mem_end) {

	struct request *req;
	req = all_requests + NR_REQUEST;
	while (--req >= all_requests) {
		req->dev = -1;
		req->next = NULL;
	}

	memset(ro_bits,0,sizeof(ro_bits));

	//if (ramdisk_size)
	//	mem_start += rd_init(mem_start, ramdisk_size * 1024);
	return mem_start;
}


