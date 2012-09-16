typedef struct {
    Message m;
    uint8_t d;
} Byte_Message;

void uart_open(void);
void uart_putchar(char c);
void uart_put(uint8_t v);
void uart_puthex(uint8_t v);
void uart_putstring(const char *string);
uint8_t uart_get(void);

