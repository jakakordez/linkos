#include <stdarg.h>
#include "string.h"

char result[2000];
char bases[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
char prefixes[] = "afpnum kMGTPE";

size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 ) ret++;
	return ret;
}

int strcpy(char *src, char *dst){
	int count = 0;
	while(*src){
		*dst++ = *src++;
		count ++;
	}
	return count;
}

int strcpyz(char *src, char *dst){
	int count = strcpy(src, dst);
	*dst = '\0';
	return count;
}

int printBase(char *out, int n, int base){
	int c = 0;
	if (n/base != 0) c+=printBase(out, n/base, base);
	*(out+c) = bases[n%base];
	return c+1;
}

int printFloat(char *out, double n, int offset){
	int c = 0;
	if (n/10 != 0) c+=printFloat(out, n/10., offset+1);
	*(out+c) = bases[(long)n%10];
	c++;
	if(offset == 0){
		*(out+c) = ',';
		while(n > 0){
			n *= 10.;
			c++;
			*(out+c) = bases[(long)n%10];
		}
	}
	return c+1;
}

void process(char **format, char **result, va_list *valist){
	int n;
	float f;
	switch(**format){
		case 'c':
			*format += 1;
			**result = (char)va_arg(*valist, int);
			*result += 1;
		break;
		case 's':
			*format += 1;
			*result += strcpy((char *)va_arg(*valist, char *), *result);
		break;
		case 'h': case 'd':
			*format += 1;
			n = (int)va_arg(*valist, int);
			if(n < 0) {
				n*=-1;
				**result = '-';
				*result+=1;
			}
			*result += printBase(*result, n, (**format == 'h')?16:10);
		break;
		case 'f':
			*format += 1;
			f = (double)va_arg(*valist, double);
			*result += printFloat(*result, f, 0);
		break;
		case 'p':
			/**format += 1;
			f = (int)va_arg(*valist, int);
			if(n < 0) {
				n*=-1;
				**result = '-';
				*result+=1;
			}
			int prefix = 0;
			while(n > 1000){
				n /= 1000.;
				prefix++;
			}
			*result += printBase(*result, n, (**format == 'h')?16:10);*/
		break;
	}
}

char * printf(char *format, ...){
	va_list valist;

	char *presult = result;

	/* initialize valist for num number of arguments */
	va_start(valist, format);

	while(*format){
		if(*format == '\\'){
			format++;
			*presult++ = *format++;
		}
		else if(*format == '%'){
			format ++;
			process(&format, &presult, &valist);
		}else{
			*presult++ = *format++;
		}
	}
	*presult = '\0';

	/* clean memory reserved for valist */
	va_end(valist);

	return result;
}