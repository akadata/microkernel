#include <stdlib.h>
#include "kernel.h"
#include "kernel_port.h"
#include "kernel_log.h"

#define IDLE_TASK_STACKSIZE ((size_t) 64)

Task *running_task;
List ready_tasks;
/* The waiting list is not strictly needed by the multitasking
logic. */
List waiting_tasks;

/* The IDLE task is always guaranteed to be in the ready
list. It's program code is in idle_function. */
static void idle_function(void)
{
    while (1);
}

uint8_t kernel_init(void)
{
    Task *idle;

    log_init();
    list_init(&ready_tasks);
    list_init(&waiting_tasks);

    idle = task_create("idle", PRIORITY_IDLE,
      idle_function, IDLE_TASK_STACKSIZE);
    if (NULL == idle) {
        return 1;
        /* 1 is a magic number. */
    }
    return 0;
}

Task *kernel_start(void)
{
    Task *init;

    init = calloc(1, sizeof (Task));
    if (NULL == init) {
        return NULL;
    }
    list_init(&(init->messages));
    init->node.ln_pri = PRIORITY_NORMAL;
    init->name = "init";
 
    list_enqueue(&ready_tasks, (Node *) init);
    running_task = init;

    /* Initialize timer. */
    port_timer_init();
    port_reschedule();
    return init;
}

Task *task_create(char *name, Priority priority, Function *entry,
  size_t stacksize)
{
    Task *task;
    Context *context;

    task = calloc(1, sizeof (Task));
    if (NULL == task) {
        return NULL;
    }
    context = context_create(entry, stacksize);
    if (NULL == context) {
        free(task);
        return NULL;
    }
    task->context = context;

    list_init(&(task->messages));
    task->node.ln_pri = priority;
    task->name = name;
 
    list_enqueue(&ready_tasks, (Node *) task);
    log_line(name);
    return task;
}

Task *task_self(void) {
    return running_task;
}

