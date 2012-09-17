/* Define DEBUG to turn on debug output. When DEBUG is not
defined, the extra code will be taken care of (removed) by
the compiler. */

void port_log_init(void);
void port_log_putchar(char c);
void port_log_puthex(uint8_t v);
void port_log_putstring(const char *string);

#ifdef DEBUG
#define DEBUG_LOG 1
#else
#define DEBUG_LOG 0
#endif

#define log_init() \
    do { \
        if (DEBUG_LOG) { \
            port_log_init(); \
        } \
    } while (0)

#define task_line(string) \
    do { \
        if (DEBUG_LOG) { \
            port_log_putstring("\n"); \
            port_log_putstring(running_task->name); \
            port_log_putchar(':'); \
            port_log_putstring(__func__); \
            port_log_putstring("(): "); \
            port_log_putstring(string); \
        } \
    } while (0)

#define log_hex(n) \
    do { \
        if (DEBUG_LOG) { \
            port_log_puthex(n); \
        } \
    } while (0)

#define log_string(string) \
    do { \
        if (DEBUG_LOG) { \
            port_log_putstring(string); \
        } \
    } while (0)

#define log_line(string) \
    do { \
        if (DEBUG_LOG) { \
            port_log_putstring("\n" __FILE__ ":"); \
            port_log_putstring(__func__); \
            port_log_putstring("(): "); \
            port_log_putstring(string); \
        } \
    } while (0)

