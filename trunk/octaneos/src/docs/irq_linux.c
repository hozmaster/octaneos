# 1 "i8259.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "i8259.c"
# 1 "/home/bigbinc/kern_src/linux/include/linux/config.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/linux/autoconf.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/linux/config.h" 2
# 2 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/ptrace.h" 1
# 24 "/home/bigbinc/kern_src/linux/include/linux/ptrace.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/ptrace.h" 1
# 26 "/home/bigbinc/kern_src/linux/include/asm/ptrace.h"
struct pt_regs {
        long ebx;
        long ecx;
        long edx;
        long esi;
        long edi;
        long ebp;
        long eax;
        int xds;
        int xes;
        long orig_eax;
        long eip;
        int xcs;
        long eflags;
        long esp;
        int xss;
};
# 25 "/home/bigbinc/kern_src/linux/include/linux/ptrace.h" 2
# 3 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/errno.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/errno.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/linux/errno.h" 2
# 4 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/signal.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/signal.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/linux/types.h" 1







# 1 "/home/bigbinc/kern_src/linux/include/linux/posix_types.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/linux/stddef.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/linux/posix_types.h" 2
# 36 "/home/bigbinc/kern_src/linux/include/linux/posix_types.h"
typedef struct {
        unsigned long fds_bits [(1024/(8 * sizeof(unsigned long)))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int);


typedef int __kernel_key_t;

# 1 "/home/bigbinc/kern_src/linux/include/asm/posix_types.h" 1
# 10 "/home/bigbinc/kern_src/linux/include/asm/posix_types.h"
typedef unsigned short __kernel_dev_t;
typedef unsigned long __kernel_ino_t;
typedef unsigned short __kernel_mode_t;
typedef unsigned short __kernel_nlink_t;
typedef long __kernel_off_t;
typedef int __kernel_pid_t;
typedef unsigned short __kernel_ipc_pid_t;
typedef unsigned short __kernel_uid_t;
typedef unsigned short __kernel_gid_t;
typedef unsigned int __kernel_size_t;
typedef int __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;
typedef long __kernel_time_t;
typedef long __kernel_suseconds_t;
typedef long __kernel_clock_t;
typedef int __kernel_daddr_t;
typedef char * __kernel_caddr_t;
typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
typedef unsigned int __kernel_uid32_t;
typedef unsigned int __kernel_gid32_t;

typedef unsigned short __kernel_old_uid_t;
typedef unsigned short __kernel_old_gid_t;


typedef long long __kernel_loff_t;


typedef struct {



        int __val[2];

} __kernel_fsid_t;
# 47 "/home/bigbinc/kern_src/linux/include/linux/posix_types.h" 2
# 9 "/home/bigbinc/kern_src/linux/include/linux/types.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/types.h" 1



typedef unsigned short umode_t;






typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;


typedef __signed__ long long __s64;
typedef unsigned long long __u64;
# 10 "/home/bigbinc/kern_src/linux/include/linux/types.h" 2



typedef __kernel_fd_set fd_set;
typedef __kernel_dev_t dev_t;
typedef __kernel_ino_t ino_t;
typedef __kernel_mode_t mode_t;
typedef __kernel_nlink_t nlink_t;
typedef __kernel_off_t off_t;
typedef __kernel_pid_t pid_t;
typedef __kernel_daddr_t daddr_t;
typedef __kernel_key_t key_t;
typedef __kernel_suseconds_t suseconds_t;
# 40 "/home/bigbinc/kern_src/linux/include/linux/types.h"
typedef __kernel_uid_t uid_t;
typedef __kernel_gid_t gid_t;



typedef __kernel_loff_t loff_t;
# 54 "/home/bigbinc/kern_src/linux/include/linux/types.h"
typedef __kernel_size_t size_t;




typedef __kernel_ssize_t ssize_t;




typedef __kernel_ptrdiff_t ptrdiff_t;




typedef __kernel_time_t time_t;




typedef __kernel_clock_t clock_t;




typedef __kernel_caddr_t caddr_t;



typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;


typedef unsigned char unchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;




typedef __u8 u_int8_t;
typedef __s8 int8_t;
typedef __u16 u_int16_t;
typedef __s16 int16_t;
typedef __u32 u_int32_t;
typedef __s32 int32_t;



typedef __u8 uint8_t;
typedef __u16 uint16_t;
typedef __u32 uint32_t;


typedef __u64 uint64_t;
typedef __u64 u_int64_t;
typedef __s64 int64_t;
# 123 "/home/bigbinc/kern_src/linux/include/linux/types.h"
struct ustat {
        __kernel_daddr_t f_tfree;
        __kernel_ino_t f_tinode;
        char f_fname[6];
        char f_fpack[6];
};
# 5 "/home/bigbinc/kern_src/linux/include/asm/signal.h" 2


struct siginfo;
# 27 "/home/bigbinc/kern_src/linux/include/asm/signal.h"
typedef unsigned long sigset_t;
# 129 "/home/bigbinc/kern_src/linux/include/asm/signal.h"
typedef void (*__sighandler_t)(int);
# 156 "/home/bigbinc/kern_src/linux/include/asm/signal.h"
struct sigaction {
        union {
          __sighandler_t _sa_handler;
          void (*_sa_sigaction)(int, struct siginfo *, void *);
        } _u;
        sigset_t sa_mask;
        unsigned long sa_flags;
        void (*sa_restorer)(void);
};






typedef struct sigaltstack {
        void *ss_sp;
        int ss_flags;
        size_t ss_size;
} stack_t;
# 5 "/home/bigbinc/kern_src/linux/include/linux/signal.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/siginfo.h" 1







typedef union sigval {
        int sival_int;
        void *sival_ptr;
} sigval_t;




typedef struct siginfo {
        int si_signo;
        int si_errno;
        int si_code;

        union {
                int _pad[((128/sizeof(int)) - 3)];


                struct {
                        pid_t _pid;
                        uid_t _uid;
                } _kill;


                struct {
                        unsigned int _timer1;
                        unsigned int _timer2;
                } _timer;


                struct {
                        pid_t _pid;
                        uid_t _uid;
                        sigval_t _sigval;
                } _rt;


                struct {
                        pid_t _pid;
                        uid_t _uid;
                        int _status;
                        clock_t _utime;
                        clock_t _stime;
                } _sigchld;


                struct {
                        void *_addr;
                } _sigfault;


                struct {
                        int _band;
                        int _fd;
                } _sigpoll;
        } _sifields;
} siginfo_t;
# 199 "/home/bigbinc/kern_src/linux/include/asm/siginfo.h"
typedef struct sigevent {
        sigval_t sigev_value;
        int sigev_signo;
        int sigev_notify;
        union {
                int _pad[((64/sizeof(int)) - 3)];

                struct {
                        void (*_function)(sigval_t);
                        void *_attribute;
                } _sigev_thread;
        } _sigev_un;
} sigevent_t;
# 6 "/home/bigbinc/kern_src/linux/include/linux/signal.h" 2
# 5 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/param.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

extern unsigned long event;


# 1 "/home/bigbinc/kern_src/linux/include/linux/binfmts.h" 1




# 1 "/home/bigbinc/kern_src/linux/include/linux/capability.h" 1
# 17 "/home/bigbinc/kern_src/linux/include/linux/capability.h"
# 1 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 1
# 10 "/home/bigbinc/kern_src/linux/include/linux/fs.h"
# 1 "/home/bigbinc/kern_src/linux/include/linux/linkage.h" 1
# 11 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/limits.h" 1
# 12 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/wait.h" 1
# 13 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/vfs.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/statfs.h" 1







typedef __kernel_fsid_t fsid_t;



struct statfs {
        long f_type;
        long f_bsize;
        long f_blocks;
        long f_bfree;
        long f_bavail;
        long f_files;
        long f_ffree;
        __kernel_fsid_t f_fsid;
        long f_namelen;
        long f_spare[6];
};
# 5 "/home/bigbinc/kern_src/linux/include/linux/vfs.h" 2
# 15 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/net.h" 1
# 22 "/home/bigbinc/kern_src/linux/include/linux/net.h"
# 1 "/home/bigbinc/kern_src/linux/include/linux/socket.h" 1





# 1 "/home/bigbinc/kern_src/linux/include/asm/socket.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/sockios.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/asm/socket.h" 2
# 7 "/home/bigbinc/kern_src/linux/include/linux/socket.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/sockios.h" 1
# 8 "/home/bigbinc/kern_src/linux/include/linux/socket.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/uio.h" 1
# 19 "/home/bigbinc/kern_src/linux/include/linux/uio.h"
struct iovec
{
        void *iov_base;
        __kernel_size_t iov_len;
};
# 9 "/home/bigbinc/kern_src/linux/include/linux/socket.h" 2


typedef unsigned short sa_family_t;





struct sockaddr {
        sa_family_t sa_family;
        char sa_data[14];
};

struct linger {
        int l_onoff;
        int l_linger;
};







struct msghdr {
        void * msg_name;
        int msg_namelen;
        struct iovec * msg_iov;
        __kernel_size_t msg_iovlen;
        void * msg_control;
        __kernel_size_t msg_controllen;
        unsigned msg_flags;
};







struct cmsghdr {
        __kernel_size_t cmsg_len;
        int cmsg_level;
        int cmsg_type;
};
# 102 "/home/bigbinc/kern_src/linux/include/linux/socket.h"
static __inline__ struct cmsghdr * __cmsg_nxthdr(void *__ctl, __kernel_size_t __size,
                                               struct cmsghdr *__cmsg)
{
        struct cmsghdr * __ptr;

        __ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) + ( ((__cmsg->cmsg_len)+sizeof(long)-1) & ~(sizeof(long)-1) ));
        if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
                return (struct cmsghdr *)0;

        return __ptr;
}

static __inline__ struct cmsghdr * cmsg_nxthdr (struct msghdr *__msg, struct cmsghdr *__cmsg)
{
        return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}







struct ucred {
        __u32 pid;
        __u32 uid;
        __u32 gid;
};
# 23 "/home/bigbinc/kern_src/linux/include/linux/net.h" 2


