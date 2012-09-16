/* main.c: Example program for kernel. */
#include "kernel.h"
#include "kernel_log.h"
#include "kernel_port.h"

void count(uint16_t n) {
    uint8_t i;
    volatile uint16_t t;

    log_line(task_self()->name);
    log_string(" entered counter function.");
    i = 0;
    while (1) {
        if (0 == (i % n)) {
            task_line("");
        }
        port_log_puthex(i++);
        port_log_putchar(' ');
        while(t++) {
            t--; t++; t--; t++; t--; t++; t--; t++; t--; t++; t--; t++;
        }
    }
}

void hello(void)
{
    task_wait(1);
    task_wait(2);
    task_wait(8+16);
    task_wait(32);
}

Task *my;

int main(void)
{
    kernel_init();
    my = task_create("hello", PRIORITY_NORMAL, hello, 64);
    kernel_start();
    task_signal(my, 4);
    task_signal(my, 2);
    task_signal(my, 8);
    task_signal(my, 1);
    task_wait(4);
    return 0;
}

void yield(void)
{
    interrupts_disable();
    list_remove_node((Node *) task_self());
    list_enqueue(&ready_tasks, (Node *) task_self());
    port_reschedule();
}

