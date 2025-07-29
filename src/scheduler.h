#ifndef SCHEDULER_H
#define SCHEDULER_H

void scheduler_init(void);
void scheduler_add(void (*entry)(void));
void scheduler_schedule(void);

#endif
