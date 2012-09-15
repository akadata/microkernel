/* main.c: Example program for kernel. */
#include "kernel.h"
#include "log.h"
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

    log_line("I am ");
    log_string(task_self()->name);
    i = 0;
    while (1) {
        if (0 == (i % n)) {
            swisch();
            log_line("");
        }
        uart_puthex(i++);
        uart_putchar(' ');
        while(t++) {
            t--; t++; t--; t++; t--; t++;
        }
    }
}

void hello(void)
{
    count(3);
}

int main(void)
{
    kernel_init();
    task_create("hello", PRIORITY_NORMAL, hello, 64);
    kernel_start();
    while(1) {
        count(5);
    }
    return 0;
}

