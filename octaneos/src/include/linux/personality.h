/*
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
 */
#ifndef _PERSONALITY_H
#define _PERSONALITY_H

#include <system/system.h>
#include <linux/ptrace.h>

/* Flags for bug emulation. These occupy the top three bytes. */
#define STICKY_TIMEOUTS		0x4000000
#define WHOLE_SECONDS		0x2000000

/* Personality types. These go in the low byte. Avoid using the top bit,
 * it will conflict with error returns.
 */
#define PER_MASK		(0x00ff)
#define PER_LINUX		(0x0000)
#define PER_SVR4		(0x0001 | STICKY_TIMEOUTS)
#define PER_SVR3		(0x0002 | STICKY_TIMEOUTS)
#define PER_SCOSVR3		(0x0003 | STICKY_TIMEOUTS | WHOLE_SECONDS)
#define PER_WYSEV386	(0x0004 | STICKY_TIMEOUTS)
#define PER_ISCR4		(0x0005 | STICKY_TIMEOUTS)
#define PER_BSD			(0x0006)

/* Prototype for an lcall7 syscall handler. */
typedef asmlinkage void (*lcall7_func)(struct pt_regs *);

/* Description of an execution domain - personality range supported,
 * lcall7 syscall handler, start up / shut down functions etc.
 * N.B. The name and lcall7 handler must be where they are since the
 * offset of the handler is hard coded in kernel/sys_call.S.
 */
struct exec_domain {
	char *name;
	lcall7_func handler;
	unsigned char pers_low, pers_high;
	unsigned long *signal_map;
	unsigned long *signal_invmap;
	int *use_count;
	struct exec_domain *next;
};

extern struct exec_domain default_exec_domain;
extern struct exec_domain *lookup_exec_domain(unsigned long personality);
extern int register_exec_domain(struct exec_domain *it);
extern int unregister_exec_domain(struct exec_domain *it);
extern asmlinkage int sys_personality(unsigned long personality);

#endif
