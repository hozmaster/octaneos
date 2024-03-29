
#include <system/system.h>
#include <linux/errno.h>
#include <linux/tasks.h>
#include <linux/page.h>
#include <linux/sched.h>
#include <linux/ldt.h>
#include <system/filesystem.h>
#include <linux/unistd.h>
#include <linux/mm.h>

#include <linux/string.h>

asmlinkage void ret_from_sys_call(void) __asm__("ret_from_sys_call");

/* These should maybe be in <linux/tasks.h> */

#define MAX_TASKS_PER_USER (NR_TASKS/2)
#define MIN_TASKS_LEFT_FOR_ROOT 4

extern int shm_fork(struct task_struct *, struct task_struct *);
long last_pid=0;

static int find_empty_process(void) {

	int free_task;
	int i, tasks_free;
	int this_user_tasks;

repeat:
	if ((++last_pid) & 0xffff8000)
		last_pid=1;
	this_user_tasks = 0;
	tasks_free = 0;
	free_task = -EAGAIN;
	i = NR_TASKS;
	while (--i > 0) {
		if (!task[i]) {
			free_task = i;
			tasks_free++;
			continue;
		}
		if (task[i]->uid == current->uid)
			this_user_tasks++;
		if (task[i]->pid == last_pid || task[i]->pgrp == last_pid ||
		    task[i]->session == last_pid)
			goto repeat;
	}
	if (tasks_free <= MIN_TASKS_LEFT_FOR_ROOT ||
	    this_user_tasks > MAX_TASKS_PER_USER)
		if (current->uid)
			return -EAGAIN;
	return free_task;
}

static struct file *copy_fd(struct file * old_file) {

	struct file *new_file = get_empty_filp();
	int error;

	if (new_file) {
		//memcpy(new_file,old_file,sizeof(struct file));
		new_file->f_count = 1;
		if (new_file->f_inode)
			new_file->f_inode->i_count++;

		if (new_file->f_op && new_file->f_op->open) {
			error = new_file->f_op->open(new_file->f_inode,new_file);
			if (error) {
				iput(new_file->f_inode);
				new_file->f_count = 0;
				new_file = NULL;
			}
		}
	}
	return new_file;
}

static int dup_mmap(struct task_struct * tsk)
{
	struct vm_area_struct * mpnt, **p, *tmp;

	tsk->mm->mmap = NULL;
	p = &tsk->mm->mmap;
	for (mpnt = current->mm->mmap ; mpnt ; mpnt = mpnt->vm_next) {

		//tmp = (struct vm_area_struct *) kmalloc(sizeof(struct vm_area_struct), GFP_KERNEL);
		if (!tmp)
			return -ENOMEM;
		*tmp = *mpnt;
		tmp->vm_task = tsk;
		tmp->vm_next = NULL;
		if (tmp->vm_inode)
			tmp->vm_inode->i_count++;
		*p = tmp;
		p = &tmp->vm_next;
	}
	return 0;
}

/*
 * SHAREFD not yet implemented..
 */
static void copy_files(unsigned long clone_flags, struct task_struct * p)
{
	int i;
	struct file * f;

	if (clone_flags & COPYFD) {
		for (i=0; i<NR_OPEN;i++)
			if ((f = p->files->fd[i]) != NULL)
				p->files->fd[i] = copy_fd(f);
	} else {
		for (i=0; i<NR_OPEN;i++)
			if ((f = p->files->fd[i]) != NULL)
				f->f_count++;
	}
}

/*
 * CLONEVM not yet correctly implemented: needs to clone the mmap
 * instead of duplicating it..
 */
static int copy_mm(unsigned long clone_flags, struct task_struct * p)
{
	if (clone_flags & COPYVM) {
		p->mm->swappable = 1;
		p->mm->min_flt = p->mm->maj_flt = 0;
		p->mm->cmin_flt = p->mm->cmaj_flt = 0;
		if (copy_page_tables(p))
			return 1;
		dup_mmap(p);
	} else {
		if (clone_page_tables(p))
			return 1;
		dup_mmap(p);		/* wrong.. */
	}
	//return shm_fork(current, p);
	return 0;
}

static void copy_fs(unsigned long clone_flags, struct task_struct * p)
{
	if (current->fs->pwd)
		current->fs->pwd->i_count++;
	if (current->fs->root)
		current->fs->root->i_count++;
	if (current->executable)
		current->executable->i_count++;
}

#define IS_CLONE (regs.orig_eax == __NR_clone)

/*
 *  Ok, this is the main fork-routine. It copies the system process
 * information (task[nr]) and sets up the necessary registers. It
 * also copies the data segment in its entirety.
 */
