/* main.c: Example program for kernel. */
#include "kernel.h"
#include "log.h"

void hello(void)
{
    uint8_t i;
    volatile uint16_t t;

    log_line("Hello, world!");
    i = 0;
    while (1) {
        if (0 == (i % 16)) {
            log_line("");
        }
        uart_puthex(i++);
        uart_putchar(' ');
        while(t++) {
            t--; t++; t--; t++; t--; t++;
        }
    }
}

int main(void)
{
    kernel_init();
    task_create("hello", PRIORITY_NORMAL, hello, 64);
    kernel_start();
    return 0;
}

