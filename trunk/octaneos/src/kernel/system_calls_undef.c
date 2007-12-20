//
// Copyright (C) 2003, 2007 Berlin Brown
// $Id: system_calls.c,v 1.5 2005/05/26 00:06:53 bigbinc Exp $
//

#include <system/system.h>
#include <linux/errno.h>
#include <asm/io.h>

asmlinkage int syscall_trace(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_exit(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_read(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_write(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_open(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_close(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_waitpid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_creat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_link(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_unlink(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_execve(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_chdir(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mknod(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_chmod(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lchown16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_stat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lseek(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_oldumount(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ptrace(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fstat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_utime(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_access(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_kill(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rename(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mkdir(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rmdir(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_dup(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_pipe(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setgid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getgid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_signal(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_geteuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getegid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ioctl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fcntl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_chroot(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ustat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_dup2(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigaction(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sgetmask(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ssetmask(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setreuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setregid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigsuspend(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigpending(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_old_getrlimit(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getgroups16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setgroups16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int old_select(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_symlink(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lstat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_readlink(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_uselib(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_swapon(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int old_readdir(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int old_mmap(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_munmap(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_truncate(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ftruncate(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fchmod(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fchown16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_statfs(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fstatfs(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ioperm(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_socketcall(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_syslog(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setitimer(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getitimer(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_newstat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_newlstat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_newfstat(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_iopl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_vhangup(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_vm86old(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_wait4(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_swapoff(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sysinfo(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ipc(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigreturn(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_clone(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_modify_ldt(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mprotect(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigprocmask(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_create_module(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_init_module(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_delete_module(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_get_kernel_syms(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_quotactl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fchdir(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setfsuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setfsgid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_llseek(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getdents(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_select(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_flock(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_msync(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_readv(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_writev(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getsid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fdatasync(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sysctl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mlock(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_munlock(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mlockall(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_munlockall(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_setparam(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_getparam(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_setscheduler(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_getscheduler(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_yield(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_get_priority_max(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_get_priority_min(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sched_rr_get_interval(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_nanosleep(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mremap(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setresuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getresuid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_vm86(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_query_module(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_poll(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_nfsservctl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setresgid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getresgid16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_prctl(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigreturn(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigaction(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigprocmask(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigpending(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigtimedwait(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigqueueinfo(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_rt_sigsuspend(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_pread(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_pwrite(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_chown16(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getcwd(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_capget(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_capset(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sigaltstack(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sendfile(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_vfork(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mmap2(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_truncate64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_ftruncate64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_stat64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lstat64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fstat64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lchown(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fchown(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setresuid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getresuid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setresgid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getresgid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_chown(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setfsuid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setfsgid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_pivot_root(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_mincore(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_madvise(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getdents64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fcntl64(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_gettid(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_readahead(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_setxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lsetxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fsetxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_getxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lgetxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fgetxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_listxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_llistxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_flistxattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_removexattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_lremovexattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_fremovexattr(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_tkill(void) {
    // TODO
    return -ENOSYS;
}

asmlinkage int sys_sendfile64(void) {
    // TODO
    return -ENOSYS;
}
