#include <inttypes.h>
#include <avr/io.h>
#include "kernel_log.h"

#define F_CPU 8000000

void port_log_init(void)
{
    #undef BAUD  // avoid compiler warning
    #define BAUD 31250
    /* #define BAUD 2400 */
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
    port_log_putchar('\n');
    log_line("OK");
}

void port_log_putchar(char c)
{
    if (c == '\n') {
        port_log_putchar('\r');
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

void port_log_puthex(uint8_t v)
{
    port_log_putchar(num_to_hex(v >> 4));
    port_log_putchar(num_to_hex(v & 0x0f));
}

void port_log_putstring(const char *string)
{
  char c;

  while ((c = *string++)) {
    port_log_putchar(c);
  }
}

char port_log_getchar(void)
{
    loop_until_bit_is_set(UCSRA, RXC);
	return UDR;
}

/*
static void port_log_puttest(void)
{
    uint8_t i;
    uint16_t t;

    port_log_init();
    port_log_putstring("\nUART initialized.\n");
    i = 0;
    while (1) {
        while(t++);
        port_log_puthex(i++);
        port_log_putchar(' ');
        if (0 == (i % 16)) {
            port_log_putchar('\n');
        }
    }
}
*/

/*
int main(void) { port_log_puttest();}
*/
