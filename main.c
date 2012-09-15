/* main.c: Example program for kernel. */
#include "kernel.h"
#include "kernel_log.h"
#include "kernel_port.h"

void swisch(void)
{
    interrupts_disable();
    list_remove_node((Node *) task_self());
    list_enqueue(&ready_tasks, (Node *) task_self());
    port_reschedule();
}

void count(uint16_t n) {
    uint8_t i;
    volatile uint16_t t;

    log_line(task_self()->name);
    log_string(" entered counter function.");
    i = 0;
    while (1) {
        if (0 == (i % n)) {
            swisch();
            task_line("");
        }
        port_log_puthex(i++);
        port_log_putchar(' ');
        while(t++) {
            t--; t++; t--; t++; t--; t++;
        }
    }
}

void hello(void)
{
    task_self()->sig_mask = 1;
    count(7);
}

Task *my;

int main(void)
{
    kernel_init();
    my = task_create("hello", PRIORITY_NORMAL, hello, 64);
    kernel_start();
    swisch();
    task_signal(my, 1);
        count(11);
    return 0;
}

