/* main.c: Example program for kernel. */
#include "kernel.h"

void t1_entry(void)
{
    /* Message *m = message_create(); */
    while (1);
}

int main(void)
{
    kernel_init();
    task_create("my_task", PRIORITY_NORMAL, t1_entry, 64);

    kernel_start();
    return 0;
}

