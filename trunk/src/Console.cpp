/* 
 * File:   Console.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:58 PM
 */

//#define FULL_DEBUG

#include <time.h>
#include "initparser.h"
#include "Console.h"
#include "Graphic_subsystem.h"
#include "Interpreter.h"
#include "Car.h"
#include <assert.h>

// <editor-fold defaultstate="collapsed" desc="From file Initializer">

class Console::Initializer : public Sectionp
{
	Color fg_col;
	Color bg_col;
	Color cmd_col;
	Color resp_col;
	Fontc* printFont;
	Fontc* cmdFont;
	Fontc* respFont;
public:
	string cmdHistoryFile;
protected:
	bool afterRead (ifstream&)
	{
		printFont->setFG (fg_col);
		printFont->setBG (bg_col);
		*cmdFont = *printFont;
		cmdFont->setFG(cmd_col);
		*respFont = *printFont;
		respFont->setFG(resp_col);
		return true;
	}
public:

	Initializer (string name, Fontc* cmd_font, Fontc* print_font, Fontc* resp_font)
	: Sectionp (name, '='), printFont (print_font), cmdFont(cmd_font), respFont(resp_font),
	  cmdHistoryFile (".commands.history")
	{
		addParam (new Fontc::Initializer ("font", print_font));
		addParam (new Color::Initializer ("color", &fg_col));
		addParam (new Color::Initializer ("commands color", &cmd_col));
		addParam (new Color::Initializer ("response color", &resp_col));
		addParam (new Color::Initializer ("bgcolor", &bg_col));
		addParam (new St_loader<string> ("command history file", &cmdHistoryFile));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>

Console::Console ()
:history (printFont, 50), input (cmdFont)
{
}
//--------------------------------------------------------------------------------------------------
Console::~Console ()
{
	cmdHistory.saveToFile(parser->cmdHistoryFile);
}
//--------------------------------------------------------------------------------------------------
Serializator* Console::newParser()
{
	parser = new Initializer ("[Console]", &cmdFont, &printFont, &respFont);
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Console::init (Graphic_subsystem* c, Interpreter* interp_)
{
	Rect borders = c->getScreen()->getClipRect();
	borders.h = 210;
	history.init (borders);
	history.pushString ("You are welcome!");

	borders.y = borders.h;
	borders.h = printFont.height();
	input.init  (borders, new Arg_Method<void, const string&, Console> (this, &Console::onEnterString), "You:>");

	history.flush();
	interp = interp_;
	interp->regOutput (new Arg_Method<void, const string&, LinesView>
								(&history, &LinesView::changeLastString),
					   new Arg_Method<void, void, LinesView>
								(&history, &LinesView::flush));

	enabled = false;

	input.setOnUp (new Arg_Method<void, void, Console> (this, &Console::cmdHistoryUp));
	input.setOnDown (new Arg_Method<void, void, Console> (this, &Console::cmdHistoryDown));
	cmdHistory.initFromFile(parser->cmdHistoryFile);

	assert(ok());

	return true;
}
//--------------------------------------------------------------------------------------------------
void Console::operate (Kbd_event ev)
{
	assert(ok());
	if (enabled)
		input.operate (ev);
}
//--------------------------------------------------------------------------------------------------
void Console::draw (Graphic_subsystem* c) const
{
	if (!enabled) return;
	Canvas* screen = c->getScreen ();
	screen->pushPos(Point());
	assert(ok());
	history.draw (screen);
	input.draw (screen);
	screen->popPos();
}
//--------------------------------------------------------------------------------------------------
void Console::turn()
{
	enabled = !enabled;
}
//--------------------------------------------------------------------------------------------------
void Console::onEnterString (const string& str)
{
	assert(ok());
	cmdHistory.push (str);
	history.pushString (Stringc (input.getGreeting() + str, cmdFont));
	history.freshLine();
	string response = eval (str);
	history.pushString (Stringc (response, respFont));
}
//--------------------------------------------------------------------------------------------------
string Console::eval (string str)
{
	if (str[0] != '(')
		return interp->toString (interp->eval ("(" + str + ")"));

	return interp->toString (interp->eval (str));
}
//--------------------------------------------------------------------------------------------------
void Console::pushString (const string& str)
{
	assert(ok());
	history.pushString (str);
}
//--------------------------------------------------------------------------------------------------
void Console::cmdHistoryUp()
{
	input.setContent(cmdHistory.next());
}
//--------------------------------------------------------------------------------------------------
void Console::cmdHistoryDown()
{
	input.setContent(cmdHistory.prev());
}
//--------------------------------------------------------------------------------------------------
bool Console::ok() const
{
	return printFont.ok() && history.ok() && input.ok() && parser != 0;
}
//--------------------------------------------------------------------------------------------------
CommandHistory::CommandHistory()
{
	current = history.begin();
}
//--------------------------------------------------------------------------------------------------
void CommandHistory::initFromFile (const string& fname)
{
	ifstream file (fname.c_str());
	char buf[1024];
	while (file.good() && !file.eof())
	{
		file.getline(buf, 1024);
		push(buf);
	}
	file.close();
}
//--------------------------------------------------------------------------------------------------
void CommandHistory::saveToFile (const string& fname) const
{
	ofstream file(fname.c_str());
	list<string>::const_reverse_iterator iter = history.rbegin();
	for (int i = 0;
		 i < 50 && iter != history.rend();
		 ++i, ++iter)
		if (*iter != "")
			file << *iter <<"\n";
	file.close();
}
//--------------------------------------------------------------------------------------------------
string CommandHistory::next()
{
	if (current == history.end()) return "";
	return *(current++);
}
//--------------------------------------------------------------------------------------------------
string CommandHistory::prev()
{
	if (  current == history.begin() ||
		--current == history.begin())
		return "";
	return *((--current)++);//look back by 1
}
//--------------------------------------------------------------------------------------------------
void CommandHistory::push (const string& cmd)
{
	history.push_front (cmd);
	current = history.begin();
}
//--------------------------------------------------------------------------------------------------