struct poll_table_struct;
# 49 "/home/bigbinc/kern_src/linux/include/linux/net.h"
typedef enum {
  SS_FREE = 0,
  SS_UNCONNECTED,
  SS_CONNECTING,
  SS_CONNECTED,
  SS_DISCONNECTING
} socket_state;
# 16 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/kdev_t.h" 1
# 17 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/ioctl.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/ioctl.h" 1
# 5 "/home/bigbinc/kern_src/linux/include/linux/ioctl.h" 2
# 18 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/list.h" 1
# 19 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/dcache.h" 1
# 20 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/stat.h" 1
# 21 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/cache.h" 1




# 1 "/home/bigbinc/kern_src/linux/include/asm/cache.h" 1
# 6 "/home/bigbinc/kern_src/linux/include/linux/cache.h" 2
# 22 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/string.h" 1
# 24 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/asm/atomic.h" 1
# 22 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
typedef struct { volatile int counter; } atomic_t;
# 53 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ void atomic_add(int i, atomic_t *v)
{
        __asm__ __volatile__(
                "" "addl %1,%0"
                :"=m" (v->counter)
                :"ir" (i), "m" (v->counter));
}
# 69 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ void atomic_sub(int i, atomic_t *v)
{
        __asm__ __volatile__(
                "" "subl %1,%0"
                :"=m" (v->counter)
                :"ir" (i), "m" (v->counter));
}
# 87 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ int atomic_sub_and_test(int i, atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__(
                "" "subl %2,%0; sete %1"
                :"=m" (v->counter), "=qm" (c)
                :"ir" (i), "m" (v->counter) : "memory");
        return c;
}
# 105 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ void atomic_inc(atomic_t *v)
{
        __asm__ __volatile__(
                "" "incl %0"
                :"=m" (v->counter)
                :"m" (v->counter));
}
# 120 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ void atomic_dec(atomic_t *v)
{
        __asm__ __volatile__(
                "" "decl %0"
                :"=m" (v->counter)
                :"m" (v->counter));
}
# 137 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ int atomic_dec_and_test(atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__(
                "" "decl %0; sete %1"
                :"=m" (v->counter), "=qm" (c)
                :"m" (v->counter) : "memory");
        return c != 0;
}
# 157 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ int atomic_inc_and_test(atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__(
                "" "incl %0; sete %1"
                :"=m" (v->counter), "=qm" (c)
                :"m" (v->counter) : "memory");
        return c != 0;
}
# 178 "/home/bigbinc/kern_src/linux/include/asm/atomic.h"
static __inline__ int atomic_add_negative(int i, atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__(
                "" "addl %2,%0; sets %1"
                :"=m" (v->counter), "=qm" (c)
                :"ir" (i), "m" (v->counter) : "memory");
        return c;
}
# 26 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/bitops.h" 1
# 36 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ void set_bit(int nr, volatile void * addr)
{
        __asm__ __volatile__( ""
                "btsl %1,%0"
                :"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
}
# 53 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ void __set_bit(int nr, volatile void * addr)
{
        __asm__(
                "btsl %1,%0"
                :"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
}
# 71 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ void clear_bit(int nr, volatile void * addr)
{
        __asm__ __volatile__( ""
                "btrl %1,%0"
                :"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
}
# 90 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ void __change_bit(int nr, volatile void * addr)
{
        __asm__ __volatile__(
                "btcl %1,%0"
                :"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
}
# 107 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ void change_bit(int nr, volatile void * addr)
{
        __asm__ __volatile__( ""
                "btcl %1,%0"
                :"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
}
# 123 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int test_and_set_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__ __volatile__( ""
                "btsl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr) : "memory");
        return oldbit;
}
# 143 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int __test_and_set_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__(
                "btsl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
        return oldbit;
}
# 162 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int test_and_clear_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__ __volatile__( ""
                "btrl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr) : "memory");
        return oldbit;
}
# 182 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int __test_and_clear_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__(
                "btrl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr));
        return oldbit;
}


static __inline__ int __test_and_change_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__ __volatile__(
                "btcl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr) : "memory");
        return oldbit;
}
# 213 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int test_and_change_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__ __volatile__( ""
                "btcl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit),"=m" ((*(volatile long *) addr))
                :"Ir" (nr) : "memory");
        return oldbit;
}
# 233 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int constant_test_bit(int nr, const volatile void * addr)
{
        return ((1UL << (nr & 31)) & (((const volatile unsigned int *) addr)[nr >> 5])) != 0;
}

static __inline__ int variable_test_bit(int nr, volatile void * addr)
{
        int oldbit;

        __asm__ __volatile__(
                "btl %2,%1\n\tsbbl %0,%0"
                :"=r" (oldbit)
                :"m" ((*(volatile long *) addr)),"Ir" (nr));
        return oldbit;
}
# 262 "/home/bigbinc/kern_src/linux/include/asm/bitops.h"
static __inline__ int find_first_zero_bit(void * addr, unsigned size)
{
        int d0, d1, d2;
        int res;

        if (!size)
                return 0;

        __asm__ __volatile__(
                "movl $-1,%%eax\n\t"
                "xorl %%edx,%%edx\n\t"
                "repe; scasl\n\t"
                "je 1f\n\t"
                "xorl -4(%%edi),%%eax\n\t"
                "subl $4,%%edi\n\t"
                "bsfl %%eax,%%edx\n"
                "1:\tsubl %%ebx,%%edi\n\t"
                "shll $3,%%edi\n\t"
                "addl %%edi,%%edx"
                :"=d" (res), "=&c" (d0), "=&D" (d1), "=&a" (d2)
                :"1" ((size + 31) >> 5), "2" (addr), "b" (addr));
        return res;
}







static __inline__ int find_next_zero_bit (void * addr, int size, int offset)
{
        unsigned long * p = ((unsigned long *) addr) + (offset >> 5);
        int set = 0, bit = offset & 31, res;

        if (bit) {



                __asm__("bsfl %1,%0\n\t"
                        "jne 1f\n\t"
                        "movl $32, %0\n"
                        "1:"
                        : "=r" (set)
                        : "r" (~(*p >> bit)));
                if (set < (32 - bit))
                        return set + offset;
                set = 32 - bit;
                p++;
        }



        res = find_first_zero_bit (p, size - 32 * (p - (unsigned long *) addr));
        return (offset + set + res);
}







static __inline__ unsigned long ffz(unsigned long word)
{
        __asm__("bsfl %1,%0"
                :"=r" (word)
                :"r" (~word));
        return word;
}
# 27 "/home/bigbinc/kern_src/linux/include/linux/fs.h" 2

struct poll_table_struct;
# 50 "/home/bigbinc/kern_src/linux/include/linux/fs.h"
struct files_stat_struct {
        int nr_files;
        int nr_free_files;
        int max_files;
};
extern struct files_stat_struct files_stat;

struct inodes_stat_t {
        int nr_inodes;
        int nr_unused;
        int dummy[5];
};
extern struct inodes_stat_t inodes_stat;

extern int leases_enable, dir_notify_enable, lease_break_time;
# 18 "/home/bigbinc/kern_src/linux/include/linux/capability.h" 2
# 32 "/home/bigbinc/kern_src/linux/include/linux/capability.h"
typedef struct __user_cap_header_struct {
        __u32 version;
        int pid;
} *cap_user_header_t;

typedef struct __user_cap_data_struct {
        __u32 effective;
        __u32 permitted;
        __u32 inheritable;
} *cap_user_data_t;
# 6 "/home/bigbinc/kern_src/linux/include/linux/binfmts.h" 2
# 10 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/threads.h" 1
# 11 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/kernel.h" 1
# 167 "/home/bigbinc/kern_src/linux/include/linux/kernel.h"
struct sysinfo {
        long uptime;
        unsigned long loads[3];
        unsigned long totalram;
        unsigned long freeram;
        unsigned long sharedram;
        unsigned long bufferram;
        unsigned long totalswap;
        unsigned long freeswap;
        unsigned short procs;
        unsigned short pad;
        unsigned long totalhigh;
        unsigned long freehigh;
        unsigned int mem_unit;
        char _f[20-2*sizeof(long)-sizeof(int)];
};
# 12 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/times.h" 1



struct tms {
        clock_t tms_utime;
        clock_t tms_stime;
        clock_t tms_cutime;
        clock_t tms_cstime;
};
# 14 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/timex.h" 1
# 152 "/home/bigbinc/kern_src/linux/include/linux/timex.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/timex.h" 1
# 10 "/home/bigbinc/kern_src/linux/include/asm/timex.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/msr.h" 1
# 11 "/home/bigbinc/kern_src/linux/include/asm/timex.h" 2
# 37 "/home/bigbinc/kern_src/linux/include/asm/timex.h"
typedef unsigned long long cycles_t;

extern cycles_t cacheflush_time;

static inline cycles_t get_cycles (void)
{

        return 0;






}

extern unsigned long cpu_khz;
# 153 "/home/bigbinc/kern_src/linux/include/linux/timex.h" 2
# 161 "/home/bigbinc/kern_src/linux/include/linux/timex.h"
struct timex {
        unsigned int modes;
        long offset;
        long freq;
        long maxerror;
        long esterror;
        int status;
        long constant;
        long precision;
        long tolerance;


        struct timeval time;
        long tick;

        long ppsfreq;
        long jitter;
        int shift;
        long stabil;
        long jitcnt;
        long calcnt;
        long errcnt;
        long stbcnt;

        int :32; int :32; int :32; int :32;
        int :32; int :32; int :32; int :32;
        int :32; int :32; int :32; int :32;
};
# 15 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/rbtree.h" 1
# 100 "/home/bigbinc/kern_src/linux/include/linux/rbtree.h"
typedef struct rb_node_s
{
        struct rb_node_s * rb_parent;
        int rb_color;


        struct rb_node_s * rb_right;
        struct rb_node_s * rb_left;
}
rb_node_t;

typedef struct rb_root_s
{
        struct rb_node_s * rb_node;
}
rb_root_t;





extern void rb_insert_color(rb_node_t *, rb_root_t *);
extern void rb_erase(rb_node_t *, rb_root_t *);

