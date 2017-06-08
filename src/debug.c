#include <stdarg.h>
#include "debug.h"
#include "serial.h"
#include "string.h"

char buffer[1000];

void init_debug(){
    init_serial(COM1);
}

void dprint(char *string){
    while(*string) write_serial(COM1, *string++);
}

void dprintf(char *format, ...){
    va_list valist;
	va_start(valist, format); /* initialize valist for num number of arguments */
	sprintf(buffer, format, &valist);
	va_end(valist); /* clean memory reserved for valist */
    dprint(buffer);
}

