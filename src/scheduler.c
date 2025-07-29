#include "scheduler.h"
#include "memory.h"

#define MAX_TASKS 16

struct task { void (*entry)(void); };

static struct task tasks[MAX_TASKS];
static int task_count = 0;
static int current = 0;

void scheduler_init(void) { task_count = 0; current = 0; }

void scheduler_add(void (*entry)(void)) {
    if (task_count < MAX_TASKS) {
        tasks[task_count++].entry = entry;
    }
}

void scheduler_schedule(void) {
    if (task_count == 0) return;
    current = (current + 1) % task_count;
    tasks[current].entry();
}
