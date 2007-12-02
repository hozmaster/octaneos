/*
 * floppy.h
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
 */

#ifndef _OCTANE_FLOPPY_H
#define _OCTANE_FLOPPY_H

#define FLOPPY_IRQ 6
#define FLOPPY_DMA 2

/*
 * The following constants define the base address 
 * for the primary and secondary floppydrive controllers.
 * Drive A (fd0 or f0) is usually on the primary controller.
 *
 * STATUS_REG_A            0x3f20 + 0x0000 // PS2 SYSTEMS
 * STATUS_REG_B            0x3f20 + 0x0001 // PS2 SYSTEMS
 * DIGITAL_OUTPUT_REG      0x3f20 + 0x0002
 * MAIN_STATUS_REG         0x3f20 + 0x0004
 * DATA_RATE_SELECT_REG    0x3f20 + 0x0004 // PS2 SYSTEMS
 * DATA_REGISTER           0x3f20 + 0x0005
 * DITAL_INPUT_REG         0x3f20 + 0x0007 // AT SYSTEMS
 * CONFIG_CONTROL_REG      0x3f20 + 0x0007 // AT SYSTEMS
 */

// (0x370, Base port of the secondary controller)
#define FDC_DOR          0x3f2 /* Base port of the primary controller  */
#define FDC_STATUS       0x3f4 /* RW: Digital Output Register          */
#define FDC_DATA         0x3f5 /* RW: Data Register                    */
#define FDC_DIR          0x3f7 /* R : Digital Input Register           */
#define FDC_DCR          0x3f7 /* W : Configuration Control Register   */

// fdc status data

#define _FDC_STATUS_BUSYMASK 0x0F
#define _FDC_STATUS_BUSY     0x10
#define _FDC_STATUS_DMA      0x20
#define _FDC_STATUS_DIR      0x40
#define _FDC_STATUS_READY    0x80

#define FD_RECALIBRATE          0x07
#define FD_SEEK                 0x0F
#define FD_READ                 0xE6
#define FD_WRITE                0xC5
#define FD_SENSEI               0x08
#define FD_SPECIFY              0x03
#define FD_FORMAT               0x4D
#define FD_VERSION              0x10
#define FD_CONFIGURE            0x13
#define FD_PERPENDICULAR        0x12

#define FDCLRPRM 0              // clear user-defined parameters 
#define FDSETPRM 1              // set user-defined parameters for current media 
#define FDDEFPRM 2              // set user-defined parameters until explicitly cleared 
#define FDGETPRM 3              // get disk parameters 
#define	FDMSGON  4              // issue kernel messages on media type change 
#define	FDMSGOFF 5              // don't issue kernel messages on media type change 
#define FDFMTBEG 6              // begin formatting a disk 
#define	FDFMTTRK 7              // format the specified track 
#define FDFMTEND 8              // end formatting a disk 
#define FDSETEMSGTRESH	10	    // set fdc error reporting treshold 
#define FDFLUSH  11             // flush buffers for media; either for verifying media, or for
                                // handling a media change without closing the file
		                        // descriptor 

#define FD_FILL_BYTE 0xF6       // format fill byte 

#define FORMAT_NONE     0       // no format request 
#define FORMAT_WAIT	    1       // format request is waiting 
#define FORMAT_BUSY	    2       // formatting in progress 
#define FORMAT_OKAY	    3       // successful completion 
#define FORMAT_ERROR    4	    // formatting error


/* Bits of main status register */
#define STATUS_BUSYMASK	0x0F		/* drive busy mask */
#define STATUS_BUSY	0x10		/* FDC busy */
#define STATUS_DMA	0x20		/* 0- DMA mode */
#define STATUS_DIR	0x40		/* 0- cpu->fdc */
#define STATUS_READY	0x80		/* Data reg ready */

/* Bits of FD_ST0 */
#define ST0_DS		0x03		/* drive select mask */
#define ST0_HA		0x04		/* Head (Address) */
#define ST0_NR		0x08		/* Not Ready */
#define ST0_ECE		0x10		/* Equipment chech error */
#define ST0_SE		0x20		/* Seek end */
#define ST0_INTR	0xC0		/* Interrupt code mask */

/* Bits of FD_ST1 */
#define ST1_MAM		0x01		/* Missing Address Mark */
#define ST1_WP		0x02		/* Write Protect */
#define ST1_ND		0x04		/* No Data - unreadable */
#define ST1_OR		0x10		/* OverRun */
#define ST1_CRC		0x20		/* CRC error in data or addr */
#define ST1_EOC		0x80		/* End Of Cylinder */

/* Bits of FD_ST2 */
#define ST2_MAM		0x01		/* Missing Addess Mark (again) */
#define ST2_BC		0x02		/* Bad Cylinder */
#define ST2_SNS		0x04		/* Scan Not Satisfied */
#define ST2_SEH		0x08		/* Scan Equal Hit */
#define ST2_WC		0x10		/* Wrong Cylinder */
#define ST2_CRC		0x20		/* CRC error in data field */
#define ST2_CM		0x40		/* Control Mark = deleted */

/* Bits of FD_ST3 */
#define ST3_HA		0x04		/* Head (Address) */
#define ST3_TZ		0x10		/* Track Zero signal (1=track 0) */
#define ST3_WP		0x40		/* Write Protect */

/* Values for FD_COMMAND */
#define FD_RECALIBRATE		0x07	/* move to track 0 */
#define FD_SEEK			0x0F	/* seek track */
#define FD_READ			0xE6	/* read with MT, MFM, SKip deleted */
#define FD_WRITE		0xC5	/* write with MT, MFM */
#define FD_SENSEI		0x08	/* Sense Interrupt Status */
#define FD_SPECIFY		0x03	/* specify HUT etc */
#define FD_FORMAT		0x4D	/* format one track */
#define FD_VERSION		0x10	/* get version code */
#define FD_CONFIGURE		0x13	/* configure FIFO operation */
#define FD_PERPENDICULAR	0x12	/* perpendicular r/w mode */

// DMA commands
#define DMA_READ	0x46
#define DMA_WRITE	0x4A

/* FDC version return types */
#define FDC_TYPE_STD	0x80	/* normal 8272A clone FDC */
#define FDC_TYPE_82077	0x90	/* FIFO + perpendicular support */

struct floppy_struct {
	unsigned int size,		/* nr of 512-byte sectors total */
			sect,		/* sectors per track */
			head,		/* nr of heads */
			track,		/* nr of tracks */
			stretch;	/* !=0 means double track steps */
	unsigned char	gap,		/* gap1 size */
			rate,		/* data rate. |= 0x40 for perpendicular */
			spec1,		/* stepping rate, head unload time */
			fmt_gap;	/* gap2 size */
	char 	      * name; /* used only for predefined formats */
};

struct format_descr {
	unsigned int device,head,track;
};


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

#endif // End of Floppy.h
