#include "list.h"
#include "kernel.h"

#define PRIORITY_IDLE (INT8_MIN)
#define PRIORITY_LOW (INT8_MIN / 2)
#define PRIORITY_NORMAL (0)
#define PRIORITY_HIGH (INT8_MAX / 2)
#define IDLE_TASK_STACKSIZE ((size_t) 64)

struct message {
    Node node;
    Task *source;
    void *data;
};

struct task {
    Node node;
    List messages;
};

static Task *running_task;
static List ready_tasks;
/* It is not yet known if the waiting_list is needed. */
static List waiting_tasks;

/* The IDLE task is always guaranteed to be in the ready
list. It's program code is in idle_function. */
static void idle_function(void)
{
    while (1);
}

uint8_t kernel_init(void)
{
    if (NULL == task_create(idle_function, IDLE_TASK_STACKSIZE,
      PRIORITY_IDLE)) {
        return 1;
        /* 1 is a magic number. */
    }
    list_init(&ready_tasks);
    list_init(&waiting_tasks);
    /* Initialize timer. */
    return 0;
}

void kernel_start(void)
{
    /* "Dequeue a task from ready_tasks and start it." */
}

