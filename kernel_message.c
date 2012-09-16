#include "kernel.h"
#include "kernel_port.h"
/* #include "kernel_log.h" */

#define SIGNAL_MESSAGE ((Signal) 1)

void message_set_data(Message *message, void *data)
{
    message->data = data;
}

void *message_get_data(Message *message)
{
    return message->data;
}

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

