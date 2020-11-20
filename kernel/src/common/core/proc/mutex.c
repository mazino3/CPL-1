#include <common/core/proc/mutex.h>
#include <common/core/proc/proc.h>
#include <common/core/proc/proclayout.h>
#include <common/lib/kmsg.h>
#include <hal/proc/intlock.h>

void mutex_init(struct mutex *mutex) {
	mutex->queue_head = mutex->queue_tail = NULL;
	mutex->locked = false;
}

void mutex_lock(struct mutex *mutex) {
	if (!proc_is_initialized()) {
		return;
	}
	hal_intlock_lock();
	struct proc_process *process = proc_get_data(proc_my_id());
	if (process == NULL) {
		kmsg_err("Mutex Manager", "Failed to get current process data");
	}
	if (!(mutex->locked)) {
		mutex->locked = true;
		hal_intlock_unlock();
		return;
	}
	if (mutex->queue_head == NULL) {
		mutex->queue_head = mutex->queue_tail = process;
	} else {
		mutex->queue_tail->next = process;
	}
	process->next_in_queue = NULL;
	proc_pause_self(true);
}

void mutex_unlock(struct mutex *mutex) {
	if (!proc_is_initialized()) {
		return;
	}
	hal_intlock_lock();
	if (mutex->queue_head == NULL) {
		mutex->locked = false;
		hal_intlock_unlock();
		return;
	} else if (mutex->queue_head == mutex->queue_tail) {
		struct proc_process *process = mutex->queue_head;
		mutex->queue_head = mutex->queue_tail = NULL;
		hal_intlock_unlock();
		proc_continue(process->pid);
	} else {
		struct proc_process *process = mutex->queue_head;
		mutex->queue_head = mutex->queue_head->next;
		hal_intlock_unlock();
		proc_continue(process->pid);
	}
}

bool mutex_is_queued(struct mutex *mutex) {
	if (!proc_is_initialized()) {
		return false;
	}
	hal_intlock_lock();
	bool result = mutex->queue_head != NULL;
	hal_intlock_unlock();
	return result;
}

bool mutex_is_locked(struct mutex *mutex) {
	if (!proc_is_initialized()) {
		return false;
	}
	hal_intlock_lock();
	bool result = mutex->locked;
	hal_intlock_unlock();
	return result;
}