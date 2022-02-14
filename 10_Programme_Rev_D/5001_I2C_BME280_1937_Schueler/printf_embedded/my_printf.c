/**
* @file   	my_printf.c
* @autor  	rma / Cityline AG   
* @date   	2.4.2018
* @version 	1915V00
* @link     based on // https://www.menie.org/georges/embedded/small_printf_source_code.html
* @note     This small printf function is a tiny implementation of thestandard C library function, 
*           it does not include all the capabilities ofthe standard one. It has been designed 
*           to have a small footprint and to beeasy to include in an embedded software. 
*           The only external dependency isthe putchar() functions which output a single character 
*           to theoutput device. 
*           This printf function will accept integer formats (%d, %x,%X, %u), string format
*           (%s) and character format (%c); left and rightalignement, padding with space 
*            or 'O'. See the comments and the test codeat the end of the source code. 
*           
*************************************************************/


#include <stdio.h>										// lib for sprintf   .......
#include "debug.h"

/*
	putchar is the only external dependency for this file,
	if you have a working putchar, just remove the following
	define. If the function should be called something else,
	replace outbyte(c) by your own function call.
	#define putchar(c) outbyte(c)
*/
#define putchar(c) outbyte(c)    // print(c) from touchlib

void _outbyte(int c)
{
//       unsigned char Status=0;     
//       char loc[1];
//       loc[0]	= (char)c;
//       print(loc);     // Write to Touch on last line
//       Check, if Buffertesting is needed? Mal 14.4.2019
	 	 USARTWrite(1, c);   // transmitt content in "c" to Usart nr 1. 
}

int outbyte(int c)
{
   static char prev = 0;     // staic var prev which  becomes 0 during compiling.   After Reset prev could be set to any value
    if (c < ' ' && c != '\r' && c != '\n' && c != '\t' && c != '\b')
       return 0;
   if (c == '\n' && prev != '\r') _outbyte('\r');
    _outbyte(c);
    prev = c;
		 return 0;
}

// ---------------------------------------------

static void printchar(char **str, int c)
{
	extern int putchar(int c);
	if (str) {
		**str = c;
		++(*str);
	}
	else (void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int printloc(char **out, int *varg)
{
	register int width, pad;
	register int pc = 0;
	register char *format = (char *)(*varg++);
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	return pc;
}

/* assuming sizeof(void *) == sizeof(int) */

int my_printf(const char *format, ...)
{
	register int *varg = (int *)(&format);
	return printloc(0, varg);
}

int my_sprintf(char *out, const char *format, ...)
{
	register int *varg = (int *)(&format);
	return printloc(&out, varg);
}

