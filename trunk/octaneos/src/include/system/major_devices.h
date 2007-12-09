/*
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
 */

#ifndef _OCTANE_MAJOR_DEV_H
#define _OCTANE_MAJOR_DEV_H

//************************************************
// Major Devices
//************************************************

#define MAJOR(a) (int)((unsigned short)(a) >> 8)
#define MINOR(a) (int)((unsigned short)(a) & 0xFF)
#define MKDEV(a,b) ((int)((((a) & 0xff) << 8) | ((b) & 0xff)))

#undef NR_OPEN
#define NR_OPEN           256

#define NR_INODE          2048	// this should be bigger than NR_FILE
#define NR_FILE           1024	// this can well be larger on a larger system
#define NR_SUPER          32
#define NR_IHASH          131
#define NR_FILE_LOCKS     64
#define BLOCK_SIZE        1024
#define BLOCK_SIZE_BITS   10

#define MAY_EXEC  1
#define MAY_WRITE 2
#define MAY_READ  4

#define READ      0
#define WRITE     1
#define READA     2		// read-ahead - don't pause
#define WRITEA    3	    // "write-ahead" - silly, but somewhat useful

/*
 * assignments
 *
 * devices are as follows (same as linux)
 *
 *      character              block                  comments
 *      --------------------   --------------------   --------------------
 *  0 - unnamed                unnamed                minor 0 = true nodev
 *  1 - /dev/mem               ramdisk
 *  2 -                        floppy
 *  3 -                        hd
 *  4 - /dev/tty*
 *  5 - /dev/tty; /dev/cua*
 *  6 - lp
 *  7 -                                               UNUSED
 *  8 -                        scsi disk
 *  9 - scsi tape
 * 10 - mice
 * 11 -                        scsi cdrom
 * 12 - qic02 tape
 * 13 -                        xt disk
 * 14 - sound card
 * 15 -                        cdu31a cdrom
 * 16 - sockets
 * 17 - af_unix
 * 18 - af_inet
 * 19 -                                               UNUSED
 * 20 -                                               UNUSED
 * 21 - scsi generic
 * 22 -                        (at2disk)
 * 23 -                        mitsumi cdrom
 * 24 -	                       sony535 cdrom
 * 25 -                        matsushita cdrom       minors 0..3
 * 26 -                        matsushita cdrom 2     minors 0..3
 * 27 - qic117 tape            matsushita cdrom 3     minors 0..3
 * 28 -                        matsushita cdrom 4     minors 0..3
 */
#define UNNAMED_MAJOR 	            0
#define MEM_MAJOR	                1
#define FLOPPY_MAJOR	            2
#define HD_MAJOR	                3
#define TTY_MAJOR	                4
#define TTYAUX_MAJOR	            5
#define LP_MAJOR	                6
/* unused: 7 */
#define SCSI_DISK_MAJOR	            8
#define SCSI_TAPE_MAJOR	            9
#define MOUSE_MAJOR	                10
#define SCSI_CDROM_MAJOR            11
#define QIC02_TAPE_MAJOR            12
#define XT_DISK_MAJOR	            13
#define SOUND_MAJOR                 14
#define CDU31A_CDROM_MAJOR          15
#define SOCKET_MAJOR 	            16
#define AF_UNIX_MAJOR	            17
#define AF_INET_MAJOR	            18
/* unused: 19, 20 */
#define SCSI_GENERIC_MAJOR          21
/* unused: 22 */
#define MITSUMI_CDROM_MAJOR         23
#define CDU535_CDROM_MAJOR          24
#define MATSUSHITA_CDROM_MAJOR      25
#define MATSUSHITA_CDROM2_MAJOR     26
#define MATSUSHITA_CDROM3_MAJOR     27
#define MATSUSHITA_CDROM4_MAJOR     28
#define QIC117_TAPE_MAJOR           27

#endif
