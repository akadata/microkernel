#include <stdlib.h>
#include "kernel.h"
#include "kernel_port.h"
#include "kernel_log.h"

#define IDLE_TASK_STACKSIZE ((size_t) 64)

struct message {
    Node node;
    Task *source;
    void *data;
};

Task *running_task;
List ready_tasks;
/* The waiting list is not strictly needed by the multitasking
logic. */
static List waiting_tasks;

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
    log_line("OK");
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
    log_line("Added init task.");

    /* Initialize timer. */
    /* "Remove a task from ready_tasks and start it." */
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

void task_signal(Task *task, Signal signal) {
    /* Set destionation signal. Unblock the task if the target
    signal is in mask. */
    interrupts_disable();
    task_line("");
    log_hex(signal);
    log_string(" --> ");
    log_string(task->name);
    log_string(" {mask=");
    log_hex(task->sig_mask);
    log_string(", rec=");
    log_hex(task->sig_rec);
    log_string("} ==> rec=");

    task->sig_rec |= signal;
    log_hex(task->sig_rec);
    if (task->sig_mask & signal) {
        task_line("Unblock destination");
        list_remove_node((Node *) task); 
        list_enqueue(&ready_tasks, (Node *) task);
        if (running_task->node.ln_pri < task->node.ln_pri) {
            log_line("Force reschedule");
            port_reschedule();
        } else {
            /* An unneeded context switch is optimized away. */
            interrupts_enable();
        }
    } else {
        interrupts_enable();
    }
}

/*
sig_wait := mask
if 0 != (sig_wait & sig_rec)
  ret = sig_rec
  sig_rec = sig_wait & ~sig_rec
  return sig_rec
else
  block
  ret = sig_rec
  clear signal bits that satisfied wait conditions. ??
  sig_rec = sig_wait & ~sig_rec
  return ret
*/
/* FIXME: Motivate why satisfied signal bits are cleared. */
Signal task_wait(Signal mask) {
    
    return 0;
}

