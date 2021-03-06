/* 
 * File:   Console.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:58 PM
 */

#ifndef _CONSOLE_H
#define	_CONSOLE_H

#include <list>
#include <string>
#include "Canvas.h"
#include "Vec.h"
#include "Graphic_subsystem.h"
#include "Functor.h"
#include "Fontc.h"
#include "Exception.h"
#include "Functor.h"
#include "Lineedit.h"
#include "Linesview.h"

using std::string;
using std::list;

class Interpreter;

class CommandHistory
{
	list<string> history;
	list<string>::const_iterator current;

public:
	CommandHistory();

	void initFromFile (const string& fname);
	void saveToFile (const string& fname) const;

	string next();
	string prev();

	void push (const string& cmd);
};

class Console
{
	bool enabled;
	Fontc cmdFont;
	Fontc printFont;
	Fontc respFont;
	LinesView history;
	LineEdit input;
	CommandHistory cmdHistory;

	Interpreter* interp;

	class Initializer;
	Initializer* parser;

	string eval(string str);

public:
	Console();
	virtual ~Console();

	Serializator* newParser();
	bool init (Graphic_subsystem* c, Interpreter* interp);
	void onEnterString (const string& str);
	void pushString (const string&);

	void cmdHistoryUp();
	void cmdHistoryDown();

	void operate (Kbd_event ev);
	void draw (Graphic_subsystem* c) const;

	void turn();

	bool ok() const;
};


class Console_output :public Std_output_err
{
	Console* out;
public:
	Console_output (Console* what):out (what){;}
	virtual void print (const string& text)
	{
		out->pushString (text);
	}
};


#endif	/* _CONSOLE_H */