asmlinkage int sys_fork(struct pt_regs regs) {
	// TODO: fix
#if 0
	struct pt_regs *childregs;
	struct task_struct *p;
	int i,nr;
	unsigned long clone_flags = COPYVM | SIGCHLD;

	//if(!(p = (struct task_struct*)__get_free_page(GFP_KERNEL)))
	//	goto bad_fork;

	nr = find_empty_process();
	if (nr < 0)
		goto bad_fork_free;
	task[nr] = p;
	*p = *current;

	if (p->exec_domain && p->exec_domain->use_count)
		(*p->exec_domain->use_count)++;

	// TODO: 
	//if (p->binfmt && p->binfmt->use_count)
	//	(*p->binfmt->use_count)++;

	p->did_exec = 0;
	p->swapping = 0;
	p->kernel_stack_page = 0;
	p->state = TASK_UNINTERRUPTIBLE;
	p->flags &= ~(PF_PTRACED|PF_TRACESYS);
	p->pid = last_pid;
	p->p_pptr = p->p_opptr = current;
	p->p_cptr = NULL;

	//SET_LINKS(p);

	p->signal = 0;
	p->it_real_value = p->it_virt_value = p->it_prof_value = 0;
	p->it_real_incr = p->it_virt_incr = p->it_prof_incr = 0;
	p->leader = 0;		/* process leadership doesn't inherit */
	p->utime = p->stime = 0;
	p->cutime = p->cstime = 0;
	p->start_time = jiffies;
/*
 * set up new TSS and kernel stack
 */
	//if (!(p->kernel_stack_page = get_free_page(GFP_KERNEL)))
	//	goto bad_fork_cleanup;

	*(unsigned long *)p->kernel_stack_page = STACK_MAGIC;
	p->tss.es = KERNEL_DS;
	p->tss.cs = KERNEL_CS;
	p->tss.ss = KERNEL_DS;
	p->tss.ds = KERNEL_DS;
	p->tss.fs = USER_DS;
	p->tss.gs = KERNEL_DS;
	p->tss.ss0 = KERNEL_DS;
	p->tss.esp0 = p->kernel_stack_page + PAGE_SIZE;
	p->tss.tr = _TSS(nr);

	childregs = ((struct pt_regs *) (p->kernel_stack_page + PAGE_SIZE)) - 1;
	p->tss.esp = (unsigned long) childregs;
	//p->tss.eip = (unsigned long) ret_from_sys_call;
	*childregs = regs;
	childregs->eax = 0;
	p->tss.back_link = 0;

	p->tss.eflags = regs.eflags & 0xffffcfff;	/* iopl is always 0 for a new process */
	if (IS_CLONE) {
		if (regs.ebx)
			childregs->esp = regs.ebx;
		clone_flags = regs.ecx;
		if (childregs->esp == regs.esp)
			clone_flags |= COPYVM;
	}
	p->exit_signal = clone_flags & CSIGNAL;
	p->tss.ldt = _LDT(nr);

	//if (p->ldt) {
		//p->ldt = (struct desc_struct*) vmalloc(LDT_ENTRIES*LDT_ENTRY_SIZE);

		// TODO:
		//if (p->ldt != NULL)
		//	memcpy(p->ldt, current->ldt, LDT_ENTRIES*LDT_ENTRY_SIZE);
	//}

	// TODO:
	//p->tss.bitmap = offsetof(struct tss_struct, io_bitmap);
	for (i = 0; i < IO_BITMAP_SIZE+1 ; i++) /* IO bitmap is actually SIZE+1 */
		p->tss.io_bitmap[i] = ~0;

	if (last_task_used_math == current)
		__asm__("clts ; fnsave %0 ; frstor %0":"=m" (p->tss.i387));

	//if (copy_mm(clone_flags, p))
	//	goto bad_fork_cleanup;

	//copy_files(clone_flags, p);
	//copy_fs(clone_flags, p);

	//set_tss_desc(gdt+(nr<<1)+FIRST_TSS_ENTRY,&(p->tss));
	//if (p->ldt)
	//	set_ldt_desc(gdt+(nr<<1)+FIRST_LDT_ENTRY,p->ldt, 512);
	//else
	//	set_ldt_desc(gdt+(nr<<1)+FIRST_LDT_ENTRY,&default_ldt, 1);

	p->counter = current->counter >> 1;
	p->state = TASK_RUNNING;	/* do this last, just in case */
	return p->pid;

bad_fork_cleanup:
	task[nr] = NULL;
	//REMOVE_LINKS(p);

	//free_page(p->kernel_stack_page);
bad_fork_free:
	//free_page((long) p);
bad_fork:
	return -EAGAIN;
#endif
	return 0;
}
