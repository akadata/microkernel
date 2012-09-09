typedef struct context Context;

struct task {
    Node node;
    List messages;
    Context *context;
    char *name;
};

extern Task *running_task;
extern List ready_tasks;

/* context_create: Allocate and initialize a task context. The
task entry point is entry. The function temporary disables
multitasking. A pointer to the created context is returned,
or NULL if an error occured. */
Context *context_create(Function *entry);

/* interrupts_enable: Enable interrupts. */
void interrupts_enable(void);

/* interrupts_disable: Disable interrupts. */
void interrupts_disable(void);

/* port_reschedule: Ensure the highest priority task is
running. */
void port_reschedule(void);

