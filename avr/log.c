#include <inttypes.h>
#include <avr/io.h>
#include "log.h"

#define F_CPU 8000000

void uart_init(void)
{
    #undef BAUD  // avoid compiler warning
    /* #define BAUD 31250 */
    #define BAUD 2400
    #include <util/setbaud.h>
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
        UCSRA |= (1 << U2X);
    #else
        UCSRA &= ~(1 << U2X);
    #endif
    /* Enable receiver and transmitter. */
    UCSRB=(1<<RXEN)|(1<<TXEN);
    uart_putchar('\n');
    log_line("OK");
}

void uart_putchar(char c)
{
    if (c == '\n') {
        uart_putchar('\r');
    }
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
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

char uart_getchar(void)
{
    loop_until_bit_is_set(UCSRA, RXC);
	return UDR;
}

/*
static void uart_puttest(void)
{
    uint8_t i;
    uint16_t t;

    uart_init();
    uart_putstring("\nUART initialized.\n");
    i = 0;
    while (1) {
        while(t++);
        uart_puthex(i++);
        uart_putchar(' ');
        if (0 == (i % 16)) {
            uart_putchar('\n');
        }
    }
}
*/

/*
int main(void) { uart_puttest();}
*/