static inline void rb_link_node(rb_node_t * node, rb_node_t * parent, rb_node_t ** rb_link)
{
        node->rb_parent = parent;
        node->rb_color = 0;
        node->rb_left = node->rb_right = ((void *)0);

        *rb_link = node;
}
# 16 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/asm/system.h" 1





# 1 "/home/bigbinc/kern_src/linux/include/linux/init.h" 1
# 48 "/home/bigbinc/kern_src/linux/include/linux/init.h"
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

extern initcall_t __initcall_start, __initcall_end;
# 61 "/home/bigbinc/kern_src/linux/include/linux/init.h"
struct kernel_param {
        const char *str;
        int (*setup_func)(char *);
};

extern struct kernel_param __setup_start, __setup_end;
# 7 "/home/bigbinc/kern_src/linux/include/asm/system.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/segment.h" 1
# 8 "/home/bigbinc/kern_src/linux/include/asm/system.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/bitops.h" 1
# 11 "/home/bigbinc/kern_src/linux/include/linux/bitops.h"
static inline int generic_ffs(int x)
{
        int r = 1;

        if (!x)
                return 0;
        if (!(x & 0xffff)) {
                x >>= 16;
                r += 16;
        }
        if (!(x & 0xff)) {
                x >>= 8;
                r += 8;
        }
        if (!(x & 0xf)) {
                x >>= 4;
                r += 4;
        }
        if (!(x & 3)) {
                x >>= 2;
                r += 2;
        }
        if (!(x & 1)) {
                x >>= 1;
                r += 1;
        }
        return r;
}






static inline unsigned int generic_hweight32(unsigned int w)
{
        unsigned int res = (w & 0x55555555) + ((w >> 1) & 0x55555555);
        res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
        res = (res & 0x0F0F0F0F) + ((res >> 4) & 0x0F0F0F0F);
        res = (res & 0x00FF00FF) + ((res >> 8) & 0x00FF00FF);
        return (res & 0x0000FFFF) + ((res >> 16) & 0x0000FFFF);
}

static inline unsigned int generic_hweight16(unsigned int w)
{
        unsigned int res = (w & 0x5555) + ((w >> 1) & 0x5555);
        res = (res & 0x3333) + ((res >> 2) & 0x3333);
        res = (res & 0x0F0F) + ((res >> 4) & 0x0F0F);
        return (res & 0x00FF) + ((res >> 8) & 0x00FF);
}

static inline unsigned int generic_hweight8(unsigned int w)
{
        unsigned int res = (w & 0x55) + ((w >> 1) & 0x55);
        res = (res & 0x33) + ((res >> 2) & 0x33);
        return (res & 0x0F) + ((res >> 4) & 0x0F);
}
# 9 "/home/bigbinc/kern_src/linux/include/asm/system.h" 2
# 131 "/home/bigbinc/kern_src/linux/include/asm/system.h"
static inline unsigned long get_limit(unsigned long segment)
{
        unsigned long __limit;
        __asm__("lsll %1,%0"
                :"=r" (__limit):"r" (segment));
        return __limit+1;
}







struct __xchg_dummy { unsigned long a[100]; };
# 158 "/home/bigbinc/kern_src/linux/include/asm/system.h"
static inline void __set_64bit (unsigned long long * ptr,
                unsigned int low, unsigned int high)
{
        __asm__ __volatile__ (
                "\n1:\t"
                "movl (%0), %%eax\n\t"
                "movl 4(%0), %%edx\n\t"
                "cmpxchg8b (%0)\n\t"
                "jnz 1b"
                :
                : "D"(ptr),
                        "b"(low),
                        "c"(high)
                : "ax","dx","memory");
}

static inline void __set_64bit_constant (unsigned long long *ptr,
                                                 unsigned long long value)
{
        __set_64bit(ptr,(unsigned int)(value), (unsigned int)((value)>>32ULL));
}



static inline void __set_64bit_var (unsigned long long *ptr,
                         unsigned long long value)
{
        __set_64bit(ptr,*(((unsigned int*)&(value))+0), *(((unsigned int*)&(value))+1));
}
# 203 "/home/bigbinc/kern_src/linux/include/asm/system.h"
static inline unsigned long __xchg(unsigned long x, volatile void * ptr, int size)
{
        switch (size) {
                case 1:
                        __asm__ __volatile__("xchgb %b0,%1"
                                :"=q" (x)
                                :"m" (*((struct __xchg_dummy *)(ptr))), "0" (x)
                                :"memory");
                        break;
                case 2:
                        __asm__ __volatile__("xchgw %w0,%1"
                                :"=r" (x)
                                :"m" (*((struct __xchg_dummy *)(ptr))), "0" (x)
                                :"memory");
                        break;
                case 4:
                        __asm__ __volatile__("xchgl %0,%1"
                                :"=r" (x)
                                :"m" (*((struct __xchg_dummy *)(ptr))), "0" (x)
                                :"memory");
                        break;
        }
        return x;
}
# 237 "/home/bigbinc/kern_src/linux/include/asm/system.h"
static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old,
                                      unsigned long new, int size)
{
        unsigned long prev;
        switch (size) {
        case 1:
                __asm__ __volatile__("" "cmpxchgb %b1,%2"
                                     : "=a"(prev)
                                     : "q"(new), "m"(*((struct __xchg_dummy *)(ptr))), "0"(old)
                                     : "memory");
                return prev;
        case 2:
                __asm__ __volatile__("" "cmpxchgw %w1,%2"
                                     : "=a"(prev)
                                     : "q"(new), "m"(*((struct __xchg_dummy *)(ptr))), "0"(old)
                                     : "memory");
                return prev;
        case 4:
                __asm__ __volatile__("" "cmpxchgl %1,%2"
                                     : "=a"(prev)
                                     : "q"(new), "m"(*((struct __xchg_dummy *)(ptr))), "0"(old)
                                     : "memory");
                return prev;
        }
        return old;
}
# 350 "/home/bigbinc/kern_src/linux/include/asm/system.h"
void disable_hlt(void);
void enable_hlt(void);

extern unsigned long dmi_broken;
extern int is_sony_vaio_laptop;
# 18 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/semaphore.h" 1
# 19 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/page.h" 1
# 20 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/asm/mmu.h" 1







typedef struct {
        void *segments;
        unsigned long cpuvalid;
} mm_context_t;
# 22 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/smp.h" 1
# 24 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/tty.h" 1
# 25 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/sem.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/linux/ipc.h" 1
# 9 "/home/bigbinc/kern_src/linux/include/linux/ipc.h"
struct ipc_perm
{
        __kernel_key_t key;
        __kernel_uid_t uid;
        __kernel_gid_t gid;
        __kernel_uid_t cuid;
        __kernel_gid_t cgid;
        __kernel_mode_t mode;
        unsigned short seq;
};


# 1 "/home/bigbinc/kern_src/linux/include/asm/ipcbuf.h" 1
# 14 "/home/bigbinc/kern_src/linux/include/asm/ipcbuf.h"
struct ipc64_perm
{
        __kernel_key_t key;
        __kernel_uid32_t uid;
        __kernel_gid32_t gid;
        __kernel_uid32_t cuid;
        __kernel_gid32_t cgid;
        __kernel_mode_t mode;
        unsigned short __pad1;
        unsigned short seq;
        unsigned short __pad2;
        unsigned long __unused1;
        unsigned long __unused2;
};
# 22 "/home/bigbinc/kern_src/linux/include/linux/ipc.h" 2
# 5 "/home/bigbinc/kern_src/linux/include/linux/sem.h" 2
# 23 "/home/bigbinc/kern_src/linux/include/linux/sem.h"
struct semid_ds {
        struct ipc_perm sem_perm;
        __kernel_time_t sem_otime;
        __kernel_time_t sem_ctime;
        struct sem *sem_base;
        struct sem_queue *sem_pending;
        struct sem_queue **sem_pending_last;
        struct sem_undo *undo;
        unsigned short sem_nsems;
};


# 1 "/home/bigbinc/kern_src/linux/include/asm/sembuf.h" 1
# 14 "/home/bigbinc/kern_src/linux/include/asm/sembuf.h"
struct semid64_ds {
        struct ipc64_perm sem_perm;
        __kernel_time_t sem_otime;
        unsigned long __unused1;
        __kernel_time_t sem_ctime;
        unsigned long __unused2;
        unsigned long sem_nsems;
        unsigned long __unused3;
        unsigned long __unused4;
};
# 36 "/home/bigbinc/kern_src/linux/include/linux/sem.h" 2


struct sembuf {
        unsigned short sem_num;
        short sem_op;
        short sem_flg;
};


union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
        void *__pad;
};

struct seminfo {
        int semmap;
        int semmni;
        int semmns;
        int semmnu;
        int semmsl;
        int semopm;
        int semume;
        int semusz;
        int semvmx;
        int semaem;
};
# 26 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/securebits.h" 1





extern unsigned securebits;
# 28 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/fs_struct.h" 1
# 29 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

struct exec_domain;
# 58 "/home/bigbinc/kern_src/linux/include/linux/sched.h"
extern unsigned long avenrun[];
# 75 "/home/bigbinc/kern_src/linux/include/linux/sched.h"
extern int nr_running, nr_threads;
extern int last_pid;


# 1 "/home/bigbinc/kern_src/linux/include/linux/time.h" 1
# 9 "/home/bigbinc/kern_src/linux/include/linux/time.h"
struct timespec {
        time_t tv_sec;
        long tv_nsec;
};
# 88 "/home/bigbinc/kern_src/linux/include/linux/time.h"
struct timeval {
        time_t tv_sec;
        suseconds_t tv_usec;
};

struct timezone {
        int tz_minuteswest;
        int tz_dsttime;
};
# 119 "/home/bigbinc/kern_src/linux/include/linux/time.h"
struct itimerspec {
        struct timespec it_interval;
        struct timespec it_value;
};

