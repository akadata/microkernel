/* This file contains declarations needed for porting. */

typedef struct context Context;

struct task {
    Node node;
    Context *context;
    char *name;
    List messages;
    Signal sig_mask;
    Signal sig_rec;
};

extern Task *running_task;
extern List ready_tasks;
extern List waiting_tasks;

/* context_create: Allocate stack and initialize task
context. The code entry point is entry. A pointer to the
created context is returned, or NULL if an error occured. */
Context *context_create(Function *entry, size_t stacksize);

/* interrupts_enable: Enable interrupts. */
void interrupts_enable(void);

/* interrupts_disable: Disable interrupts. */
void interrupts_disable(void);

/* Set up round robin timer. */
void port_timer_init(void);

/* port_reschedule: Ensure the highest priority task is
running. */
void port_reschedule(void);

