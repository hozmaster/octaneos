/*
 * Berlin Brown
 *
 * $Id: hardware_interrupts.S,v 1.6 2004/04/10 14:16:25 bigbinc Exp $
 *
 * assembly wrapper functions for interrupts
 *
 * The actual work will get done by other C implementations, these
 * ASM based wrappers are only used to ensure proper stack manipulations.
 *
 * (Also see interrupts.c for C definitions
 * this file is synonymous with irq.c in the linux kernel)
 */

#define __ASSEMBLY__

#include <linux/sys.h>
#include <linux/linkage.h>
#include <asm/segment.h>

EBX			= 0x00
ECX			= 0x04
EDX			= 0x08
ESI			= 0x0C
EDI			= 0x10
EBP			= 0x14
EAX			= 0x18
DS			= 0x1C
ES			= 0x20
FS			= 0x24
GS			= 0x28
ORIG_EAX 	= 0x2C
EIP   		= 0x30
CS   		= 0x34
EFLAGS  	= 0x38
OLDESP  	= 0x3C
OLDSS  		= 0x40

CF_MASK  	= 0x00000001
IF_MASK  	= 0x00000200
NT_MASK  	= 0x00004000
VM_MASK  	= 0x00020000

								
/*
 * these are offsets into the task-struct.
 */
state		=  0
counter		=  4
priority	=  8
signal		= 12
blocked		= 16
flags		= 20
errno		= 24
dbgreg6		= 52
dbgreg7		= 56
exec_domain = 60

ENOSYS = 38

.globl system_call, lcall7
.globl ret_from_sys_call, sys_call_table
		
#*************************************************
# Register SAVE_ALL, reusable code block
#*************************************************
#define SAVE_ALL       \
    cld;               \
    push %gs;          \
    push %fs;          \
    push %es;          \
    push %ds;          \
    pushl %eax;        \
    pushl %ebp;        \
    pushl %edi;        \
    pushl %esi;        \
    pushl %edx;        \
    pushl %ecx;        \
    pushl %ebx;        \
    movl $SYMBOL_NAME(KERNEL_DS),%edx; \
    mov %dx,%ds;       \
    mov %dx,%es;       \
    movl $SYMBOL_NAME(USER_DS),%edx;   \
    mov %dx,%fs;       \
    movl $0,%edx;

#*************************************************
# Register RESTORE_ALL, reusable code block
#*************************************************
#define RESTORE_ALL    \
    popl %ebx;         \
    popl %ecx;         \
    popl %edx;         \
    popl %esi;         \
    popl %edi;         \
    popl %ebp;         \
    popl %eax;         \
    pop %ds;           \
    pop %es;           \
    pop %fs;           \
    pop %gs;           \
    addl $4,%esp;      \
    iret

#***************************************************
# PIC SETUP Routines
#***************************************************
#define PIC_CLOSE_FIRST(mask)   \
    inb $0x21,%al;              \
    jmp 1f;                     \
1:  jmp 1f;                     \
1:  andb $~(mask),cache_21;     \
    movb cache_21,%al;          \
    outb %al,$0x21;

#define PIC_CLOSE_SECOND(mask)  \
    inb $0xA1,%al;              \
    jmp 1f;                     \
1:  jmp 1f;                     \
1:  andb $~(mask),cache_A1;     \
    movb cache_A1,%al;          \
    outb %al,$0xA1;

#define SETUP_PIC01(mask)    \
    inb $0x21,%al;           \
    jmp 1f;                  \
1:  jmp 1f;                  \
1:  orb $mask,cache_21;      \
    movb cache_21,%al;       \
    outb %al,$0x21;          \
    jmp 1f;                  \
1:  jmp 1f;                  \
1:  movb $0x20,%al;          \
    outb %al,$0x20;


#define SETUP_PIC02(mask)   \
    inb $0xA1,%al;          \
    jmp 1f;                 \
