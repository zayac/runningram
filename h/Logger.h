/*
 * Logger.h
 *
 *  Created on: Sep 8, 2011
 *      Author: necto
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <iostream>
#include <string>

using std::string;

enum Levels
{
	NOLEVEL,
	INFO,
	WARNING,
	ERROR
};

class Logger
{
public:
	struct LogHeading
	{
		Levels level;
		bool hasPlace;
		const char* file;
		int line;

		LogHeading (Levels lev) :level(lev), hasPlace (false){}
		LogHeading (Levels lev, const char* fname, int lnumber)
				:level(lev), hasPlace(true), file (fname), line(lnumber) {}
	};
	static string levelName (Levels level);
	static string timeStr();
private:

	std::ofstream file;
	Levels current;

public:
	static Logger def;

	bool printFname, printLineNumber;
	Levels filter;

	Logger();
	virtual ~Logger();

	string heading (LogHeading head);
	string heading (Levels level, string place);
	string heading (Levels level, const char* file, int line);

	void log (Levels level, string message);
	void startNewMessage (LogHeading head);

	template <class T>
	void out (const T& smth)
	{
		if (current > filter)
			file <<smth;
	}

	template <class T>
	inline Logger& operator << (const T& smth)
			   {out (smth); return *this;}
};

template <>
inline Logger& Logger::operator << (const Logger::LogHeading& head)
{
	startNewMessage (head);
	return *this;
}

Logger& log (Levels level);
Logger& log (Levels level, const char* file, int line);

#define LOG(level) log(level, __FILE__, __LINE__)

#endif /* LOGGER_H_ */