struct itimerval {
        struct timeval it_interval;
        struct timeval it_value;
};
# 80 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/param.h" 1
# 81 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/resource.h" 1
# 21 "/home/bigbinc/kern_src/linux/include/linux/resource.h"
struct rusage {
        struct timeval ru_utime;
        struct timeval ru_stime;
        long ru_maxrss;
        long ru_ixrss;
        long ru_idrss;
        long ru_isrss;
        long ru_minflt;
        long ru_majflt;
        long ru_nswap;
        long ru_inblock;
        long ru_oublock;
        long ru_msgsnd;
        long ru_msgrcv;
        long ru_nsignals;
        long ru_nvcsw;
        long ru_nivcsw;
};

struct rlimit {
        unsigned long rlim_cur;
        unsigned long rlim_max;
};
# 56 "/home/bigbinc/kern_src/linux/include/linux/resource.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/resource.h" 1
# 57 "/home/bigbinc/kern_src/linux/include/linux/resource.h" 2
# 82 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/timer.h" 1
# 16 "/home/bigbinc/kern_src/linux/include/linux/timer.h"
struct timer_list {
        struct list_head list;
        unsigned long expires;
        unsigned long data;
        void (*function)(unsigned long);
};

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);
# 41 "/home/bigbinc/kern_src/linux/include/linux/timer.h"
int mod_timer(struct timer_list *timer, unsigned long expires);

extern void it_real_fn(unsigned long);

static inline void init_timer(struct timer_list * timer)
{
        timer->list.next = timer->list.prev = ((void *)0);
}

static inline int timer_pending (const struct timer_list * timer)
{
        return timer->list.next != ((void *)0);
}
# 83 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/asm/processor.h" 1
# 10 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/vm86.h" 1
# 68 "/home/bigbinc/kern_src/linux/include/asm/vm86.h"
struct vm86_regs {



        long ebx;
        long ecx;
        long edx;
        long esi;
        long edi;
        long ebp;
        long eax;
        long __null_ds;
        long __null_es;
        long __null_fs;
        long __null_gs;
        long orig_eax;
        long eip;
        unsigned short cs, __csh;
        long eflags;
        long esp;
        unsigned short ss, __ssh;



        unsigned short es, __esh;
        unsigned short ds, __dsh;
        unsigned short fs, __fsh;
        unsigned short gs, __gsh;
};

struct revectored_struct {
        unsigned long __map[8];
};

struct vm86_struct {
        struct vm86_regs regs;
        unsigned long flags;
        unsigned long screen_bitmap;
        unsigned long cpu_type;
        struct revectored_struct int_revectored;
        struct revectored_struct int21_revectored;
};






struct vm86plus_info_struct {
        unsigned long force_return_for_pic:1;
        unsigned long vm86dbg_active:1;
        unsigned long vm86dbg_TFpendig:1;
        unsigned long unused:28;
        unsigned long is_vm86pus:1;
        unsigned char vm86dbg_intxxtab[32];
};

struct vm86plus_struct {
        struct vm86_regs regs;
        unsigned long flags;
        unsigned long screen_bitmap;
        unsigned long cpu_type;
        struct revectored_struct int_revectored;
        struct revectored_struct int21_revectored;
        struct vm86plus_info_struct vm86plus;
};
# 11 "/home/bigbinc/kern_src/linux/include/asm/processor.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/math_emu.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/sigcontext.h" 1
# 18 "/home/bigbinc/kern_src/linux/include/asm/sigcontext.h"
struct _fpreg {
        unsigned short significand[4];
        unsigned short exponent;
};

struct _fpxreg {
        unsigned short significand[4];
        unsigned short exponent;
        unsigned short padding[3];
};

struct _xmmreg {
        unsigned long element[4];
};

struct _fpstate {

        unsigned long cw;
        unsigned long sw;
        unsigned long tag;
        unsigned long ipoff;
        unsigned long cssel;
        unsigned long dataoff;
        unsigned long datasel;
        struct _fpreg _st[8];
        unsigned short status;
        unsigned short magic;


        unsigned long _fxsr_env[6];
        unsigned long mxcsr;
        unsigned long reserved;
        struct _fpxreg _fxsr_st[8];
        struct _xmmreg _xmm[8];
        unsigned long padding[56];
};



struct sigcontext {
        unsigned short gs, __gsh;
        unsigned short fs, __fsh;
        unsigned short es, __esh;
        unsigned short ds, __dsh;
        unsigned long edi;
        unsigned long esi;
        unsigned long ebp;
        unsigned long esp;
        unsigned long ebx;
        unsigned long edx;
        unsigned long ecx;
        unsigned long eax;
        unsigned long trapno;
        unsigned long err;
        unsigned long eip;
        unsigned short cs, __csh;
        unsigned long eflags;
        unsigned long esp_at_signal;
        unsigned short ss, __ssh;
        struct _fpstate * fpstate;
        unsigned long oldmask;
        unsigned long cr2;
};
# 5 "/home/bigbinc/kern_src/linux/include/asm/math_emu.h" 2

int restore_i387_soft(void *s387, struct _fpstate *buf);
int save_i387_soft(void *s387, struct _fpstate * buf);





struct info {
        long ___orig_eip;
        long ___ebx;
        long ___ecx;
        long ___edx;
        long ___esi;
        long ___edi;
        long ___ebp;
        long ___eax;
        long ___ds;
        long ___es;
        long ___orig_eax;
        long ___eip;
        long ___cs;
        long ___eflags;
        long ___esp;
        long ___ss;
        long ___vm86_es;
        long ___vm86_ds;
        long ___vm86_fs;
        long ___vm86_gs;
};
# 12 "/home/bigbinc/kern_src/linux/include/asm/processor.h" 2




# 1 "/home/bigbinc/kern_src/linux/include/asm/cpufeature.h" 1
# 17 "/home/bigbinc/kern_src/linux/include/asm/processor.h" 2
# 33 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
struct cpuinfo_x86 {
        __u8 x86;
        __u8 x86_vendor;
        __u8 x86_model;
        __u8 x86_mask;
        char wp_works_ok;
        char hlt_works_ok;
        char hard_math;
        char rfu;
        int cpuid_level;
        __u32 x86_capability[4];
        char x86_vendor_id[16];
        char x86_model_id[64];
        int x86_cache_size;

        int fdiv_bug;
        int f00f_bug;
        int coma_bug;
        unsigned long loops_per_jiffy;
        unsigned long *pgd_quick;
        unsigned long *pmd_quick;
        unsigned long *pte_quick;
        unsigned long pgtable_cache_sz;
} __attribute__((__aligned__((1 << ((4))))));
# 72 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
extern struct cpuinfo_x86 boot_cpu_data;
extern struct tss_struct init_tss[1];
# 94 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
extern char ignore_irq13;

extern void identify_cpu(struct cpuinfo_x86 *);
extern void print_cpu_info(struct cpuinfo_x86 *);
extern void dodgy_tsc(void);
# 124 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
static inline void cpuid(int op, int *eax, int *ebx, int *ecx, int *edx)
{
        __asm__("cpuid"
                : "=a" (*eax),
                  "=b" (*ebx),
                  "=c" (*ecx),
                  "=d" (*edx)
                : "0" (op));
}




static inline unsigned int cpuid_eax(unsigned int op)
{
        unsigned int eax;

        __asm__("cpuid"
                : "=a" (eax)
                : "0" (op)
                : "bx", "cx", "dx");
        return eax;
}
static inline unsigned int cpuid_ebx(unsigned int op)
{
        unsigned int eax, ebx;

        __asm__("cpuid"
                : "=a" (eax), "=b" (ebx)
                : "0" (op)
                : "cx", "dx" );
        return ebx;
}
static inline unsigned int cpuid_ecx(unsigned int op)
{
        unsigned int eax, ecx;

        __asm__("cpuid"
                : "=a" (eax), "=c" (ecx)
                : "0" (op)
                : "bx", "dx" );
        return ecx;
}
static inline unsigned int cpuid_edx(unsigned int op)
{
        unsigned int eax, edx;

        __asm__("cpuid"
                : "=a" (eax), "=d" (edx)
                : "0" (op)
                : "bx", "cx");
        return edx;
}
# 199 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
extern unsigned long mmu_cr4_features;

static inline void set_in_cr4 (unsigned long mask)
{
        mmu_cr4_features |= mask;
        __asm__("movl %%cr4,%%eax\n\t"
                "orl %0,%%eax\n\t"
                "movl %%eax,%%cr4\n"
                : : "irg" (mask)
                :"ax");
}

static inline void clear_in_cr4 (unsigned long mask)
{
        mmu_cr4_features &= ~mask;
        __asm__("movl %%cr4,%%eax\n\t"
                "andl %0,%%eax\n\t"
                "movl %%eax,%%cr4\n"
                : : "irg" (~mask)
                :"ax");
}
# 256 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
extern int MCA_bus;



extern unsigned int machine_id;
extern unsigned int machine_submodel_id;
extern unsigned int BIOS_revision;
extern unsigned int mca_pentium_flag;
# 282 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
struct i387_fsave_struct {
        long cwd;
        long swd;
        long twd;
        long fip;
        long fcs;
        long foo;
        long fos;
        long st_space[20];
        long status;
};

struct i387_fxsave_struct {
        unsigned short cwd;
        unsigned short swd;
        unsigned short twd;
        unsigned short fop;
        long fip;
        long fcs;
        long foo;
        long fos;
        long mxcsr;
        long reserved;
        long st_space[32];
        long xmm_space[32];
        long padding[56];
} __attribute__ ((aligned (16)));

struct i387_soft_struct {
        long cwd;
        long swd;
        long twd;
        long fip;
        long fcs;
        long foo;
        long fos;
        long st_space[20];
        unsigned char ftop, changed, lookahead, no_update, rm, alimit;
        struct info *info;
        unsigned long entry_eip;
};

union i387_union {
        struct i387_fsave_struct fsave;
        struct i387_fxsave_struct fxsave;
        struct i387_soft_struct soft;
};

typedef struct {
        unsigned long seg;
} mm_segment_t;

