/*
 * Copyright (C) 2003, 2007 Berlin Brown (berlin.brown@gmail.com)
 *
 * File: bad_interrupts.S
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

# Functions Defined:
# Use FIRST PIC [ 0 to 7 ]
#  for SECOND PIC [ 8 - 16 ]
# bad_interrupt_00
# bad_interrupt_01
# bad_interrupt_02
# bad_interrupt_03
# bad_interrupt_04
# bad_interrupt_05
# bad_interrupt_06
# bad_interrupt_07

# bad_interrupt_08
# bad_interrupt_09
# bad_interrupt_10
# bad_interrupt_11
# bad_interrupt_12
# bad_interrupt_13
# bad_interrupt_14
# bad_interrupt_15

#  ============================= 00 ========================

.align 4
ENTRY(bad_interrupt_00)
    # /// Save Most registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x01,SYMBOL_NAME(cache_21)     # /// MASK CODE
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# ============================= 01 ========================

.align 4
ENTRY(bad_interrupt_01)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x02,SYMBOL_NAME(cache_21)     # /// MASK CODE
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# ============================= 02 ========================

.align 4
ENTRY(bad_interrupt_02)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x04,SYMBOL_NAME(cache_21)
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# ============================= 03 ========================

.align 4
ENTRY(bad_interrupt_03)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x08,SYMBOL_NAME(cache_21)     # /// MASK CODE
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 04 ========================

.align 4
ENTRY(bad_interrupt_04)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x10,SYMBOL_NAME(cache_21)
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 05 ========================

.align 4
ENTRY(bad_interrupt_05)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x20,SYMBOL_NAME(cache_21)
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 06 ========================

.align 4
ENTRY(bad_interrupt_06)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x40,SYMBOL_NAME(cache_21)
        movb SYMBOL_NAME(cache_21),%al
        outb %al,$0x21
        jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 07 ========================

.align 4
ENTRY(bad_interrupt_07)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    inb $0x21,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x80,SYMBOL_NAME(cache_21)
    movb SYMBOL_NAME(cache_21),%al
    outb %al,$0x21
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0x20

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 08 ========================

.align 4
ENTRY(bad_interrupt_08)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es


    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x01,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 09 ========================

.align 4
ENTRY(bad_interrupt_09_02)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x02,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 10 ========================

.align 4
ENTRY(bad_interrupt_10_04)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x04,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 11 ========================

.align 4
ENTRY(bad_interrupt_11_08)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x08,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 12 ========================

.align 4
ENTRY(bad_interrupt_12_10)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================
    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x10,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret


# /// ============================= 13 ========================

.align 4
ENTRY(bad_interrupt_13_20)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x20,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 14 ========================

.align 4
ENTRY(bad_interrupt_14_40)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es

    # // SECOND /// ================================

    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x40,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20

    # END SECOND  ================================

    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret

# /// ============================= 15 ========================
.align 4
ENTRY(bad_interrupt_15)
    # /// Save Most Registers
    cld
    push %es
    push %ds
    pushl %eax
    pushl %edx
    pushl %ecx
    movl $SYMBOL_NAME(__KERNEL_DS),%edx
    mov %dx,%ds
    mov %dx,%es
    # // SECOND /// ================================
    inb $0xA1,%al
    jmp 1f
1:  jmp 1f
1:  orb $0x80,SYMBOL_NAME(cache_A1)
    movb SYMBOL_NAME(cache_A1),%al
    outb %al,$0xA1
    jmp 1f
1:  jmp 1f
1:  movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  outb %al,$0x20
    popl %ecx
    popl %edx
    popl %eax
    pop %ds
    pop %es
    iret


