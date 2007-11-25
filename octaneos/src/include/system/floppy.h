//
// Copyright (C) 2003, 2007 Berlin Brown
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
//

typedef enum {
    FDC_WRITE = 0,
    FDC_READ  = 1
} FdcTransfer;


/* A Cylinder/Head/Sector address */
typedef struct {
    unsigned c, h, s;
} cylinder_head, cylinder_head_obj*;

/* The format of a floppy disk. */
typedef struct {
    unsigned    size;        /* Total number of sectors on disk    */
    unsigned    sec_per_trk; /* Number of sectors per track        */
    unsigned    heads;       /* Number of heads (sides)            */
    unsigned    tracks;      /* Number of tracks                   */
    unsigned    strech;      /* 1 if tracks are to be doubled      */
    BYTE        gap3;        /* GAP3 to use for reading/writing    */
    BYTE        rate;        /* Sector size and data transfer rate */
    BYTE        sr_hut;      /* Step Rate and Head Unload Time     */
    BYTE        gap3fmt;     /* GAP3 to use for formatting         */
    const char *name;        /* Disk format name                   */
} floppy_format, floppy_format_obj*;


/* Parameters used to handle a floppy drive. */
typedef struct {
    unsigned    cmos_type;  /* Drive type read from CMOS            */
    unsigned    hlt;        /* Head Load Time (in SPECIFY format)   */
    unsigned    spin_up;    /* Time needed for spinning up [us]     */
    unsigned    spin_down;  /* Time needed for spinning down [us]   */
    unsigned    sel_delay;  /* Select delay [us]                    */
    unsigned    int_tmout;  /* Interrupt timeout [us]               */
    unsigned    detect[8];  /* Autodetect formats                   */
    unsigned    native_fmt; /* Native disk format for drive         */
    const char *name;       /* Name displayed for the drive type    */
} DriveParams, drive_params_obj*;

/* Status structure for a Floppy Disk Drive */
typedef struct
{
    struct Fdc          *fdc;       /* Controller of this drive (see fdc.c)  */
    const  DriveParams  *dp;        /* Parameters for this drive             */
    const  FloppyFormat *fmt;       /* Current format of the floppy          */
    unsigned             number;    /* Drive number per FDC [0..MAXDRIVES-1] */
    volatile unsigned    flags;     /* See the DF_* enums in "fdc.c"         */
    unsigned             track;     /* Current floppy track sought           */
    void                *spin_down; /* Event handle for motor spindown       */
} floppy_drive_stat, floppy_drive_stat_obj*;

