#include <assert.h>
#include <inttypes.h>
#include <avr/io.h>
#include "kernel.h"
#include "port.h"
#include "uart_driver.h"

Task *uart_tx;

#define UART_TX_STACK 64

static void uart_init(void)
{
    #define F_CPU 8000000
    #define BAUD 31250
    #include <util/setbaud.h>
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
        UCSRA |= _BV(U2X);
    #else
        UCSRA &= ~(_BV(U2X));
    #endif
    /* Enable receiver and transmitter. */
    UCSRB = _BV(RXEN) | _BV(TXEN);
}

static void uart_tx_driver_program(void)
{
    Byte_Message *m;

    uart_init();
    while(1) {
        message_wait();
        while ((m = (Byte_Message *) message_get())) {
            loop_until_bit_is_set(UCSRA, UDRE);
            UDR = m->d;
            message_put(m->m.source, (Message *) m);
        }
    }
}

void uart_open() {
    uart_tx = task_create("UART_TX", PRIORITY_HIGH,
      uart_tx_driver_program, UART_TX_STACK);
    assert(uart_tx);
}

void uart_put(uint8_t v)
{
    Byte_Message m;
    m.d = v;
    message_put(uart_tx, (Message *) &m);
    message_wait();
    while (message_get()) {
    }
}

void uart_putchar(char c)
{
    if ('\n' == c) {
        uart_putchar('\r');
    }
    uart_put(c);
}

static char num_to_hex(uint8_t n)
{
    if (n > 9) {
        return n + 'a' - 10;
    } else {
        return n + '0';
    }
}

void uart_puthex(uint8_t v)
{
    uart_putchar(num_to_hex(v >> 4));
    uart_putchar(num_to_hex(v & 0x0f));
}

void uart_putstring(const char *string)
{
    char c;

    while ((c = *string++)) {
        uart_putchar(c);
    }
}

uint8_t uart_get(void)
{
    loop_until_bit_is_set(UCSRA, RXC);
	return UDR;
}

