#include <stdlib.h>
#ifdef CONFIG_USE_SERIAL

#ifdef CONFIG_SERIAL_BAUDRATE
#define BAUDRATE	CONFIG_SERIAL_BAUDRATE
#else
#define BAUDRATE	9600
#endif

#define MYUBBR		( ( F_CPU / BAUDRATE / 16 ) - 1 )

#define RULES_CP	PD2
#define RULES_PS	PD3
#define RULES_DO	PD4

void serial_init(void);   // Initializes Serial
void serial_write(char c); // Send a character over serial
void serial_write_bits_u32(uint32_t bits);

#endif
