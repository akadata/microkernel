#include <stdlib.h>
#include <avr/interrupt.h>
#include "kernel.h"
#include "kernel_port.h"

struct context {
    struct {
        /* an anonymous padding byte */
        unsigned char _padding_byte;
        /* general purpose registers */
        unsigned char r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7;
        unsigned char r8,  r9,  r10, r11, r12, r13, r14, r15;
        unsigned char r16, r17, r18, r19, r20, r21, r22, r23;
        unsigned char r24, r25, r26, r27, r28, r29, r30;
        /* status register */
        unsigned char rSREG;
        unsigned char r31;
        /* program counter */
        unsigned char pc_low, pc_high;
    } *stack_frame;
};

Context *context_create(Function *entry)
{
    Context *c;

    interrupts_disable();
    c = malloc(sizeof (Context));
    interrupts_enable();
    if (NULL == c) {
        return NULL;
    }

    /* Memory for the context has been allocated. Initialize
    it. */
    return c;
}

void interrupts_enable(void)
{
    sei();
}

void interrupts_disable(void)
{
    cli();
}

void port_reschedule(void)
{
    /* save context */
    /* reschedule */
    /* restore context */
}

