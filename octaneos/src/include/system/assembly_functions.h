//
// Copyright (C) 2003, 2007 Berlin Brown
// 
// $Id: assembly_functions.h,v 1.3 2005/05/26 00:06:45 bigbinc Exp $
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

#ifndef _ASSEMBLY_FUNCTIONS_H_
#define _ASSEMBLY_FUNCTIONS_H_

#define __ASSEMBLY__

//
// this macro may or may not be different
//
// than the on in exceptions.S
//

#define REG_SAVE_ALL \
        cld; \
        push %gs; \
        push %fs; \
        push %es; \
        push %ds; \
        pushl %eax; \
        pushl %ebp; \
        pushl %edi; \
        pushl %esi; \
        pushl %edx; \
        pushl %ecx; \
        pushl %ebx; \
        movl $(__KERNEL_DS),%edx; \
        mov %dx,%ds; \
        mov %dx,%es; \
        movl $(__USER_DS),%edx; \
        mov %dx,%fs;


#define REG_RESTORE_ALL \
	popl %ebx; \
        popl %ecx; \
        popl %edx; \
        popl %esi; \
        popl %edi; \
        popl %ebp; \
        popl %eax; \
        pop %ds; \
        pop %es; \
        pop %fs; \
        pop %gs; \
        addl $4,%esp; \
        iret

#endif
