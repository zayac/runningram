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

using std::string;
using std::list;

const int cursor_width = 2;
const int blink_interval = 300;

class Stringc :public string
{
	Fontc font;

public:
	Stringc (){}//:font(){}
	Stringc (const string& str, Fontc f) :string(str), font(f) {}

	Stringc& operator = (const string& str) {string::operator =(str);}
	void Set_font (Fontc f) {font = f;}

	inline int Draw (Canvas* screen, Rect* brd, int offset = 0, bool color_reverse = false) const//!!!??? Offset may be is tooish
		{return font.Draw_line (screen, c_str() + offset, brd, color_reverse);}

	inline Point pSize (int offset = 0) const {return font.Str_size (c_str() + offset);}
	inline int Height (int offset = 0) const {return pSize (offset).y;}
	inline int Width (int offset = 0) const {return pSize (offset).x;}

	Stringc Get_bordered_substring (int width, int offset = 0) const;
			//There are more problems width russian characters

	operator string& () {return *(string*)this;}

	bool Ok() const {return font.Ok();}
};

class Lines_view
{
	int max_lines;
	Fontc& font;
	Rect borders;
	list <Stringc> data;

	int Draw_text (Canvas* screen, const Stringc&, int start_offset) const;
	int Draw_tolerable_line (Canvas* screen, const Stringc&, int offset, Rect brd) const;
		

	int Get_height (const Stringc& what) const;

public:
	Lines_view (Fontc& f, int max_lines_) :font(f), max_lines (max_lines_){}

	void Init (const Rect&);
	void Push_string (const string&);
	void Draw (Canvas* c) const;

	bool Ok() const;
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

	void Draw_text (Canvas* c) const;
	void Draw_cursor (Canvas* c) const;

	void Cursor_left();
	void Cursor_right();
	void Cursor_home();
	void Cursor_end();
	void Delete_left();
	void Delete_right();
	void Finish_input();
	void Type_char (Kbd_event& ev);
	void Insert_symbol (char);

	int Cursor_offset() const;
	
public:
	Line_edit (Fontc& f) :font(f), on_enter(0){}
	~Line_edit();

	const string& Get_greeting() const {return greeting;}
	
	void Init (const Rect& brd, Arg_Functor <void, const string&> *on_enter, string greeting = "");
	void Operate (Kbd_event ev);
	void Draw (Canvas* c) const;

	bool Ok() const;
};

class Console
{
	bool enabled;
	Fontc font;
	Lines_view history;
	Line_edit input;

	class Initialaiser;
	Initialaiser* parser;

public:
	Console();
	virtual ~Console();

	Serializator* Get_parser();
	bool Init (Graphic_subsystem* c);
	void Cleanup();
	void On_enter_string (const string& str);
	void Push_string (const string&);

	void Operate (Kbd_event ev);
	void Draw (Graphic_subsystem* c) const;

	void Switch();

	void Out (const string& str);

	bool Ok() const;
};


class Console_output :public Std_output_err
{
	Console* out;
public:
	Console_output (Console* what):out (what){;}
	virtual void print (const string& text)
	{
		out->Push_string (text);
	}
};

//!!! only for debug
extern Console* interface;


#endif	/* _CONSOLE_H */

