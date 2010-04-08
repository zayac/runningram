#include <stdarg.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include "Exception.h"

Std_output_err* Exception::out = new Output_cerr;


Exception::Exception()
	:text("unnamed Exception")
{
	text += "; errno:";
	text += strerror(errno);
}

Exception::Exception (const string& str, int line, char* file)
	:text (str)
{
	char buf[128] = "";
	if (file != 0)
		sprintf (buf, "(at line: %d; in file: %s)", line, file);
	else if (line != 0)
		sprintf (buf, "(at line: %d;)", line);
	text += buf;
	text += "errno:";
	text += strerror(errno);
}

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
