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

class Interpreter;

const int cursor_width = 2;
const int blink_interval = 300;

class Stringc
{
	string text;
	Fontc font;

	Canvas lab;
	bool lab_upd;

public:
	Stringc ():lab_upd (false){}
	Stringc (const Stringc& orig) :text (orig.text), font (orig.font), lab_upd (false) {}
	Stringc (const string& str, Fontc f) :text (str), font (f), lab_upd (false) {}

	void insert (int pos, int number, char c);
	void clear();
	void erase (int pos, int number);
	inline string::size_type length() const {return text.size();}
	inline string substr (int start, int number) const {return text.substr(start, number);}
	inline bool empty() const {return text.empty();}
	inline const char* c_str() const {return text.c_str();}
	inline bool operator != (const string& str) const {return text != str;}
	inline bool operator == (const string& str) const {return text == str;}

	Stringc& operator = (const string& str);
	void setFont (Fontc f);

	int draw (Canvas* screen, Rect* brd, int offset = 0, bool color_reverse = false) const;//!!!??? Offset may be is tooish

	void dontUpdateLab();
	void updateLab();

	inline Point pSize (int offset = 0) const {return font.strSize (text.c_str() + offset);}
	inline int height (int offset = 0) const {return pSize (offset).y;}
	inline int width (int offset = 0) const {return pSize (offset).x;}

	int getFullHeight (Uint16 max_width) const;

	Stringc getBorderedSubstring (int width, int offset = 0) const;
			//There are more problems with russian characters

	operator string& () {return text;}
	operator const string& () const {return text;}

	bool ok() const {return font.ok() && (!lab_upd || (lab.ok() && lab.valid()));}
};

class Lines_view
{
	int max_lines;
	Fontc& fontDefault;
	Rect borders;
	list <Stringc> data;

	int drawText (Canvas* screen, const Stringc&, int start_offset) const;
	int Draw_tolerable_line (Canvas* screen, const Stringc&, int offset, Rect brd) const;
		
public:
	Lines_view (Fontc& f, int max_lines_) :fontDefault(f), max_lines (max_lines_) {}

	void init (const Rect&);
	void pushString (Stringc);
	void pushString (const string& str);
	void changeLastString (Stringc);
	void changeLastString (const string& str);
	void flush();
	void freshLine();
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
	Fontc cmdFont;
	Fontc printFont;
	Fontc respFont;
	Lines_view history;
	Line_edit input;

	Interpreter* interp;

	class Initializer;
	Initializer* parser;

	string eval(string str);

public:
	Console();
	virtual ~Console();

	Serializator* newParser();
	bool init (Graphic_subsystem* c, Interpreter* interp);
	void cleanup();
	void onEnterString (const string& str);
	void pushString (const string&);

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

//!!! only for debug
extern Console* interface;


#endif	/* _CONSOLE_H */

