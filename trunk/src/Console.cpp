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
	: Sectionp (name, '='), printFont (print_font), cmdFont(cmd_font), respFont(resp_font)
	{
		addParam (new Fontc::Initializer ("font", print_font));
		addParam (new Color::Initializer ("color", &fg_col));
		addParam (new Color::Initializer ("commands color", &cmd_col));
		addParam (new Color::Initializer ("response color", &resp_col));
		addParam (new Color::Initializer ("bgcolor", &bg_col));
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

	assert(ok());

	return true;
}
//--------------------------------------------------------------------------------------------------
void Console::cleanup()
{
	assert(ok());//!!! It may be necessary to close all opened fonts before
//        FontcCleanUp();
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
bool Console::ok() const
{
	return printFont.ok() && history.ok() && input.ok() && parser != 0;
}
//--------------------------------------------------------------------------------------------------
