#include <stdlib.h>
#include "kernel.h"
#include "kernel_port.h"

#define IDLE_TASK_STACKSIZE ((size_t) 64)

struct message {
    Node node;
    Task *source;
    void *data;
};

struct task {
    Node node;
    List messages;
    Context *context;
    char *name;
    uint16_t npreemptions;
};

static Task *running_task;
static List ready_tasks;

/* The IDLE task is always guaranteed to be in the ready
list. It's program code is in idle_function. */
static void idle_function(void)
{
    while (1);
}

uint8_t kernel_init(void)
{
    Task *idle;

    list_init(&ready_tasks);
    idle = task_create("idle", IDLE_TASK_STACKSIZE,
      idle_function, PRIORITY_IDLE);
    if (NULL == idle) {
        return 1;
        /* 1 is a magic number. */
    }
    return 0;
}

void kernel_start(void)
{
    /* Initialize timer. */
    /* "Remove a task from ready_tasks and start it." */
}

Task *task_create(char *name, Priority priority, Function *entry,
  size_t stacksize)
{
    Task *task;
    Context *context;

    interrupts_disable();
    task = malloc(stacksize + sizeof (Task));
    interrupts_enable();
    if (NULL == task) {
        return NULL;
    }
    context = context_create(entry);
    if (NULL == context) {
        return NULL;
    }
    task->context = context;

    list_init(&(task->messages));
    task->node.ln_pri = priority;
    task->name = name;
    task->npreemptions = 0;
 
    interrupts_disable();
    list_enqueue(&ready_tasks, (Node *) task);
    interrupts_enable();
    return task;
}

Task *task_self(void) {
    return running_task;
}

