#ifndef SCHED_H
#define SCHED_H

#include "process.h"

extern thread_t *current_thread;

extern void		schedule();
extern void		sched_ready(thread_t *thread);
extern void		sched_kill(thread_t *thread);

#endif /* ndef SCHED_H */