struct tss_struct {
        unsigned short back_link,__blh;
        unsigned long esp0;
        unsigned short ss0,__ss0h;
        unsigned long esp1;
        unsigned short ss1,__ss1h;
        unsigned long esp2;
        unsigned short ss2,__ss2h;
        unsigned long __cr3;
        unsigned long eip;
        unsigned long eflags;
        unsigned long eax,ecx,edx,ebx;
        unsigned long esp;
        unsigned long ebp;
        unsigned long esi;
        unsigned long edi;
        unsigned short es, __esh;
        unsigned short cs, __csh;
        unsigned short ss, __ssh;
        unsigned short ds, __dsh;
        unsigned short fs, __fsh;
        unsigned short gs, __gsh;
        unsigned short ldt, __ldth;
        unsigned short trace, bitmap;
        unsigned long io_bitmap[32 +1];



        unsigned long __cacheline_filler[5];
};

struct thread_struct {
        unsigned long esp0;
        unsigned long eip;
        unsigned long esp;
        unsigned long fs;
        unsigned long gs;

        unsigned long debugreg[8];

        unsigned long cr2, trap_no, error_code;

        union i387_union i387;

        struct vm86_struct * vm86_info;
        unsigned long screen_bitmap;
        unsigned long v86flags, v86mask, v86mode, saved_esp0;

        int ioperm;
        unsigned long io_bitmap[32 +1];
};
# 424 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
struct task_struct;
struct mm_struct;


extern void release_thread(struct task_struct *);



extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);


extern void copy_segments(struct task_struct *p, struct mm_struct * mm);
extern void release_segments(struct mm_struct * mm);




static inline unsigned long thread_saved_pc(struct thread_struct *t)
{
        return ((unsigned long *)t->esp)[3];
}

unsigned long get_wchan(struct task_struct *p);
# 458 "/home/bigbinc/kern_src/linux/include/asm/processor.h"
struct microcode {
        unsigned int hdrver;
        unsigned int rev;
        unsigned int date;
        unsigned int sig;
        unsigned int cksum;
        unsigned int ldrver;
        unsigned int pf;
        unsigned int reserved[5];
        unsigned int bits[500];
};





static inline void rep_nop(void)
{
        __asm__ __volatile__("rep;nop");
}
# 85 "/home/bigbinc/kern_src/linux/include/linux/sched.h" 2
# 125 "/home/bigbinc/kern_src/linux/include/linux/sched.h"
struct sched_param {
        int sched_priority;
};

struct completion;
# 6 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/ioport.h" 1
# 15 "/home/bigbinc/kern_src/linux/include/linux/ioport.h"
struct resource {
        const char *name;
        unsigned long start, end;
        unsigned long flags;
        struct resource *parent, *sibling, *child;
};

struct resource_list {
        struct resource_list *next;
        struct resource *res;
        struct pci_dev *dev;
};
# 83 "/home/bigbinc/kern_src/linux/include/linux/ioport.h"
extern struct resource ioport_resource;
extern struct resource iomem_resource;

extern int get_resource_list(struct resource *, char *buf, int size);

extern int check_resource(struct resource *root, unsigned long, unsigned long);
extern int request_resource(struct resource *root, struct resource *new);
extern int release_resource(struct resource *new);
extern int allocate_resource(struct resource *root, struct resource *new,
                             unsigned long size,
                             unsigned long min, unsigned long max,
                             unsigned long align,
                             void (*alignf)(void *, struct resource *, unsigned long),
                             void *alignf_data);





extern struct resource * __request_region(struct resource *, unsigned long start, unsigned long n, const char *name);







extern int __check_region(struct resource *, unsigned long, unsigned long);
extern void __release_region(struct resource *, unsigned long, unsigned long);





extern void autoirq_setup(int waittime);
extern int autoirq_report(int waittime);
# 7 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h" 1
# 14 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
struct irqaction {
        void (*handler)(int, void *, struct pt_regs *);
        unsigned long flags;
        unsigned long mask;
        const char *name;
        void *dev_id;
        struct irqaction *next;
};





enum {
        TIMER_BH = 0,
        TQUEUE_BH,
        DIGI_BH,
        SERIAL_BH,
        RISCOM8_BH,
        SPECIALIX_BH,
        AURORA_BH,
        ESP_BH,
        SCSI_BH,
        IMMEDIATE_BH,
        CYCLADES_BH,
        CM206_BH,
        JS_BH,
        MACSERIAL_BH,
        ISICOM_BH
};

# 1 "/home/bigbinc/kern_src/linux/include/asm/hardirq.h" 1





# 1 "/home/bigbinc/kern_src/linux/include/linux/irq.h" 1
# 17 "/home/bigbinc/kern_src/linux/include/linux/irq.h"
# 1 "/home/bigbinc/kern_src/linux/include/linux/spinlock.h" 1
# 57 "/home/bigbinc/kern_src/linux/include/linux/spinlock.h"
  typedef struct { } spinlock_t;
# 120 "/home/bigbinc/kern_src/linux/include/linux/spinlock.h"
  typedef struct { } rwlock_t;
# 18 "/home/bigbinc/kern_src/linux/include/linux/irq.h" 2

# 1 "/home/bigbinc/kern_src/linux/include/asm/irq.h" 1
# 28 "/home/bigbinc/kern_src/linux/include/asm/irq.h"
static __inline__ int irq_cannonicalize(int irq)
{
        return ((irq == 2) ? 9 : irq);
}

extern void disable_irq(unsigned int);
extern void disable_irq_nosync(unsigned int);
extern void enable_irq(unsigned int);
extern void release_x86_irqs(struct task_struct *);
# 20 "/home/bigbinc/kern_src/linux/include/linux/irq.h" 2
# 39 "/home/bigbinc/kern_src/linux/include/linux/irq.h"
struct hw_interrupt_type {
        const char * typename;
        unsigned int (*startup)(unsigned int irq);
        void (*shutdown)(unsigned int irq);
        void (*enable)(unsigned int irq);
        void (*disable)(unsigned int irq);
        void (*ack)(unsigned int irq);
        void (*end)(unsigned int irq);
        void (*set_affinity)(unsigned int irq, unsigned long mask);
};

typedef struct hw_interrupt_type hw_irq_controller;
# 59 "/home/bigbinc/kern_src/linux/include/linux/irq.h"
typedef struct {
        unsigned int status;
        hw_irq_controller *handler;
        struct irqaction *action;
        unsigned int depth;
        spinlock_t lock;
} __attribute__((__aligned__((1 << ((4)))))) irq_desc_t;

extern irq_desc_t irq_desc [224];

# 1 "/home/bigbinc/kern_src/linux/include/asm/hw_irq.h" 1
# 61 "/home/bigbinc/kern_src/linux/include/asm/hw_irq.h"
extern int irq_vector[224];
# 71 "/home/bigbinc/kern_src/linux/include/asm/hw_irq.h"
extern void mask_irq(unsigned int irq);
extern void unmask_irq(unsigned int irq);
extern void disable_8259A_irq(unsigned int irq);
extern void enable_8259A_irq(unsigned int irq);
extern int i8259A_irq_pending(unsigned int irq);
extern void make_8259A_irq(unsigned int irq);
extern void init_8259A(int aeoi);
extern void FASTCALL(send_IPI_self(int vector));
extern void init_VISWS_APIC_irqs(void);
extern void setup_IO_APIC(void);
extern void disable_IO_APIC(void);
extern void print_IO_APIC(void);
extern int IO_APIC_get_PCI_irq_vector(int bus, int slot, int fn);
extern void send_IPI(int dest, int vector);

extern unsigned long io_apic_irqs;

extern atomic_t irq_err_count;
extern atomic_t irq_mis_count;

extern char _stext, _etext;
# 183 "/home/bigbinc/kern_src/linux/include/asm/hw_irq.h"
extern unsigned long prof_cpu_mask;
extern unsigned int * prof_buffer;
extern unsigned long prof_len;
extern unsigned long prof_shift;





static inline void x86_do_profile (unsigned long eip)
{
        if (!prof_buffer)
                return;





        if (!((1<<0) & prof_cpu_mask))
                return;

        eip -= (unsigned long) &_stext;
        eip >>= prof_shift;





        if (eip > prof_len-1)
                eip = prof_len-1;
        atomic_inc((atomic_t *)&prof_buffer[eip]);
}







static inline void hw_resend_irq(struct hw_interrupt_type *h, unsigned int i) {}
# 70 "/home/bigbinc/kern_src/linux/include/linux/irq.h" 2

extern int handle_IRQ_event(unsigned int, struct pt_regs *, struct irqaction *);
extern int setup_irq(unsigned int , struct irqaction * );

extern hw_irq_controller no_irq_type;
extern void no_action(int cpl, void *dev_id, struct pt_regs *regs);
# 7 "/home/bigbinc/kern_src/linux/include/asm/hardirq.h" 2


typedef struct {
        unsigned int __softirq_pending;
        unsigned int __local_irq_count;
        unsigned int __local_bh_count;
        unsigned int __syscall_count;
        struct task_struct * __ksoftirqd_task;
        unsigned int __nmi_count;
} __attribute__((__aligned__((1 << ((4)))))) irq_cpustat_t;

# 1 "/home/bigbinc/kern_src/linux/include/linux/irq_cpustat.h" 1
# 20 "/home/bigbinc/kern_src/linux/include/linux/irq_cpustat.h"
extern irq_cpustat_t irq_stat[];
# 19 "/home/bigbinc/kern_src/linux/include/asm/hardirq.h" 2
# 46 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/softirq.h" 1





# 1 "/home/bigbinc/kern_src/linux/include/linux/stringify.h" 1
# 7 "/home/bigbinc/kern_src/linux/include/asm/softirq.h" 2
# 47 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h" 2
# 56 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
enum
{
        HI_SOFTIRQ=0,
        NET_TX_SOFTIRQ,
        NET_RX_SOFTIRQ,
        TASKLET_SOFTIRQ
};





struct softirq_action
{
        void (*action)(struct softirq_action *);
        void *data;
};

 __attribute__((regparm(0))) void do_softirq(void);
extern void open_softirq(int nr, void (*action)(struct softirq_action*), void *data);
extern void softirq_init(void);

