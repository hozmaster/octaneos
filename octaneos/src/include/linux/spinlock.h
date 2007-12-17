#ifndef __OCTANE_SPINLOCK_H
#define __OCTANE_SPINLOCK_H

typedef struct { } spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { }

#define spin_lock_init(lock)	do { } while(0)
#define spin_lock(lock)		(void)(lock) /* Not "unused variable". */
#define spin_trylock(lock)	(1)
#define spin_unlock_wait(lock)	do { } while(0)
#define spin_unlock(lock)	do { } while(0)
#define spin_lock_irq(lock)	cli()
#define spin_unlock_irq(lock)	sti()

#define spin_lock_irqsave(lock, flags)      \
	do { save_flags(flags); cli(); } while (0)

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 */
typedef struct { } rwlock_t;
#define RW_LOCK_UNLOCKED (rwlock_t) { }

#define read_lock(lock)		(void)(lock) /* Not "unused variable." */
#define read_unlock(lock)	do { } while(0)
#define write_lock(lock)	(void)(lock) /* Not "unused variable." */
#define write_unlock(lock)	do { } while(0)
#define read_lock_irq(lock)	cli()
#define read_unlock_irq(lock)	sti()
#define write_lock_irq(lock)	cli()
#define write_unlock_irq(lock)	sti()

#define read_lock_irqsave(lock, flags)	\
	do { save_flags(flags); cli(); } while (0)
#define read_unlock_irqrestore(lock, flags) \
	restore_flags(flags)
#define write_lock_irqsave(lock, flags)	\
	do { save_flags(flags); cli(); } while (0)
#define write_unlock_irqrestore(lock, flags) \
	restore_flags(flags)

#endif
