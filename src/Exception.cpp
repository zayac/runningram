#include <stdarg.h>
#include <iostream>
#include "Exception.h"

Std_output_err* Exception::out;

void Output_cerr::print (const string& text)
{
	std::cerr << text;
}

string Sprintf (char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	char buf[512];
	vsnprintf (buf, 512, fmt, ap);
	va_end (ap);
	return buf;
}