1:  jmp 1f;                 \
1:  orb $mask,cache_A1;     \
    movb cache_A1,%al;      \
    outb %al,$0xA1;         \
    jmp 1f;                 \
1:  jmp 1f;                 \
1:  movb $0x20,%al;         \
    outb %al,$0xA0;         \
    jmp 1f;                 \
1:  jmp 1f;                 \
1:  outb %al,$0x20;


/**
 * Interrupt Vector List:
 * 0  0x01   system_timer
 * 1  0x02   keyboard
 * 2  0x04
 * 3  0x08
 * 4  0x10
 * 5  0x20   harddrive
 * 6  0x40   floppy
 * 7  0x80
 * (Second PIC)
 * 8  0x01
 * 9  0x02
 * 10 0x04
 * 11 0x08
 * 12 0x10
 * 13 0x20
 * 14 0x40
 * 15 0x80
 */		 

#***************************************************
# TIMER_IRQ
#***************************************************
ENTRY(hw_interrupt_entry_00)
    pushl $-0-2
    SAVE_ALL
    SETUP_PIC01(0x01)
    sti
    movl %esp,%ebx
    pushl %ebx
    pushl $0	# PUSH the IRQ request INDEX number on the stack
	call SYMBOL_NAME(do_IRQ)
    addl $8,%esp
    cli
    PIC_CLOSE_FIRST(0x01)
	decl SYMBOL_NAME(intr_count)
	#*****************
	# TODO: BUG fix to set the intr_count to zero
	# for the bottom handler.
	#*****************
	xorl %eax, %eax
	movl %eax, SYMBOL_NAME(intr_count)
	jmp ret_from_sys_call

#***************************************************
# KEYBOARD_IRQ
# Interrupt Entry = 01
# @see interrupts.c
# @see scheduler.c
#***************************************************
ENTRY(hw_interrupt_entry_01)
    pushl $-0-2
    SAVE_ALL
    SETUP_PIC01(0x02)
    sti
    movl %esp,%ebx
    pushl %ebx
    pushl $0	# PUSH the IRQ request INDEX number on the stack
	###call SYMBOL_NAME(do_IRQ)
	call SYMBOL_NAME(__debug_keyboard_interrupt)
    addl $8,%esp
    cli
    PIC_CLOSE_FIRST(0x02)
    RESTORE_ALL

#***************************************************
# FLOPPY_IRQ
# See floppy.c
#***************************************************
ENTRY(hw_interrupt_entry_06)
    pushl $-0-2
    SAVE_ALL
    SETUP_PIC01(0x40)
    sti
    movl %esp,%ebx
    pushl %ebx
    pushl $0
	call SYMBOL_NAME(do_IRQ)
    addl $8,%esp
    cli
    PIC_CLOSE_FIRST(0x40)
    RESTORE_ALL

#***********************************************************
# Lcall7
#***********************************************************  
ENTRY(lcall7)
	pushfl                  # We get a different stack layout with call gates,
	pushl %eax              # which has to be cleaned up later.
	SAVE_ALL
	movl EIP(%esp), %eax    # Due to call gates, this is eflags, not eip.
	movl CS(%esp), %edx	    # this is eip
	movl EFLAGS(%esp), %ecx	# and this is cs
	movl %eax,EFLAGS(%esp)	#
	movl %edx,EIP(%esp)     # Now we move them to their "normal" places
	movl %ecx,CS(%esp)
	movl %esp,%eax
	movl SYMBOL_NAME(current),%edx
	pushl %eax
	movl exec_domain(%edx),%edx	# Get the execution domain
	movl 4(%edx),%edx           # Get the lcall7 handler for the domain
	call *%edx
	popl %eax
	jmp ret_from_sys_call
			
#***********************************************************
# Handle Bottom Half (private)
#***********************************************************
handle_bottom_half:
	pushfl
	incl SYMBOL_NAME(intr_count)
	sti
	call SYMBOL_NAME(do_bottom_half)
	popfl
	decl SYMBOL_NAME(intr_count)
	jmp 9f
