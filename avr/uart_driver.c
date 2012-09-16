#include <inttypes.h>
#include <avr/io.h>
#include "uart_driver.h"

void uart_init(void)
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
    uart_putstring("\nUART driver initialized\n");
}

void uart_putchar(char c)
{
    if ('\n' == c) {
        uart_putchar('\r');
    }
    uart_put(c);
}

void uart_put(uint8_t v)
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = v;
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

