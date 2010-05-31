/* 
 * File:   Exception.h
 * Author: necto
 *
 * Created on March 20, 2010, 3:00 PM
 */

#ifndef _Exception_H
#define	_Exception_H
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

class Exception
{
	string text;
	static Std_output_err* out;

public:
	Exception();
	Exception (const string& str, int line = 0, const char* file = 0);

	void print() {out->print (text);};

	inline static void Set_output (Std_output_err* nout) {Exception::out = nout;}
private:

};
string Sprintf (const char* fmt, ...);

#define GODFORBID(cond, msg) if (cond) throw Exception (msg);
#define GODFORBIDlf(cond, msg) if (cond) throw Exception (msg, __LINE__, __FILE__);

#endif	/* _Exception_H */