extern void FASTCALL(cpu_raise_softirq(unsigned int cpu, unsigned int nr));
extern void FASTCALL(raise_softirq(unsigned int nr));
# 103 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
struct tasklet_struct
{
        struct tasklet_struct *next;
        unsigned long state;
        atomic_t count;
        void (*func)(unsigned long);
        unsigned long data;
};
# 119 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
enum
{
        TASKLET_STATE_SCHED,
        TASKLET_STATE_RUN
};

struct tasklet_head
{
        struct tasklet_struct *list;
} __attribute__ ((__aligned__((1 << ((4))))));

extern struct tasklet_head tasklet_vec[1];
extern struct tasklet_head tasklet_hi_vec[1];
# 155 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
extern void FASTCALL(__tasklet_schedule(struct tasklet_struct *t));

static inline void tasklet_schedule(struct tasklet_struct *t)
{
        if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
                __tasklet_schedule(t);
}

extern void FASTCALL(__tasklet_hi_schedule(struct tasklet_struct *t));

static inline void tasklet_hi_schedule(struct tasklet_struct *t)
{
        if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
                __tasklet_hi_schedule(t);
}


static inline void tasklet_disable_nosync(struct tasklet_struct *t)
{
        atomic_inc(&t->count);
        barrier();
}

static inline void tasklet_disable(struct tasklet_struct *t)
{
        tasklet_disable_nosync(t);
        do { } while (0);
        barrier();
}

static inline void tasklet_enable(struct tasklet_struct *t)
{
        barrier();
        atomic_dec(&t->count);
}

static inline void tasklet_hi_enable(struct tasklet_struct *t)
{
        barrier();
        atomic_dec(&t->count);
}

extern void tasklet_kill(struct tasklet_struct *t);
extern void tasklet_init(struct tasklet_struct *t,
                         void (*func)(unsigned long), unsigned long data);
# 222 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
extern struct tasklet_struct bh_task_vec[];


extern spinlock_t global_bh_lock;

static inline void mark_bh(int nr)
{
        tasklet_hi_schedule(bh_task_vec+nr);
}

extern void init_bh(int nr, void (*routine)(void));
extern void remove_bh(int nr);
# 263 "/home/bigbinc/kern_src/linux/include/linux/interrupt.h"
extern unsigned long probe_irq_on(void);
extern int probe_irq_off(unsigned long);
extern unsigned int probe_irq_mask(unsigned long);
# 8 "i8259.c" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/slab.h" 1
# 10 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/random.h" 1
# 35 "/home/bigbinc/kern_src/linux/include/linux/random.h"
struct rand_pool_info {
        int entropy_count;
        int buf_size;
        __u32 buf[0];
};
# 11 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/linux/smp_lock.h" 1
# 12 "i8259.c" 2

# 1 "/home/bigbinc/kern_src/linux/include/linux/kernel_stat.h" 1
# 18 "/home/bigbinc/kern_src/linux/include/linux/kernel_stat.h"
struct kernel_stat {
        unsigned int per_cpu_user[1],
                     per_cpu_nice[1],
                     per_cpu_system[1];
        unsigned int dk_drive[16][16];
        unsigned int dk_drive_rio[16][16];
        unsigned int dk_drive_wio[16][16];
        unsigned int dk_drive_rblk[16][16];
        unsigned int dk_drive_wblk[16][16];
        unsigned int pgpgin, pgpgout;
        unsigned int pswpin, pswpout;

        unsigned int irqs[1][224];

        unsigned int ipackets, opackets;
        unsigned int ierrors, oerrors;
        unsigned int collisions;
        unsigned int context_swtch;
};

extern struct kernel_stat kstat;





static inline int kstat_irqs (int irq)
{
        int i, sum=0;

        for (i = 0 ; i < 1 ; i++)
                sum += kstat.irqs[0][irq];

        return sum;
}
# 14 "i8259.c" 2



# 1 "/home/bigbinc/kern_src/linux/include/asm/io.h" 1
# 309 "/home/bigbinc/kern_src/linux/include/asm/io.h"
static inline unsigned char inb(unsigned short port) { unsigned char _v; __asm__ __volatile__ ("in" "b" " %" "w" "1,%" "" "0" : "=a" (_v) : "Nd" (port) ); return _v; } static inline unsigned char inb_p(unsigned short port) { unsigned char _v; __asm__ __volatile__ ("in" "b" " %" "w" "1,%" "" "0" "\noutb %%al,$0x80" : "=a" (_v) : "Nd" (port) ); return _v; }


static inline unsigned short inw(unsigned short port) { unsigned short _v; __asm__ __volatile__ ("in" "w" " %" "w" "1,%" "" "0" : "=a" (_v) : "Nd" (port) ); return _v; } static inline unsigned short inw_p(unsigned short port) { unsigned short _v; __asm__ __volatile__ ("in" "w" " %" "w" "1,%" "" "0" "\noutb %%al,$0x80" : "=a" (_v) : "Nd" (port) ); return _v; }


static inline unsigned int inl(unsigned short port) { unsigned int _v; __asm__ __volatile__ ("in" "l" " %" "w" "1,%" "" "0" : "=a" (_v) : "Nd" (port) ); return _v; } static inline unsigned int inl_p(unsigned short port) { unsigned int _v; __asm__ __volatile__ ("in" "l" " %" "w" "1,%" "" "0" "\noutb %%al,$0x80" : "=a" (_v) : "Nd" (port) ); return _v; }


static inline void outb(unsigned char value, unsigned short port) { __asm__ __volatile__ ("out" "b" " %" "b" "0,%" "w" "1" : : "a" (value), "Nd" (port)); } static inline void outb_p(unsigned char value, unsigned short port) { __asm__ __volatile__ ("out" "b" " %" "b" "0,%" "w" "1" "\noutb %%al,$0x80" : : "a" (value), "Nd" (port));}
static inline void outw(unsigned short value, unsigned short port) { __asm__ __volatile__ ("out" "w" " %" "w" "0,%" "w" "1" : : "a" (value), "Nd" (port)); } static inline void outw_p(unsigned short value, unsigned short port) { __asm__ __volatile__ ("out" "w" " %" "w" "0,%" "w" "1" "\noutb %%al,$0x80" : : "a" (value), "Nd" (port));}
static inline void outl(unsigned int value, unsigned short port) { __asm__ __volatile__ ("out" "l" " %" "0,%" "w" "1" : : "a" (value), "Nd" (port)); } static inline void outl_p(unsigned int value, unsigned short port) { __asm__ __volatile__ ("out" "l" " %" "0,%" "w" "1" "\noutb %%al,$0x80" : : "a" (value), "Nd" (port));}

