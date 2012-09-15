void uart_init(void);
void uart_putchar(char c);
void uart_puthex(uint8_t v);
void uart_putstring(const char *string);

#ifdef DEBUG
#define DEBUG_LOG 1
#else
#define DEBUG_LOG 0
#endif

#define log_init() \
    do { \
        if (DEBUG_LOG) { \
            uart_init(); \
        } \
    } while (0)

#define log_string(string) \
    do { \
        if (DEBUG_LOG) { \
            uart_putstring(string); \
        } \
    } while (0)

#define log_line(string) \
    do { \
        if (DEBUG_LOG) { \
            uart_putstring("\n" __FILE__ ":"); \
            uart_puthex(__LINE__); \
            uart_putchar(':'); \
            uart_putstring(__func__); \
            uart_putstring("(): "); \
            uart_putstring(string); \
        } \
    } while (0)

