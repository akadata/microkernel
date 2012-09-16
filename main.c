/* This is a test program for a polling uart driver. */
#include <assert.h>
#include <stdlib.h>
#include "kernel.h"
#include "kernel_log.h"
#include "kernel_port.h"
#include "uart_driver.h"

Task *t1, *t2;

typedef struct {
    Message m;
    uint8_t d;
} Byte_Message;

void f1(void)
{
    Byte_Message m;
    Message *reply;

    while (1) {
        uart_putstring("\nf1: Enter a byte message: ");
        m.d = uart_get();
        uart_putstring("\nf1: Got byte: ");
        uart_put(m.d);
        message_put(t2, (Message *) &m);
        uart_putstring("\nf1: Waiting for reply");
        message_wait();
        while ((reply = message_get())) {
            if (reply == (Message *) &m) {
                uart_putstring("\nf1: Reply received");
            }
        }
    }
}

void f2(void)
{
    Byte_Message *m;

    while(1) {
        message_wait();
        while ((m = (Byte_Message *) message_get())) {
            uart_putstring("\nf2: Received message: ");
            uart_put(m->d);
            message_put(m->m.source, (Message *) m);
        }
    }
}

int main(void)
{
    uart_init();
    kernel_init();
    t1 = task_create("T1", PRIORITY_NORMAL, f1, 64);
    assert(t1);
    t2 = task_create("T2", PRIORITY_NORMAL, f2, 64);
    assert(t2);
    assert(kernel_start());
    task_wait(2);
    return 0;
}