static inline void insb(unsigned short port, void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; ins" "b" : "=D" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }
static inline void insw(unsigned short port, void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; ins" "w" : "=D" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }
static inline void insl(unsigned short port, void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; ins" "l" : "=D" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }

static inline void outsb(unsigned short port, const void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; outs" "b" : "=S" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }
static inline void outsw(unsigned short port, const void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; outs" "w" : "=S" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }
static inline void outsl(unsigned short port, const void * addr, unsigned long count) { __asm__ __volatile__ ("rep ; outs" "l" : "=S" (addr), "=c" (count) : "d" (port),"0" (addr),"1" (count)); }
# 18 "i8259.c" 2


# 1 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h" 1
# 17 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h" 1
# 18 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/apicdef.h" 1
# 122 "/home/bigbinc/kern_src/linux/include/asm/apicdef.h"
struct local_apic {

        struct { unsigned int __reserved[4]; } __reserved_01;

        struct { unsigned int __reserved[4]; } __reserved_02;

        struct {
                unsigned int __reserved_1 : 24,
                        phys_apic_id : 4,
                        __reserved_2 : 4;
                unsigned int __reserved[3];
        } id;

        const
        struct {
                unsigned int version : 8,
                        __reserved_1 : 8,
                        max_lvt : 8,
                        __reserved_2 : 8;
                unsigned int __reserved[3];
        } version;

        struct { unsigned int __reserved[4]; } __reserved_03;

        struct { unsigned int __reserved[4]; } __reserved_04;

        struct { unsigned int __reserved[4]; } __reserved_05;

        struct { unsigned int __reserved[4]; } __reserved_06;

        struct {
                unsigned int priority : 8,
                        __reserved_1 : 24;
                unsigned int __reserved_2[3];
        } tpr;

        const
        struct {
                unsigned int priority : 8,
                        __reserved_1 : 24;
                unsigned int __reserved_2[3];
        } apr;

        const
        struct {
                unsigned int priority : 8,
                        __reserved_1 : 24;
                unsigned int __reserved_2[3];
        } ppr;

        struct {
                unsigned int eoi;
                unsigned int __reserved[3];
        } eoi;

        struct { unsigned int __reserved[4]; } __reserved_07;

        struct {
                unsigned int __reserved_1 : 24,
                        logical_dest : 8;
                unsigned int __reserved_2[3];
        } ldr;

        struct {
                unsigned int __reserved_1 : 28,
                        model : 4;
                unsigned int __reserved_2[3];
        } dfr;

        struct {
                unsigned int spurious_vector : 8,
                        apic_enabled : 1,
                        focus_cpu : 1,
                        __reserved_2 : 22;
                unsigned int __reserved_3[3];
        } svr;

        struct {
                unsigned int bitfield;
                unsigned int __reserved[3];
        } isr [8];

        struct {
                unsigned int bitfield;
                unsigned int __reserved[3];
        } tmr [8];

        struct {
                unsigned int bitfield;
                unsigned int __reserved[3];
        } irr [8];

        union {
                struct {
                        unsigned int send_cs_error : 1,
                                receive_cs_error : 1,
                                send_accept_error : 1,
                                receive_accept_error : 1,
                                __reserved_1 : 1,
                                send_illegal_vector : 1,
                                receive_illegal_vector : 1,
                                illegal_register_address : 1,
                                __reserved_2 : 24;
                        unsigned int __reserved_3[3];
                } error_bits;
                struct {
                        unsigned int errors;
                        unsigned int __reserved_3[3];
                } all_errors;
        } esr;

        struct { unsigned int __reserved[4]; } __reserved_08;

        struct { unsigned int __reserved[4]; } __reserved_09;

        struct { unsigned int __reserved[4]; } __reserved_10;

        struct { unsigned int __reserved[4]; } __reserved_11;

        struct { unsigned int __reserved[4]; } __reserved_12;

        struct { unsigned int __reserved[4]; } __reserved_13;

        struct { unsigned int __reserved[4]; } __reserved_14;

        struct {
                unsigned int vector : 8,
                        delivery_mode : 3,
                        destination_mode : 1,
                        delivery_status : 1,
                        __reserved_1 : 1,
                        level : 1,
                        trigger : 1,
                        __reserved_2 : 2,
                        shorthand : 2,
                        __reserved_3 : 12;
                unsigned int __reserved_4[3];
        } icr1;

        struct {
                union {
                        unsigned int __reserved_1 : 24,
                                phys_dest : 4,
                                __reserved_2 : 4;
                        unsigned int __reserved_3 : 24,
                                logical_dest : 8;
                } dest;
                unsigned int __reserved_4[3];
        } icr2;

        struct {
                unsigned int vector : 8,
                        __reserved_1 : 4,
                        delivery_status : 1,
                        __reserved_2 : 3,
                        mask : 1,
                        timer_mode : 1,
                        __reserved_3 : 14;
                unsigned int __reserved_4[3];
        } lvt_timer;

        struct { unsigned int __reserved[4]; } __reserved_15;

        struct {
                unsigned int vector : 8,
                        delivery_mode : 3,
                        __reserved_1 : 1,
                        delivery_status : 1,
                        __reserved_2 : 3,
                        mask : 1,
                        __reserved_3 : 15;
                unsigned int __reserved_4[3];
        } lvt_pc;

        struct {
                unsigned int vector : 8,
                        delivery_mode : 3,
                        __reserved_1 : 1,
                        delivery_status : 1,
                        polarity : 1,
                        remote_irr : 1,
                        trigger : 1,
                        mask : 1,
                        __reserved_2 : 15;
                unsigned int __reserved_3[3];
        } lvt_lint0;

        struct {
                unsigned int vector : 8,
                        delivery_mode : 3,
                        __reserved_1 : 1,
                        delivery_status : 1,
                        polarity : 1,
                        remote_irr : 1,
                        trigger : 1,
                        mask : 1,
                        __reserved_2 : 15;
                unsigned int __reserved_3[3];
        } lvt_lint1;

        struct {
                unsigned int vector : 8,
                        __reserved_1 : 4,
                        delivery_status : 1,
                        __reserved_2 : 3,
                        mask : 1,
                        __reserved_3 : 15;
                unsigned int __reserved_4[3];
        } lvt_error;

        struct {
                unsigned int initial_count;
                unsigned int __reserved_2[3];
        } timer_icr;

        const
        struct {
                unsigned int curr_count;
                unsigned int __reserved_2[3];
        } timer_ccr;

        struct { unsigned int __reserved[4]; } __reserved_16;

        struct { unsigned int __reserved[4]; } __reserved_17;

        struct { unsigned int __reserved[4]; } __reserved_18;

        struct { unsigned int __reserved[4]; } __reserved_19;

        struct {
                unsigned int divisor : 4,
                        __reserved_1 : 28;
                unsigned int __reserved_2[3];
        } timer_dcr;

        struct { unsigned int __reserved[4]; } __reserved_20;

} __attribute__ ((packed));
# 19 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h" 2
# 50 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h"
enum fixed_addresses {
# 68 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h"
        __end_of_fixed_addresses
};

extern void __set_fixmap (enum fixed_addresses idx,
                                        unsigned long phys, pgprot_t flags);
# 94 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h"
extern void __this_fixmap_does_not_exist(void);






static inline unsigned long fix_to_virt(const unsigned int idx)
{
# 112 "/home/bigbinc/kern_src/linux/include/asm/fixmap.h"
        if (idx >= __end_of_fixed_addresses)
                __this_fixmap_does_not_exist();

        return ((0xffffe000UL) - ((idx) << 12));
}
# 18 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h" 2






extern pgd_t swapper_pg_dir[1024];
extern void paging_init(void);
# 67 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
extern unsigned long pgkern_mask;
# 95 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
extern unsigned long empty_zero_page[1024];
# 115 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
# 1 "/home/bigbinc/kern_src/linux/include/asm/pgtable-2level.h" 1
# 32 "/home/bigbinc/kern_src/linux/include/asm/pgtable-2level.h"
static inline int pgd_none(pgd_t pgd) { return 0; }
static inline int pgd_bad(pgd_t pgd) { return 0; }
static inline int pgd_present(pgd_t pgd) { return 1; }
# 53 "/home/bigbinc/kern_src/linux/include/asm/pgtable-2level.h"
static inline pmd_t * pmd_offset(pgd_t * dir, unsigned long address)
{
        return (pmd_t *) dir;
}
# 116 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h" 2
# 256 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
extern unsigned long pg0[1024];
# 277 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
static inline int pte_read(pte_t pte) { return (pte).pte_low & 0x004; }
static inline int pte_exec(pte_t pte) { return (pte).pte_low & 0x004; }
static inline int pte_dirty(pte_t pte) { return (pte).pte_low & 0x040; }
static inline int pte_young(pte_t pte) { return (pte).pte_low & 0x020; }
static inline int pte_write(pte_t pte) { return (pte).pte_low & 0x002; }

static inline pte_t pte_rdprotect(pte_t pte) { (pte).pte_low &= ~0x004; return pte; }
static inline pte_t pte_exprotect(pte_t pte) { (pte).pte_low &= ~0x004; return pte; }
static inline pte_t pte_mkclean(pte_t pte) { (pte).pte_low &= ~0x040; return pte; }
static inline pte_t pte_mkold(pte_t pte) { (pte).pte_low &= ~0x020; return pte; }
static inline pte_t pte_wrprotect(pte_t pte) { (pte).pte_low &= ~0x002; return pte; }
static inline pte_t pte_mkread(pte_t pte) { (pte).pte_low |= 0x004; return pte; }
static inline pte_t pte_mkexec(pte_t pte) { (pte).pte_low |= 0x004; return pte; }
static inline pte_t pte_mkdirty(pte_t pte) { (pte).pte_low |= 0x040; return pte; }
static inline pte_t pte_mkyoung(pte_t pte) { (pte).pte_low |= 0x020; return pte; }
static inline pte_t pte_mkwrite(pte_t pte) { (pte).pte_low |= 0x002; return pte; }

static inline int ptep_test_and_clear_dirty(pte_t *ptep) { return test_and_clear_bit(6, ptep); }
static inline int ptep_test_and_clear_young(pte_t *ptep) { return test_and_clear_bit(5, ptep); }
static inline void ptep_set_wrprotect(pte_t *ptep) { clear_bit(1, ptep); }
static inline void ptep_mkdirty(pte_t *ptep) { set_bit(6, ptep); }
# 309 "/home/bigbinc/kern_src/linux/include/asm/pgtable.h"
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
        pte.pte_low &= (PTE_MASK | 0x020 | 0x040);
        pte.pte_low |= pgprot_val(newprot);
        return pte;
}
# 21 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/delay.h" 1
# 10 "/home/bigbinc/kern_src/linux/include/asm/delay.h"
extern void __bad_udelay(void);

extern void __udelay(unsigned long usecs);
extern void __const_udelay(unsigned long usecs);
extern void __delay(unsigned long loops);
# 22 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/desc.h" 1



# 1 "/home/bigbinc/kern_src/linux/include/asm/ldt.h" 1
# 15 "/home/bigbinc/kern_src/linux/include/asm/ldt.h"
struct modify_ldt_ldt_s {
        unsigned int entry_number;
        unsigned long base_addr;
        unsigned int limit;
        unsigned int seg_32bit:1;
        unsigned int contents:2;
        unsigned int read_exec_only:1;
        unsigned int limit_in_pages:1;
        unsigned int seg_not_present:1;
        unsigned int useable:1;
};
# 5 "/home/bigbinc/kern_src/linux/include/asm/desc.h" 2
# 44 "/home/bigbinc/kern_src/linux/include/asm/desc.h"
struct desc_struct {
        unsigned long a,b;
};

extern struct desc_struct gdt_table[];
extern struct desc_struct *idt, *gdt;

struct Xgt_desc_struct {
        unsigned short size;
        unsigned long address __attribute__((packed));
};
# 67 "/home/bigbinc/kern_src/linux/include/asm/desc.h"
extern struct desc_struct default_ldt[];
extern void set_intr_gate(unsigned int irq, void * addr);
extern void set_ldt_desc(unsigned int n, void *addr, unsigned int size);
extern void set_tss_desc(unsigned int n, void *addr);

static inline void clear_LDT(void)
{
        int cpu = 0;
        set_ldt_desc(cpu, &default_ldt[0], 5);
        __asm__ __volatile__("lldt %%ax"::"a" ((((cpu)<<2) + (12 +1))<<3));
}




static inline void load_LDT (struct mm_struct *mm)
{
        int cpu = 0;
        void *segments = mm->context.segments;
        int count = 8192;

        if (!segments) {
                segments = &default_ldt[0];
                count = 5;
        }

        set_ldt_desc(cpu, segments, count);
        __asm__ __volatile__("lldt %%ax"::"a" ((((cpu)<<2) + (12 +1))<<3));
}
# 23 "i8259.c" 2
# 1 "/home/bigbinc/kern_src/linux/include/asm/apic.h" 1




# 1 "/home/bigbinc/kern_src/linux/include/linux/pm.h" 1
# 6 "/home/bigbinc/kern_src/linux/include/asm/apic.h" 2
# 24 "i8259.c" 2
# 38 "i8259.c"

 __attribute__((regparm(0)))
 void call_do_IRQ(void); __asm__( "\n" 
".align 16,0x90""\n" 
"common_interrupt:\n\t" "cld\n\t" 
"pushl %es\n\t" 
"pushl %ds\n\t" 
"pushl %eax\n\t" 
"pushl %ebp\n\t" 
"pushl %edi\n\t" 
"pushl %esi\n\t" 
"pushl %edx\n\t" 
"pushl %ecx\n\t" 
"pushl %ebx\n\t" 
"movl $" "0x18" ",%edx\n\t" 
"movl %edx,%ds\n\t" 
"movl %edx,%es\n\t" 

"call_do_IRQ"":\n\t" 
"call " "do_IRQ" "\n\t" 
"jmp ret_from_intr\n");



# 53 "i8259.c"


 __attribute__((regparm(0))) void IRQ0x00_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x00" "_interrupt:\n\t" "pushl $""0x00""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) 




void IRQ0x01_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x01" "_interrupt:\n\t" "pushl $""0x01""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x02_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x02" "_interrupt:\n\t" "pushl $""0x02""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x03_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x03" "_interrupt:\n\t" "pushl $""0x03""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x04_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x04" "_interrupt:\n\t" "pushl $""0x04""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x05_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x05" "_interrupt:\n\t" "pushl $""0x05""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x06_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x06" "_interrupt:\n\t" "pushl $""0x06""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x07_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x07" "_interrupt:\n\t" "pushl $""0x07""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x08_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x08" "_interrupt:\n\t" "pushl $""0x08""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x09_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x09" "_interrupt:\n\t" "pushl $""0x09""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x0a_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0a" "_interrupt:\n\t" "pushl $""0x0a""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x0b_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0b" "_interrupt:\n\t" "pushl $""0x0b""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x0c_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0c" "_interrupt:\n\t" "pushl $""0x0c""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) void IRQ0x0d_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0d" "_interrupt:\n\t" "pushl $""0x0d""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) 


void IRQ0x0e_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0e" "_interrupt:\n\t" "pushl $""0x0e""-256\n\t" "jmp common_interrupt"); __attribute__((regparm(0))) 


void IRQ0x0f_interrupt(void); __asm__( "\n"".align 16,0x90""\n" "IRQ" "0x0f" "_interrupt:\n\t" "pushl $""0x0f""-256\n\t" "jmp common_interrupt");
# 109 "i8259.c"


void (*interrupt[224])(void) = {
        IRQ0x00_interrupt, IRQ0x01_interrupt, IRQ0x02_interrupt, IRQ0x03_interrupt, IRQ0x04_interrupt, IRQ0x05_interrupt, IRQ0x06_interrupt, IRQ0x07_interrupt, IRQ0x08_interrupt, IRQ0x09_interrupt, IRQ0x0a_interrupt, IRQ0x0b_interrupt, IRQ0x0c_interrupt, IRQ0x0d_interrupt, IRQ0x0e_interrupt, IRQ0x0f_interrupt,







};
# 132 "i8259.c"
spinlock_t i8259A_lock = (spinlock_t) { };

static void end_8259A_irq (unsigned int irq)
{
        if (!(irq_desc[irq].status & (2|1)))
                enable_8259A_irq(irq);
}



void mask_and_ack_8259A(unsigned int);

static unsigned int startup_8259A_irq(unsigned int irq)
{
        enable_8259A_irq(irq);
        return 0;
}

static struct hw_interrupt_type i8259A_irq_type = {
        "XT-PIC",
        startup_8259A_irq,
        disable_8259A_irq,
        enable_8259A_irq,
        disable_8259A_irq,
        mask_and_ack_8259A,
        end_8259A_irq,
        ((void *)0)
};
# 168 "i8259.c"
static unsigned int cached_irq_mask = 0xffff;
# 183 "i8259.c"
unsigned long io_apic_irqs;

void disable_8259A_irq(unsigned int irq)
{
        unsigned int mask = 1 << irq;
        unsigned long flags;

        do { __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (flags): :"memory"); (void)(&i8259A_lock); } while (0);
        cached_irq_mask |= mask;
        if (irq & 8)
                outb(((((unsigned char *)&(cached_irq_mask))[1])),0xA1);
        else
                outb(((((unsigned char *)&(cached_irq_mask))[0])),0x21);
        do { do { } while(0); __asm__ __volatile__("pushl %0 ; popfl": :"g" (flags):"memory", "cc"); } while (0);
}

