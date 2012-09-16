#include <stdlib.h>
#include <avr/interrupt.h>
#include "kernel.h"
#include "kernel_port.h"
#include "kernel_log.h"
#include "port.h"

static Context *inter_sp;

inline void interrupts_enable(void)
{
    sei();
}

inline void interrupts_disable(void)
{
    cli();
}

Context *context_create(Function *entry, size_t stacksize)
{
    Context *c;
    uint8_t *bos;

    bos = calloc(1, stacksize + sizeof (Context));
    if (NULL == bos) {
        return NULL;
    }
    c = (Context *) (bos + stacksize);
    /* The C compiler assumes that register r1 is zero. */

    c->pc_low = (unsigned int) entry >> 8;
    c->pc_high = (unsigned int) entry;
    return c;
}

/* Output Compare Register. T = N(1+OCR0) / f */
#define TIMER0_OUTPUT_COMPARE_REGISTER_MS 124
/* Timer/Counter Control Register: prescaler f/64 */
#define TIMER0_CLOCK_SOURCE_MS (_BV(CS01) + _BV(CS00))

void port_timer_init(void)
{
    /* Clear Time on Compare (CTC) mode */
    TCCR0 |= _BV(WGM01) + TIMER0_CLOCK_SOURCE_MS;
    /* Output Compare Register.*/
    OCR0 = TIMER0_OUTPUT_COMPARE_REGISTER_MS;
    /* Timer/Counter 0: TCNT0 increases until a compare match
    occurs between TCNT0 and OCR0, and then TCNT0 is cleared. */
    TCNT0 = 0;
    /* TIFR */
    TIFR |= _BV(OCF0);
    /* Timer/Counter Interrupt Mask Register */
    /* Timer/Counter0 Output Compare Match Interrupt Enable */
    TIMSK |= _BV(OCIE0);
}

/* port_reschedule: Ensure the highest priority task is running.
pre conditions:
x Interrupts are disabled.
x running_task belongs to exactly one of the queues ready_tasks
  and waiting_tasks.
x running_task may not be the task in ready_tasks with the
  highest priority.
x The running_task pointer references the task that issued
  the call.

post conditions:
x Interrupts are enabled.
x running_task belongs to ready_tasks.
x running_task is the task with the highest priority.
x The running_task pointer references the task that returns
  from the call.
*/

static void reschedule(void)
{
    running_task->context = inter_sp;
    running_task = (Task *) list_head(&ready_tasks);
    inter_sp = running_task->context;
}

/* IMPORT NOTE REGARDING INTERRUPT ROUTINES AND
port_reschedule(). Be sure that the routines don't trash the
task stack. Watch out for local variables on the stack frame
(register Y=r28:r29). */

#define RR_TIMEOUT_MS ((uint16_t) 100)
static volatile uint16_t timer_tick = RR_TIMEOUT_MS;

void port_reschedule(void) __attribute__ ((naked));
void port_reschedule(void)
{
    SAVE_CONTEXT();
    log_line(running_task->name);
    log_string(" --> ");
    reschedule();
    timer_tick = RR_TIMEOUT_MS;
    log_string(running_task->name);
    RESTORE_CONTEXT();
    /* The RETI instruction enables interrupts. */
    __asm__ __volatile__ ("reti \n\t");
}

ISR(TIMER0_COMP_vect, ISR_NAKED) {
    SAVE_CONTEXT();
    /* Decide if a task change shall occur. We know that
    running_task is in the ready list. */
    if (0 == timer_tick) {
        list_remove_node((Node *) running_task);
        list_enqueue(&ready_tasks, (Node *) running_task);
        reschedule();
        timer_tick = RR_TIMEOUT_MS;
    } else {
        timer_tick--;
    }
    RESTORE_CONTEXT();
    __asm__ __volatile__ ("reti \n\t");
}