.align 4
reschedule:
	pushl $ret_from_sys_call
	jmp SYMBOL_NAME(schedule)
		
#***********************************************************
# System Call (public)
#***********************************************************  
ENTRY(system_call)
	pushl %eax                      # save orig_eax
	SAVE_ALL
	movl $-ENOSYS,EAX(%esp)
	cmpl $(NR_syscalls),%eax
	jae ret_from_sys_call
	call *SYMBOL_NAME(sys_call_table)(,%eax,4)
	testl %eax, %eax
	je ret_from_sys_call
	movl SYMBOL_NAME(current),%ebx
	andl $~CF_MASK,EFLAGS(%esp)	    # clear carry - assume no errors
	movl $0,errno(%ebx)
	movl %db6,%edx
	movl %edx,dbgreg6(%ebx)  		# save current hardware debugging status
	testb $0x20,flags(%ebx)  		# PF_TRACESYS
	jne 1f
	call *%eax
	movl %eax, EAX(%esp)  			# save the return value
	movl errno(%ebx),%edx
	negl %edx
	je ret_from_sys_call
	movl %edx,EAX(%esp)
	orl $(CF_MASK),EFLAGS(%esp) 	# set carry to indicate error
	jmp ret_from_sys_call
.align 4
1:  call SYMBOL_NAME(syscall_trace)
	movl ORIG_EAX(%esp),%eax
	call *SYMBOL_NAME(sys_call_table)(,%eax,4)
	movl %eax,EAX(%esp)             # save the return value
	movl SYMBOL_NAME(current),%eax
	movl errno(%eax),%edx
	negl %edx
	je 1f
	movl %edx,EAX(%esp)
	orl $(CF_MASK),EFLAGS(%esp) 	# set carry to indicate error
1:  call SYMBOL_NAME(syscall_trace)

.align 4, 0x90
ENTRY(ret_from_sys_call)
	cmpl $0, SYMBOL_NAME(intr_count)
	jne 2f
	movl SYMBOL_NAME(bh_mask),%eax
	andl SYMBOL_NAME(bh_active),%eax
	jne handle_bottom_half
9:  movl EFLAGS(%esp),%eax  	# check VM86 flag: CS/SS are
	testl $(VM_MASK),%eax  	    # different then
	jne 1f
	cmpw $(KERNEL_CS),CS(%esp)  # was old code segment supervisor ?
	je  2f
1:  sti
	orl $(IF_MASK),%eax  		# these just try to make sure
	andl $~NT_MASK,%eax  		# the program doesn't do anything
	movl %eax,EFLAGS(%esp)  	# stupid		
	####cmpl $0,_need_resched
	jne reschedule
	movl SYMBOL_NAME(current),%eax
	####cmpl _task, %eax        # task[0] cannot have signals
	je 2f
	cmpl $0,state(%eax)		    # state
	jne reschedule
	cmpl $0,counter(%eax)  	    # counter
	je reschedule
	movl blocked(%eax),%ecx
	movl %ecx,%ebx   			# save blocked in %ebx for signal handling
	notl %ecx
	andl signal(%eax),%ecx
	jne signal_return
2:  RESTORE_ALL
.align 4
signal_return:
	movl %esp,%ecx
	pushl %ecx
	testl $(VM_MASK),EFLAGS(%ecx)
	jne v86_signal_return
	pushl %ebx
	####call SYMBOL_NAME(do_signal)
	popl %ebx
	popl %ebx
	RESTORE_ALL
		
#***********************************************************
# V86 signal return
#***********************************************************
.align 4
v86_signal_return:
	####call SYMBOL_NAME(save_v86_state)
	movl %eax,%esp
	pushl %eax
	pushl %ebx
	####call SYMBOL_NAME(do_signal)
	popl %ebx
	popl %ebx
	RESTORE_ALL
  
#***********************************************************
# END
#***********************************************************

