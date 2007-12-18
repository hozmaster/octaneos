/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: exceptions.S
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

#define __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/segment.h>
#include <system/assembly_functions.h>

#***********************************************************
# Note: see lowlevel.S for same list
# Stack pointer register offsets
#***********************************************************
REGI_EBX    = 0x00
REGI_ECX    = 0x04
REGI_EDX    = 0x08

REGI_ESI    = 0x0C
REGI_EDI    = 0x10
REGI_EBP    = 0x14
REGI_EAX    = 0x18
REGI_DS     = 0x1C
REGI_ES     = 0x20
REGI_FS     = 0x24
REGI_GS     = 0x28

REGI_OLD_EAX    = 0x2C
REGI_EIP    = 0x30
REGI_CS     = 0x34
REGI_EFLAGS = 0x38

#define SAVE_ALL \
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
		
#***********************************************************
# Register Restore all
#***********************************************************
#define RESTORE_ALL \
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

#***********************************************************		
# System Call:
# EAX represents the offset in the sys_call table
# set typically by the calling system function
#***********************************************************
ALIGN
ENTRY(system_call)
    SAVE_ALL
    cmpl __NO_SYS_CALLS, %eax
    jae end_sys_call
	#*******************************************************
    # see:   scheduler.c
    # make the system call from the function pointer array
    #*******************************************************
    call *SYMBOL_NAME(system_call_table)(,%eax,4)
    movl %eax, REGI_EAX(%esp)
end_sys_call:
    RESTORE_ALL

#======================== INT_0001 ==========================

ENTRY(load_divide_error)
    SAVE_ALL
    call handler_divide_error
    RESTORE_ALL

#======================== INT_0002 ==========================

ENTRY(load_debug_error)
    SAVE_ALL
    call handler_debug_error
    RESTORE_ALL

#======================== INT_0003 ==========================

ENTRY(load_nmi_error)
    SAVE_ALL
    call handler_nmi_error
    RESTORE_ALL


#======================== INT_0004 ==========================

ENTRY(load_breakpoint_error)
    SAVE_ALL
    call handler_breakpoint_error
    RESTORE_ALL


#======================== INT_0005 ==========================

ENTRY(load_overflow_error)
    SAVE_ALL
    call handler_overflow_error
    RESTORE_ALL


#======================== INT_0006 ==========================

ENTRY(load_boundscheck_error)
    SAVE_ALL
    call handler_bounds_error
    RESTORE_ALL


#======================== INT_0007 ==========================

ENTRY(load_invalidop_error)
    SAVE_ALL
    call handler_invalid_opcode_error
    RESTORE_ALL


#======================== INT_0008 ==========================

ENTRY(load_device_na_error)
    SAVE_ALL
    call handler_device_error
    RESTORE_ALL

#======================== INT_0009 ==========================

ENTRY(load_doublefault_error)
    SAVE_ALL
    call handler_doublefault_error
    RESTORE_ALL

#======================== INT_0010 ==========================

ENTRY(load_segmentover_error)
    SAVE_ALL
    call handler_divide_error
    RESTORE_ALL

#======================== INT_0011 ==========================

ENTRY(load_invalidtss_error)
    SAVE_ALL
    call handler_tss_error
    RESTORE_ALL

#======================== INT_0012 ==========================

ENTRY(load_segment_error)
    SAVE_ALL
    call handler_segmentna_error
    RESTORE_ALL

#======================== INT_0013 ==========================

ENTRY(load_stack_error)
    SAVE_ALL
    call handler_stackexception_error
    RESTORE_ALL

#======================== INT_0014 ==========================

ENTRY(load_protection_error)
    SAVE_ALL
    call handler_triple_error
    RESTORE_ALL

#======================== INT_0015 ==========================

ENTRY(load_pagefault_error)
    SAVE_ALL
    call handler_pagefault_error
    RESTORE_ALL


#======================== INT_0016 ==========================

ENTRY(load_intel_error)
    SAVE_ALL
    call handler_intelx_error
    RESTORE_ALL

#======================== INT_0017 ==========================

ENTRY(load_coprocessor_error)
    SAVE_ALL
    call handler_coprocessor_error
    RESTORE_ALL

#==== End Exceptions  =======================================
