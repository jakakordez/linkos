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
	if ((long)n/10 > 0) c+=printFloat(out, n/10., offset+1);
	*(out+c) = bases[(long)n%10];
	c++;
	if(offset == 0){
		for(double l = 10.; (n-(long)n) != 0 && l < 10000.; l*=10.){
			if(l == 10.){
				*(out+c) = ',';
				c++;
			}
			*(out+c) = bases[(long)(n*l)%10];
			c++;
		}
	}
	return c;
}

void process(char **format, char **result, va_list *valist){
	int n;
	double f;
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
			if(f < 0) {
				f*=-1;
				**result = '-';
				*result+=1;
			}
			*result += printFloat(*result, f, 0);
		break;
		case 'p':
			*format += 1;
			f = (double)va_arg(*valist, double);
			if(f < 0) {
				f*=-1;
				**result = '-';
				*result+=1;
			}
			int prefix = 6;
			if(f > 1){
				while(f >= 1000 && prefix++ < 12) f /= 1000.;
			}
			else{
				while(f < 1 && prefix-- > 0) f *= 1000.;
			}

			*result += printFloat(*result, f, 0);
			**result = ' ';
			*result+=1;
			if(prefix != 6){
				**result = prefixes[prefix];
				*result+=1;
			}
		break;
	}
}

void sprintf(char *presult, char *format, va_list *valist){
	while(*format){
		if(*format == '\\'){
			format++;
			*presult++ = *format++;
		}
		else if(*format == '%'){
			format ++;
			process(&format, &presult, valist);
		}else{
			*presult++ = *format++;
		}
	}
	*presult = '\0';
}

char *printf(char *format, ...){
	va_list valist;
	va_start(valist, format); /* initialize valist for num number of arguments */
	sprintf(result, format, &valist);
	va_end(valist); /* clean memory reserved for valist */

	return result;
}
