/*
 * Logger.cpp
 *
 *  Created on: Sep 8, 2011
 *      Author: necto
 */

#include <time.h>
#include "../h/Logger.h"

using std::endl;

Logger Logger::def;

Logger& log (Levels level)
{
	return Logger::def << Logger::LogHeading (level);
}
//--------------------------------------------------------------------------------------------------
Logger& log (Levels level, const char* file, int line)
{
	return Logger::def << Logger::LogHeading (level, file, line);
}
//--------------------------------------------------------------------------------------------------
Logger::Logger()
	:file ("log.txt"), current (NOLEVEL), printFname (true),
	 printLineNumber (true), filter(NOLEVEL)
{
}
//--------------------------------------------------------------------------------------------------
Logger::~Logger()
{
	file.close();
}
//--------------------------------------------------------------------------------------------------
string Logger::levelName (Levels level)
{
	switch (level)
	{
	case INFO:
		return "INFO";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	}
	return "uncnown level";
}
//--------------------------------------------------------------------------------------------------
string Logger::timeStr()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [30];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (buffer, 30, "%X", timeinfo);
	return buffer;
}
//--------------------------------------------------------------------------------------------------
string Logger::heading (LogHeading head)
{
	if (head.hasPlace)
		return heading (head.level, head.file, head.line);
	else
		return heading (head.level, "");
}
//--------------------------------------------------------------------------------------------------
string Logger::heading (Levels level, string place)
{
	return string("[") + timeStr() + place + " | " + levelName (level) + "] ";
}
//--------------------------------------------------------------------------------------------------
string Logger::heading (Levels level, const char* file, int line)
{
	string place;
	if (printFname || printLineNumber) place += " (";
	if (printFname) place += file;
	if (printFname && printLineNumber) place += ":";
	if (printLineNumber)
	{
		char buf[32];
		sprintf (buf, "%d", line);
		place += buf;
	}

	if (printFname || printLineNumber) place += ")";

	return heading (level, place);
}
//--------------------------------------------------------------------------------------------------
void Logger::log (Levels level, string message)
{
	current = level;
	file<<heading (level) <<" " <<message <<endl;
}
//--------------------------------------------------------------------------------------------------
void Logger::startNewMessage (LogHeading head)
{
	current = head.level;
	out ("\n");
	out (heading (head));
}
