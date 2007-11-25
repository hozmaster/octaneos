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
 * 
 * (Structures defined by: Kees Bot, 2000)
 */

/* Registers used in an PC real mode call for BIOS or DOS services.  A
 * driver is called through the vector if the interrupt number is zero.
 */
union reg86 {
    struct l {
	u32_t	ef;             /* 32 bit flags (output only) */
	u32_t	vec;            /* Driver vector (input only) */
	u32_t	_ds_es[1];
	u32_t	eax;            /* 32 bit general registers */
	u32_t	ebx;
	u32_t	ecx;
	u32_t	edx;
	u32_t	esi;
	u32_t	edi;
	u32_t	ebp;
    } l;
    struct w {
	u16_t	f, _ef[1];      /* 16 bit flags (output only) */
	u16_t	off, seg;       /* Driver vector (input only) */
	u16_t	ds, es;         /* DS and ES real mode segment regs */
	u16_t	ax, _eax[1];    /* 16 bit general registers */
	u16_t	bx, _ebx[1];
	u16_t	cx, _ecx[1];
	u16_t	dx, _edx[1];
	u16_t	si, _esi[1];
	u16_t	di, _edi[1];
	u16_t	bp, _ebp[1];
    } w;
    struct b {
	u8_t	intno, _intno[3];   /* Interrupt number (input only) */
	u8_t	_vec[4];
	u8_t	_ds_es[4];
	u8_t	al, ah, _eax[2];    /* 8 bit general registers */
	u8_t	bl, bh, _ebx[2];
	u8_t	cl, ch, _ecx[2];
	u8_t	dl, dh, _edx[2];
	u8_t	_esi[4];
	u8_t	_edi[4];
	u8_t	_ebp[4];
    } b;
};


