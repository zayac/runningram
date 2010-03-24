/* 
 * File:   Exeption.h
 * Author: necto
 *
 * Created on March 20, 2010, 3:00 PM
 */

#ifndef _EXEPTION_H
#define	_EXEPTION_H
#include <string>
#include <stdio.h>

using std::string;

class Std_output_err
{
public:
	virtual void print (const string& text) = 0;
};

class Output_cerr :public Std_output_err
{
public:
	virtual void print (const string& text);
};

class Exeption
{
	string text;
	static Std_output_err* out;

public:
	Exeption() :text("unnamed exeption"){}
	Exeption (const string& str, int line = 0, char* file = 0):text (str)
	{
		char buf[128] = "";
		if (file != 0)
			sprintf (buf, "(at line: %d; in file: %s)", line, file);
		else if (line != 0)
			sprintf (buf, "(at line: %d;)", line);
		text += buf;
	};

	void print() {out->print (text);};

	inline static void Set_output (Std_output_err* nout) {Exeption::out = nout;}
private:

};
string Sprintf (char* fmt, ...);

#endif	/* _EXEPTION_H */

