/* This is a test program for a polling uart driver. */
#include <assert.h>
#include <stdlib.h>
#include "kernel.h"
#include "kernel_log.h"
#include "kernel_port.h"
#include "uart_driver.h"

static void f(void)
{
    uart_putstring("\nThis is ");
    uart_putstring(task_self()->name);
    message_wait();
}

int main(void)
{
    Task *t;

    kernel_init();
    uart_open();
    t = task_create("task 1", PRIORITY_NORMAL + 1, f, 100);
    assert(t);
    t = task_create("task 2", PRIORITY_NORMAL, f, 100);
    assert(t);
    assert(kernel_start());
    f();
    task_wait(2);
    return 0;
}
