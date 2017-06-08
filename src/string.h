#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

size_t strlen(const char* str);
int strcpy(char *src, char *dst);
int strcpyz(char *src, char *dst);
void sprintf(char *presult, char *format, va_list *valist);
char *printf(char *format, ...);
