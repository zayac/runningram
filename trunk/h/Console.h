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
#include "Key_event.h"
#include "Canvas.h"
#include "Vec.h"
#include "Graphic_subsystem.h"
#include "Functor.h"
#include "Fontc.h"
#include "Exception.h"
#include "Functor.h"

using std::string;
using std::list;

class lua_State;

const int cursor_width = 2;
const int blink_interval = 300;

class Stringc :public string
{
	Fontc font;

	Canvas lab;
	bool lab_upd;

public:
	Stringc ():lab_upd(false){}//:font(){}
	Stringc (const string& str, Fontc f) :string(str), font(f), lab_upd(false) {}

	Stringc& operator = (const string& str) {string::operator =(str); lab_upd = false;}
	void setFont (Fontc f) {font = f; lab_upd = false;}

	inline int draw (Canvas* screen, Rect* brd, int offset = 0, bool color_reverse = false) const//!!!??? Offset may be is tooish
	{
		assert(brd != 0);
		if (lab_upd && !color_reverse)//!!! Not debugged yet
		{
			int pos = font.strLen (substr(0, offset).c_str());
			screen->copy (lab, Point(pos, 0), Rect (brd->x, brd->y, brd->w - pos, brd->h));
			return min (brd->h, lab.getHeight());
		}
		else
			return font.drawLine (screen, c_str() + offset, brd, color_reverse);
	}

	void updateLab();

	inline Point pSize (int offset = 0) const {return font.strSize (c_str() + offset);}
	inline int height (int offset = 0) const {return pSize (offset).y;}
	inline int width (int offset = 0) const {return pSize (offset).x;}

	Stringc getBorderedSubstring (int width, int offset = 0) const;
			//There are more problems width russian characters

	operator string& () {return *(string*)this;}

	bool ok() const {return font.ok() && (!lab_upd || lab.ok());}
};

class Lines_view
{
	int max_lines;
	Fontc& font;
	Rect borders;
	list <Stringc> data;

	int drawText (Canvas* screen, const Stringc&, int start_offset) const;
	int Draw_tolerable_line (Canvas* screen, const Stringc&, int offset, Rect brd) const;
		

	int getHeight (const Stringc& what) const;

public:
	Lines_view (Fontc& f, int max_lines_) :font(f), max_lines (max_lines_){}

	void init (const Rect&);
	void pushString (const string&);
	void draw (Canvas* c) const;

	bool ok() const;
};

class Line_edit
{
	Fontc& font;
	Stringc greeting;
	int grsize;
	Stringc data;
	int cursor_pos;
	int start_view;
	Rect borders;
	Arg_Functor <void, const string&> *on_enter;

	Uint32 last_actiont;

	void drawText (Canvas* c) const;
	void drawCursor (Canvas* c) const;

	void cursorLeft();
	void cursorRight();
	void cursorHome();
	void cursorEnd();
	void deleteLeft();
	void deleteRight();
	void finishInput();
	void typeChar (Kbd_event& ev);
	void insertSymbol (char);

	int cursorOffset() const;
	
public:
	Line_edit (Fontc& f) :font(f), on_enter(0){}
	~Line_edit();

	const string& getGreeting() const {return greeting;}
	
	void init (const Rect& brd, Arg_Functor <void, const string&> *on_enter, string greeting = "");
	void operate (Kbd_event ev);
	void draw (Canvas* c) const;

	bool ok() const;
};

class Console
{
	bool enabled;
	Fontc font;
	Lines_view history;
	Line_edit input;

	lua_State* vm;

	class Initialaiser;
	Initialaiser* parser;

public:
	Console();
	virtual ~Console();

	Serializator* getParser();
	bool init (Graphic_subsystem* c);
	void cleanup();
	void onEnterString (const string& str);
	void pushString (const string&);
	int output (lua_State* ls);

	void operate (Kbd_event ev);
	void draw (Graphic_subsystem* c) const;

	void registerProcessor (string name, int (*fun)(lua_State*));
	void registerProcessor (string name, Arg_Functor <int, lua_State*>*);

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

//!!! only for debug
extern Console* interface;


#endif	/* _CONSOLE_H */

