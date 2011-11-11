#include <stdlib.h>

#define BAUDRATE	9600
#define MYUBBR		( ( F_CPU / BAUDRATE / 16 ) - 1 )

#define RULES_CP	PD2
#define RULES_PS	PD3
#define RULES_DO	PD4

void serial_init(void);   // Initializes Serial
void serial_write(char c); // Send a character over serial
