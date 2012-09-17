#include "kernel.h"
#include "kernel_port.h"

#define SIGNAL_MESSAGE ((Signal) 1)
#define SIGNAL_REPLY ((Signal) 2)

/* Asynchronous message passing. */
void message_put(Task *destination, Message *message)
{
    message->node.ln_pri = PRIORITY_NORMAL;
    message->source = running_task;
    interrupts_disable();
    list_add_end(&destination->messages, (Node *) message);
    interrupts_enable();
    task_signal(destination, SIGNAL_MESSAGE);
}

void message_wait(void)
{
    task_wait(SIGNAL_MESSAGE);
}

Message *message_get(void)
{
    Message *m;

    interrupts_disable();
    m = (Message *) list_remove_head(&running_task->messages);
    interrupts_enable();
    return m;
}

/* Synchronous message passing. */
void message_putget(Task *destination, Message *message)
{
    message_put(destination, message);
    task_wait(SIGNAL_REPLY);  
}

void message_reply(Message *message)
{
    task_signal(message->source, SIGNAL_REPLY);
}