#***********************************************************
# System Call Table
#***********************************************************
.data
ENTRY(sys_call_table)
    .long SYMBOL_NAME(sys_setup)		/* 0  -  old "setup()" system call*/
    .long SYMBOL_NAME(sys_exit)
    .long SYMBOL_NAME(sys_fork)
    .long SYMBOL_NAME(sys_read)
    .long SYMBOL_NAME(sys_write)
    .long SYMBOL_NAME(sys_open)   		/* 5 */
    .long SYMBOL_NAME(sys_close)
    .long SYMBOL_NAME(sys_waitpid)
    .long SYMBOL_NAME(sys_creat)
    .long SYMBOL_NAME(sys_link)
    .long SYMBOL_NAME(sys_unlink)       /* 10 */
    .long SYMBOL_NAME(sys_execve)
    .long SYMBOL_NAME(sys_chdir)
    .long SYMBOL_NAME(sys_time)
    .long SYMBOL_NAME(sys_mknod)
    .long SYMBOL_NAME(sys_chmod)  		/* 15 */
    .long SYMBOL_NAME(sys_lchown16)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old break syscall holder */
    .long SYMBOL_NAME(sys_stat)
    .long SYMBOL_NAME(sys_lseek)
    .long SYMBOL_NAME(sys_getpid)		/* 20 */
    .long SYMBOL_NAME(sys_mount)
    .long SYMBOL_NAME(sys_oldumount)
    .long SYMBOL_NAME(sys_setuid16)
    .long SYMBOL_NAME(sys_getuid16)
    .long SYMBOL_NAME(sys_stime)  		/* 25 */
    .long SYMBOL_NAME(sys_ptrace)
    .long SYMBOL_NAME(sys_alarm)
    .long SYMBOL_NAME(sys_fstat)
    .long SYMBOL_NAME(sys_pause)
    .long SYMBOL_NAME(sys_utime)  		/* 30 */
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old stty syscall holder */
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old gtty syscall holder */
    .long SYMBOL_NAME(sys_access)
    .long SYMBOL_NAME(sys_nice)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* 35 */  /* old ftime syscall holder */
    .long SYMBOL_NAME(sys_sync)
    .long SYMBOL_NAME(sys_kill)
    .long SYMBOL_NAME(sys_rename)
    .long SYMBOL_NAME(sys_mkdir)
    .long SYMBOL_NAME(sys_rmdir)  		/* 40 */
    .long SYMBOL_NAME(sys_dup)
    .long SYMBOL_NAME(sys_pipe)
    .long SYMBOL_NAME(sys_times)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old prof syscall holder */
    .long SYMBOL_NAME(sys_brk)   		/* 45 */
    .long SYMBOL_NAME(sys_setgid16)
    .long SYMBOL_NAME(sys_getgid16)
    .long SYMBOL_NAME(sys_signal)
    .long SYMBOL_NAME(sys_geteuid16)
    .long SYMBOL_NAME(sys_getegid16) 	/* 50 */
    .long SYMBOL_NAME(sys_acct)
    .long SYMBOL_NAME(sys_umount)		/* recycled never used phys() */
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old lock syscall holder */
    .long SYMBOL_NAME(sys_ioctl)
    .long SYMBOL_NAME(sys_fcntl)  		/* 55 */
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old mpx syscall holder */
    .long SYMBOL_NAME(sys_setpgid)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old ulimit syscall holder */
    .long SYMBOL_NAME(sys_olduname)
    .long SYMBOL_NAME(sys_umask)  		/* 60 */
    .long SYMBOL_NAME(sys_chroot)
    .long SYMBOL_NAME(sys_ustat)
    .long SYMBOL_NAME(sys_dup2)
    .long SYMBOL_NAME(sys_getppid)
    .long SYMBOL_NAME(sys_getpgrp)  	/* 65 */
    .long SYMBOL_NAME(sys_setsid)
    .long SYMBOL_NAME(sys_sigaction)
    .long SYMBOL_NAME(sys_sgetmask)
    .long SYMBOL_NAME(sys_ssetmask)
    .long SYMBOL_NAME(sys_setreuid16)	/* 70 */
    .long SYMBOL_NAME(sys_setregid16)
    .long SYMBOL_NAME(sys_sigsuspend)
    .long SYMBOL_NAME(sys_sigpending)
    .long SYMBOL_NAME(sys_sethostname)
    .long SYMBOL_NAME(sys_setrlimit)	/* 75 */
    .long SYMBOL_NAME(sys_old_getrlimit)
    .long SYMBOL_NAME(sys_getrusage)
    .long SYMBOL_NAME(sys_gettimeofday)
    .long SYMBOL_NAME(sys_settimeofday)
    .long SYMBOL_NAME(sys_getgroups16)	/* 80 */
    .long SYMBOL_NAME(sys_setgroups16)
    .long SYMBOL_NAME(old_select)
    .long SYMBOL_NAME(sys_symlink)
    .long SYMBOL_NAME(sys_lstat)
    .long SYMBOL_NAME(sys_readlink)  	/* 85 */
    .long SYMBOL_NAME(sys_uselib)
    .long SYMBOL_NAME(sys_swapon)
    .long SYMBOL_NAME(sys_reboot)
    .long SYMBOL_NAME(old_readdir)
    .long SYMBOL_NAME(old_mmap)  		/* 90 */
    .long SYMBOL_NAME(sys_munmap)
    .long SYMBOL_NAME(sys_truncate)
    .long SYMBOL_NAME(sys_ftruncate)
    .long SYMBOL_NAME(sys_fchmod)
    .long SYMBOL_NAME(sys_fchown16)  	/* 95 */
    .long SYMBOL_NAME(sys_getpriority)
    .long SYMBOL_NAME(sys_setpriority)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old profil syscall holder */
    .long SYMBOL_NAME(sys_statfs)
    .long SYMBOL_NAME(sys_fstatfs)  	/* 100 */
    .long SYMBOL_NAME(sys_ioperm)
    .long SYMBOL_NAME(sys_socketcall)
    .long SYMBOL_NAME(sys_syslog)
    .long SYMBOL_NAME(sys_setitimer)
    .long SYMBOL_NAME(sys_getitimer) 	/* 105 */
    .long SYMBOL_NAME(sys_newstat)
    .long SYMBOL_NAME(sys_newlstat)
    .long SYMBOL_NAME(sys_newfstat)
    .long SYMBOL_NAME(sys_uname)
    .long SYMBOL_NAME(sys_iopl)      	/* 110 */
    .long SYMBOL_NAME(sys_vhangup)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* old "idle" system call */
    .long SYMBOL_NAME(sys_vm86old)
    .long SYMBOL_NAME(sys_wait4)
    .long SYMBOL_NAME(sys_swapoff)  	/* 115 */
    .long SYMBOL_NAME(sys_sysinfo)
    .long SYMBOL_NAME(sys_ipc)
    .long SYMBOL_NAME(sys_fsync)
    .long SYMBOL_NAME(sys_sigreturn)
    .long SYMBOL_NAME(sys_clone)  		/* 120 */
    .long SYMBOL_NAME(sys_setdomainname)
    .long SYMBOL_NAME(sys_newuname)
    .long SYMBOL_NAME(sys_modify_ldt)
    .long SYMBOL_NAME(sys_adjtimex)
    .long SYMBOL_NAME(sys_mprotect)  	/* 125 */
    .long SYMBOL_NAME(sys_sigprocmask)
    .long SYMBOL_NAME(sys_create_module)
    .long SYMBOL_NAME(sys_init_module)
    .long SYMBOL_NAME(sys_delete_module)
    .long SYMBOL_NAME(sys_get_kernel_syms) /* 130 */
    .long SYMBOL_NAME(sys_quotactl)
    .long SYMBOL_NAME(sys_getpgid)
    .long SYMBOL_NAME(sys_fchdir)
    .long SYMBOL_NAME(sys_bdflush)
    .long SYMBOL_NAME(sys_sysfs)  		/* 135 */
    .long SYMBOL_NAME(sys_personality)
    .long SYMBOL_NAME(sys_ni_syscall) 	/* for afs_syscall */
    .long SYMBOL_NAME(sys_setfsuid16)
    .long SYMBOL_NAME(sys_setfsgid16)
    .long SYMBOL_NAME(sys_llseek)		/* 140 */
    .long SYMBOL_NAME(sys_getdents)
    .long SYMBOL_NAME(sys_select)
    .long SYMBOL_NAME(sys_flock)
    .long SYMBOL_NAME(sys_msync)
    .long SYMBOL_NAME(sys_readv)		/* 145 */
    .long SYMBOL_NAME(sys_writev)
    .long SYMBOL_NAME(sys_getsid)
    .long SYMBOL_NAME(sys_fdatasync)
    .long SYMBOL_NAME(sys_sysctl)
    .long SYMBOL_NAME(sys_mlock)		/* 150 */
    .long SYMBOL_NAME(sys_munlock)
    .long SYMBOL_NAME(sys_mlockall)
    .long SYMBOL_NAME(sys_munlockall)
    .long SYMBOL_NAME(sys_sched_setparam)
    .long SYMBOL_NAME(sys_sched_getparam)   /* 155 */
    .long SYMBOL_NAME(sys_sched_setscheduler)
    .long SYMBOL_NAME(sys_sched_getscheduler)
    .long SYMBOL_NAME(sys_sched_yield)
    .long SYMBOL_NAME(sys_sched_get_priority_max)
    .long SYMBOL_NAME(sys_sched_get_priority_min)  /* 160 */
    .long SYMBOL_NAME(sys_sched_rr_get_interval)
    .long SYMBOL_NAME(sys_nanosleep)
    .long SYMBOL_NAME(sys_mremap)
    .long SYMBOL_NAME(sys_setresuid16)
    .long SYMBOL_NAME(sys_getresuid16)		/* 165 */
    .long SYMBOL_NAME(sys_vm86)
    .long SYMBOL_NAME(sys_query_module)
    .long SYMBOL_NAME(sys_poll)
    .long SYMBOL_NAME(sys_nfsservctl)
    .long SYMBOL_NAME(sys_setresgid16)		/* 170 */
    .long SYMBOL_NAME(sys_getresgid16)
    .long SYMBOL_NAME(sys_prctl)
    .long SYMBOL_NAME(sys_rt_sigreturn)
    .long SYMBOL_NAME(sys_rt_sigaction)
    .long SYMBOL_NAME(sys_rt_sigprocmask)	/* 175 */
    .long SYMBOL_NAME(sys_rt_sigpending)
    .long SYMBOL_NAME(sys_rt_sigtimedwait)
    .long SYMBOL_NAME(sys_rt_sigqueueinfo)
    .long SYMBOL_NAME(sys_rt_sigsuspend)
    .long SYMBOL_NAME(sys_pread)			/* 180 */
    .long SYMBOL_NAME(sys_pwrite)
    .long SYMBOL_NAME(sys_chown16)
    .long SYMBOL_NAME(sys_getcwd)
    .long SYMBOL_NAME(sys_capget)
    .long SYMBOL_NAME(sys_capset)        	/* 185 */
    .long SYMBOL_NAME(sys_sigaltstack)
    .long SYMBOL_NAME(sys_sendfile)
    .long SYMBOL_NAME(sys_ni_syscall)		/* streams1 */
    .long SYMBOL_NAME(sys_ni_syscall) 		/* streams2 */
    .long SYMBOL_NAME(sys_vfork)			/* 190 */
    .long SYMBOL_NAME(sys_getrlimit)
    .long SYMBOL_NAME(sys_mmap2)
    .long SYMBOL_NAME(sys_truncate64)
    .long SYMBOL_NAME(sys_ftruncate64)
    .long SYMBOL_NAME(sys_stat64)			/* 195 */
    .long SYMBOL_NAME(sys_lstat64)
    .long SYMBOL_NAME(sys_fstat64)
    .long SYMBOL_NAME(sys_lchown)
    .long SYMBOL_NAME(sys_getuid)
    .long SYMBOL_NAME(sys_getgid)			/* 200 */
    .long SYMBOL_NAME(sys_geteuid)
    .long SYMBOL_NAME(sys_getegid)
    .long SYMBOL_NAME(sys_setreuid)
    .long SYMBOL_NAME(sys_setregid)
    .long SYMBOL_NAME(sys_getgroups)		/* 205 */
    .long SYMBOL_NAME(sys_setgroups)
    .long SYMBOL_NAME(sys_fchown)
    .long SYMBOL_NAME(sys_setresuid)
    .long SYMBOL_NAME(sys_getresuid)
    .long SYMBOL_NAME(sys_setresgid)		/* 210 */
    .long SYMBOL_NAME(sys_getresgid)
    .long SYMBOL_NAME(sys_chown)
    .long SYMBOL_NAME(sys_setuid)
    .long SYMBOL_NAME(sys_setgid)
    .long SYMBOL_NAME(sys_setfsuid)			/* 215 */
    .long SYMBOL_NAME(sys_setfsgid)
    .long SYMBOL_NAME(sys_pivot_root)
    .long SYMBOL_NAME(sys_mincore)
    .long SYMBOL_NAME(sys_madvise)
    .long SYMBOL_NAME(sys_getdents64)		/* 220 */
    .long SYMBOL_NAME(sys_fcntl64)
    .long SYMBOL_NAME(sys_ni_syscall)		/* reserved for TUX */
    .long SYMBOL_NAME(sys_ni_syscall)		/* Reserved for Security */
    .long SYMBOL_NAME(sys_gettid)
    .long SYMBOL_NAME(sys_readahead)		/* 225 */
    .long SYMBOL_NAME(sys_setxattr)
    .long SYMBOL_NAME(sys_lsetxattr)
    .long SYMBOL_NAME(sys_fsetxattr)
    .long SYMBOL_NAME(sys_getxattr)
    .long SYMBOL_NAME(sys_lgetxattr)		/* 230 */
    .long SYMBOL_NAME(sys_fgetxattr)
    .long SYMBOL_NAME(sys_listxattr)
    .long SYMBOL_NAME(sys_llistxattr)
    .long SYMBOL_NAME(sys_flistxattr)
    .long SYMBOL_NAME(sys_removexattr)		/* 235 */
    .long SYMBOL_NAME(sys_lremovexattr)
    .long SYMBOL_NAME(sys_fremovexattr)
    .long SYMBOL_NAME(sys_tkill)
    .long SYMBOL_NAME(sys_sendfile64)
    .long SYMBOL_NAME(sys_ni_syscall) /* 240 reserved for futex */
    .long SYMBOL_NAME(sys_ni_syscall) /* reserved for sched_setaffinity */
    .long SYMBOL_NAME(sys_ni_syscall) /* reserved for sched_getaffinity */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_set_thread_area */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_get_thread_area */
    .long SYMBOL_NAME(sys_ni_syscall) /* 245 sys_io_setup */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_io_destroy */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_io_getevents */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_io_submit */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_io_cancel */
    .long SYMBOL_NAME(sys_ni_syscall) /* 250 sys_alloc_hugepages */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_free_hugepages */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_exit_group */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_lookup_dcookie */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_epoll_create */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_epoll_ctl 255 */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_epoll_wait */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_remap_file_pages */
    .long SYMBOL_NAME(sys_ni_syscall) /* sys_set_tid_address */
	.rept NR_syscalls-(.-sys_call_table)/4
    .long SYMBOL_NAME(sys_ni_syscall)
	.endr
