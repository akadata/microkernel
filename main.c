/* main.c: Example program for kernel. */
#include <assert.h>
#include <stdlib.h>
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

Task *t1, *t2;
#define MESS 3
void f1(void)
{
    Message m[MESS];
    for (uint8_t i = 0; i < MESS; i++) {
        interrupts_disable();
        task_line("Sending message ");
        log_hex(i);
        interrupts_enable();
        message_put(t2, &m[i]);
    }

    interrupts_disable();
    task_line("END");
    interrupts_enable();
    task_wait(2);
}

void f2(void)
{
    Message *n;
    while(1) {
        message_wait();
        while(NULL != (n = message_get())) {
            interrupts_disable();
            task_line("Got message");
            interrupts_enable();
        }
    }

    interrupts_disable();
    task_line("END");
    interrupts_enable();
    task_wait(2);
}


void abort(void)
{
    log_line("ASSERT FAILURE");
    exit(1);
}

int main(void)
{
    kernel_init();
    t1 = task_create("Task1", PRIORITY_NORMAL+2, f1, 64);
    assert(t1);
    t2 = task_create("Task2", PRIORITY_NORMAL+1, f2, 64);
    assert(t2);
    assert(kernel_start());
    task_wait(2);
    return 0;
}

void yield(void)
{
    interrupts_disable();
    list_remove_node((Node *) task_self());
    list_enqueue(&ready_tasks, (Node *) task_self());
    port_reschedule();
}

