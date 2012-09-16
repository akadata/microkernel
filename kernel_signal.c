#include "kernel.h"
#include "kernel_port.h"
#include "kernel_log.h"

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
        log_string(", unblock, prio:");
        list_remove_node((Node *) task); 
        list_enqueue(&ready_tasks, (Node *) task);
        if (running_task->node.ln_pri < task->node.ln_pri) {
            log_string("hi");
            port_reschedule();
        } else {
            /* An unneeded context switch is optimized away. */
            log_string("lo");
            interrupts_enable();
        }
    } else {
        log_string(", no unblock.");
        interrupts_enable();
    }
}

/* Why are satisfied signal bits cleared? Because there is no
need to clear it manually afterwards then. */
Signal task_wait(Signal mask) {
    Signal ret;

    interrupts_disable();

    task_line("{mask=");
    log_hex(mask);
    log_string(", rec=");
    log_hex(running_task->sig_rec);
    log_string("} ==> ");

    if (!(mask & running_task->sig_rec)) {
        running_task->sig_mask = mask;
        list_remove_node((Node *) running_task);
        list_enqueue(&waiting_tasks, (Node *) running_task);

        log_string("blocked...");
        port_reschedule();
        /* Interrupts may occur here. */
        interrupts_disable();

        task_line("...unblocked ");
        log_string("{mask=");
        log_hex(mask);
        log_string(", rec=");
        log_hex(running_task->sig_rec);
        log_string("} ==> ");
    }
    ret = running_task->sig_rec & mask;
    running_task->sig_rec &= ~mask;
    running_task->sig_mask = 0;

    log_string("{rec=");
    log_hex(running_task->sig_rec);
    log_string("}, return ");
    log_hex(ret);

    interrupts_enable();
    return ret;
}