void enable_8259A_irq(unsigned int irq)
{
        unsigned int mask = ~(1 << irq);
        unsigned long flags;

        do { __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (flags): :"memory"); (void)(&i8259A_lock); } while (0);
        cached_irq_mask &= mask;
        if (irq & 8)
                outb(((((unsigned char *)&(cached_irq_mask))[1])),0xA1);
        else
                outb(((((unsigned char *)&(cached_irq_mask))[0])),0x21);
        do { do { } while(0); __asm__ __volatile__("pushl %0 ; popfl": :"g" (flags):"memory", "cc"); } while (0);
}

int i8259A_irq_pending(unsigned int irq)
{
        unsigned int mask = 1<<irq;
        unsigned long flags;
        int ret;

        do { __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (flags): :"memory"); (void)(&i8259A_lock); } while (0);
        if (irq < 8)
                ret = inb(0x20) & mask;
        else
                ret = inb(0xA0) & (mask >> 8);
        do { do { } while(0); __asm__ __volatile__("pushl %0 ; popfl": :"g" (flags):"memory", "cc"); } while (0);

        return ret;
}

void make_8259A_irq(unsigned int irq)
{
        disable_irq_nosync(irq);
        io_apic_irqs &= ~(1<<irq);
        irq_desc[irq].handler = &i8259A_irq_type;
        enable_irq(irq);
}







static inline int i8259A_irq_real(unsigned int irq)
{
        int value;
        int irqmask = 1<<irq;

        if (irq < 8) {
                outb(0x0B,0x20);
                value = inb(0x20) & irqmask;
                outb(0x0A,0x20);
                return value;
        }
        outb(0x0B,0xA0);
        value = inb(0xA0) & (irqmask >> 8);
        outb(0x0A,0xA0);
        return value;
}







void mask_and_ack_8259A(unsigned int irq)
{
        unsigned int irqmask = 1 << irq;
        unsigned long flags;

        do { __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (flags): :"memory"); (void)(&i8259A_lock); } while (0);
# 287 "i8259.c"
        if (cached_irq_mask & irqmask)
                goto spurious_8259A_irq;
        cached_irq_mask |= irqmask;

handle_real_irq:
        if (irq & 8) {
                inb(0xA1);
                outb(((((unsigned char *)&(cached_irq_mask))[1])),0xA1);
                outb(0x60+(irq&7),0xA0);
                outb(0x62,0x20);
        } else {
                inb(0x21);
                outb(((((unsigned char *)&(cached_irq_mask))[0])),0x21);
                outb(0x60+irq,0x20);
        }
        do { do { } while(0); __asm__ __volatile__("pushl %0 ; popfl": :"g" (flags):"memory", "cc"); } while (0);
        return;

spurious_8259A_irq:



        if (i8259A_irq_real(irq))




                goto handle_real_irq;

        {
                static int spurious_irq_mask;




                if (!(spurious_irq_mask & irqmask)) {
                        printk("spurious 8259A interrupt: IRQ%d.\n", irq);
                        spurious_irq_mask |= irqmask;
                }
                atomic_inc(&irq_err_count);





                goto handle_real_irq;
        }
}

void __attribute__ ((__section__ (".text.init"))) init_8259A(int auto_eoi)
{
        unsigned long flags;

        do { __asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (flags): :"memory"); (void)(&i8259A_lock); } while (0);

        outb(0xff, 0x21);
        outb(0xff, 0xA1);




        outb_p(0x11, 0x20);
        outb_p(0x20 + 0, 0x21);
        outb_p(0x04, 0x21);

        outb_p(0x01, 0x21);

        outb_p(0x11, 0xA0);
        outb_p(0x20 + 8, 0xA1);
        outb_p(0x02, 0xA1);
        outb_p(0x01, 0xA1);


        i8259A_irq_type.ack = mask_and_ack_8259A;

        (__builtin_constant_p(100) ? ((100) > 20000 ? __bad_udelay() : __const_udelay((100) * 0x10c6ul)) : __udelay(100));

        outb(((((unsigned char *)&(cached_irq_mask))[0])), 0x21);
        outb(((((unsigned char *)&(cached_irq_mask))[1])), 0xA1);

        do { do { } while(0); __asm__ __volatile__("pushl %0 ; popfl": :"g" (flags):"memory", "cc"); } while (0);
}
# 382 "i8259.c"
static void math_error_irq(int cpl, void *dev_id, struct pt_regs *regs)
{
        extern void math_error(void *);
        outb(0,0xF0);
        if (ignore_irq13 || !boot_cpu_data.hard_math)
                return;
        math_error((void *)regs->eip);
}





static struct irqaction irq13 = { math_error_irq, 0, 0, "fpu", ((void *)0), ((void *)0) };






static struct irqaction irq2 = { no_action, 0, 0, "cascade", ((void *)0), ((void *)0)};



void __attribute__ ((__section__ (".text.init"))) init_ISA_irqs (void)
{
        int i;


        init_8259A(0);

        for (i = 0; i < 224; i++) {
                irq_desc[i].status = 2;
                irq_desc[i].action = 0;
                irq_desc[i].depth = 1;

                if (i < 16) {



                        irq_desc[i].handler = &i8259A_irq_type;
                } else {



                        irq_desc[i].handler = &no_irq_type;
                }
        }
}

void __attribute__ ((__section__ (".text.init"))) init_IRQ(void)
{
        int i;

        init_ISA_irqs();







        for (i = 0; i < 224; i++) {
                int vector = 0x20 + i;
                if (vector != 0x80)
                        set_intr_gate(vector, interrupt[i]);
        }






        outb_p(0x34,0x43);
        outb_p(((1193180 + 100/2) / 100) & 0xff , 0x40);
        outb(((1193180 + 100/2) / 100) >> 8 , 0x40);







        if (boot_cpu_data.hard_math && !((__builtin_constant_p((0*32+ 0)) ? constant_test_bit(((0*32+ 0)),(boot_cpu_data.x86_capability)) : variable_test_bit(((0*32+ 0)),(boot_cpu_data.x86_capability)))))
                setup_irq(13, &irq13);
}
