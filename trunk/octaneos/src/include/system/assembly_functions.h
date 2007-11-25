//
// Copyright (C) 2003, 2007 Berlin Brown
// 
// $Id: assembly_functions.h,v 1.3 2005/05/26 00:06:45 bigbinc Exp $
//

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
