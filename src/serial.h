#ifndef SERIAL
#define SERIAL
#include "io.h"

#define COM1    0x3f8   /* COM1 */
#define COM2 	0x2F8
#define COM3 	0x3E8
#define COM4 	0x2E8 

void init_serial(int comPort);
char read_serial(int comPort);
void write_serial(int comPort, char a);

#endif
