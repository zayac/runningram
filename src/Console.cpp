/* 
 * File:   Console.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:58 PM
 */

#include <time.h>
#include "initparser.h"
#include "Console.h"
#include "Graphic_subsystem.h"

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Console::Initialaiser : public Sectionp
{
public:
	string font_fname;
	int height;
public:

	Initialaiser (string name)
	: Sectionp (name, '='), font_fname ("default.ttf"), height (16)
	{
		Add_param (new St_loader<int > ("font height", &height));
		Add_param (new St_loader<string > ("font file", &font_fname));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>

#define MSECS (clock()*1000/CLOCKS_PER_SEC)


//!!! only for debug
Console* interface = 0;

Console::Console () :parser (new Initialaiser ("[Console]")), history (font), input (font)
{
    //!!! only for debug
    interface = this;
}
//--------------------------------------------------------------------------------------------------
Console::~Console ()
{
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Console::Get_parser()
{
	assert(Ok());
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Console::Init (Graphic_subsystem* c)
{
	font.Open_font (parser->font_fname.c_str(), parser->height);

	font.Set_fg (Color(10, 200, 20));
	font.Set_bg (Color(10, 20, 20));

	Rect borders = c->Get_screen()->getClipRect ();
	borders.h = 210;
	history.Init (borders);
	history.Push_string ("You are welcome!");

	borders.y = borders.h;
	borders.h = font.Height ();
	input.Init  (borders, new Arg_Method<void, string*, Console> (this, &Console::On_enter_string), "You:>");

	assert(Ok());

	return true;
}
//--------------------------------------------------------------------------------------------------
void Console::Cleanup()
{
	assert(Ok());//!!! It may be needed to close opend font
        FontcCleanUp();
}
//--------------------------------------------------------------------------------------------------
void Console::Operate (Kbd_event ev)
{
	assert(Ok());
	input.Operate (ev);
}
//--------------------------------------------------------------------------------------------------
void Console::Draw (Graphic_subsystem* c) const
{
	assert(Ok());
	history.Draw (c->Get_screen ());
	input.Draw (c->Get_screen ());
}
//--------------------------------------------------------------------------------------------------
void Console::Out (const string& str)
{
    history.Push_string (str);
}
//--------------------------------------------------------------------------------------------------
void Console::On_enter_string (string* str)
{
	assert(str != 0);
	assert(Ok());
	history.Push_string (input.Get_greeting() + *str);
}
//--------------------------------------------------------------------------------------------------
bool Console::Ok() const
{
	return font.Ok() && history.Ok() && input.Ok() && parser != 0;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Init (const Rect& brd, Arg_Functor <void, string*> *enter, string gr)
{
	on_enter = enter;
	data = "Very very very long  long test edit string for test drawing of text A same characters other"
		   "Very very very long  long test edit string for test drawing of text A same characters other"
		   "Very very very long  long test edit string for test drawing of text A same characters other"
		   "Very very very long  long test edit string for test drawing of text A same characters other";
	data.Set_font (font);
	cursor_pos = 0;
	start_view = 0;
	borders = brd;
	last_actiont = MSECS;
	greeting = gr;
	greeting.Set_font (font);
	grsize = greeting.Width();
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Operate (Kbd_event ev)
{
	assert(Ok());
	if (ev.type == EV_KEYUP) return;

	last_actiont = MSECS;

	bool num_lock = ev.mod & KM_NUM;

	if (!num_lock)
		switch (ev.ki)
		{
		case KI_KP1:			Cursor_end();		return;
		case KI_KP2:						return;
		case KI_KP3:						return;
		case KI_KP4:			Cursor_left();		return;
		case KI_KP5:						return;
		case KI_KP6:			Cursor_right();		return;
		case KI_KP7:			Cursor_home();		return;
		case KI_KP8:						return;
		case KI_KP9:						return;
		case KI_KP_PERIOD:					return;
		case KI_KP_DIVIDE:					return;
		case KI_KP_MULTIPLY:					return;
		case KI_KP_MINUS:					return;
		case KI_KP_PLUS:					return;
		case KI_KP_EQUALS:					return;
		}

	switch (ev.ki)
	{
	case KI_LEFT:			Cursor_left();		break;
	case KI_RIGHT:		Cursor_right();		break;
	case KI_HOME:			Cursor_home();		break;
	case KI_END:			Cursor_end();		break;

	case KI_BACKSPACE:            Delete_left();		break;
	case KI_DELETE:		Delete_right();		break;

	case KI_RETURN:		Finish_input();		break;
	case KI_KP_ENTER:		Finish_input();		break;
	default:
		Type_char (ev);
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Type_char (Kbd_event& ev)
{
	assert(Ok());
	bool shift = ev.mod & KM_SHIFT;
	bool up_case = shift != bool (ev.mod & KM_CAPS);
	char c = 0;
	switch (ev.ki)
	{
	case KI_SPACE:		c = ' ';	break;	//space

	case KI_EXCLAIM:		c = '!';	break;	//exclamation mark
	case KI_QUOTEDBL:		c = '"';	break;	//double quote
	case KI_HASH:			c = '#';	break;	//hash
	case KI_DOLLAR:		c = '$';	break;	//dollar
	case KI_AMPERSAND:	c = '&';	break;	//ampersand

	case KI_QUOTE:		c = shift? '"':'\'';	break;	//single quote

	case KI_LEFTPAREN:	c = '(';	break;	//left parenthesis
	case KI_RIGHTPAREN:	c = ')';	break;	//right parenthesis
	case KI_ASTERISK:		c = '*';	break;	//asterisk
	case KI_PLUS:			c = '+';	break;	//plus sign

	case KI_COMMA:		c = shift? '<':',';	break;	//less or comma
	case KI_MINUS:		c = shift? '_':'-';	break;	//undercore or minus sign
	case KI_PERIOD:		c = shift? '>':'.';	break;	//greather or period / full stop
	case KI_SLASH:		c = shift? '?':'/';	break;	//question mark or forward slash
	case KI_0:			c = shift? ')':'0';	break;	//0
	case KI_1:			c = shift? '!':'1';	break;	//1
	case KI_2:			c = shift? '@':'2';	break;	//2
	case KI_3:			c = shift? '#':'3';	break;	//3
	case KI_4:			c = shift? '$':'4';	break;	//4
	case KI_5:			c = shift? '%':'5';	break;	//5
	case KI_6:			c = shift? '^':'6';	break;	//6
	case KI_7:			c = shift? '&':'7';	break;	//7
	case KI_8:			c = shift? '*':'8';	break;	//8
	case KI_9:			c = shift? '(':'9';	break;	//9

	case KI_COLON:		c = ':';	break;	//colon

	case KI_SEMICOLON:	c = shift? ':':';';	break;	//semicolon

	case KI_LESS:			c = '<';	break;	//less-than sign

	case KI_EQUALS:		c = shift? '+':'=';	break;	//equals sign

	case KI_GREATER:		c = '>';	break;	//greater-than sign
	case KI_QUESTION:		c = '?';	break;	//question mark
	case KI_AT:			c = '@';	break;	//at
	
	case KI_LEFTBRACKET:	c = shift? '{':'[';	break;	//left bracket
	case KI_BACKSLASH:	c = shift? '|':'\\';break;	//'or' or backslash
	case KI_RIGHTBRACKET:	c = shift? '}':']';	break;	//right bracket
	
	case KI_CARET:		c = '^';	break;	//caret
	case KI_UNDERSCORE:	c = '_';	break;	//underscore
	
	case KI_BACKQUOTE:	c = up_case? '~':'`';	break;	//tilda or grave
	case KI_a:			c = up_case? 'A':'a';	break;	//a
	case KI_b:			c = up_case? 'B':'b';	break;	//b
	case KI_c:			c = up_case? 'C':'c';	break;	//c
	case KI_d:			c = up_case? 'D':'d';	break;	//d
	case KI_e:			c = up_case? 'E':'e';	break;	//e
	case KI_f:			c = up_case? 'F':'f';	break;	//f
	case KI_g:			c = up_case? 'G':'g';	break;	//g
	case KI_h:			c = up_case? 'H':'h';	break;	//h
	case KI_i:			c = up_case? 'I':'i';	break;	//i
	case KI_j:			c = up_case? 'J':'j';	break;	//j
	case KI_k:			c = up_case? 'K':'k';	break;	//k
	case KI_l:			c = up_case? 'L':'l';	break;	//l
	case KI_m:			c = up_case? 'M':'m';	break;	//m
	case KI_n:			c = up_case? 'N':'n';	break;	//n
	case KI_o:			c = up_case? 'O':'o';	break;	//o
	case KI_p:			c = up_case? 'P':'p';	break;	//p
	case KI_q:			c = up_case? 'Q':'q';	break;	//q
	case KI_r:			c = up_case? 'R':'r';	break;	//r
	case KI_s:			c = up_case? 'S':'s';	break;	//s
	case KI_t:			c = up_case? 'T':'t';	break;	//t
	case KI_u:			c = up_case? 'U':'u';	break;	//u
	case KI_v:			c = up_case? 'V':'v';	break;	//v
	case KI_w:			c = up_case? 'W':'w';	break;	//w
	case KI_x:			c = up_case? 'X':'x';	break;	//x
	case KI_y:			c = up_case? 'Y':'y';	break;	//y
	case KI_z:			c = up_case? 'Z':'z';	break;	//z

	case KI_KP0:			c = '0';	break;	//0
	case KI_KP1:			c = '1';	break;	//1
	case KI_KP2:			c = '2';	break;	//2
	case KI_KP3:			c = '3';	break;	//3
	case KI_KP4:			c = '4';	break;	//4
	case KI_KP5:			c = '5';	break;	//5
	case KI_KP6:			c = '6';	break;	//6
	case KI_KP7:			c = '7';	break;	//7
	case KI_KP8:			c = '8';	break;	//8
	case KI_KP9:			c = '9';	break;	//9
	case KI_KP_PERIOD:	c = '.';	break;	//keypad period
	case KI_KP_DIVIDE:	c = '/';	break;	//keypad divide
	case KI_KP_MULTIPLY:	c = '*';	break;	//keypad multiply
	case KI_KP_MINUS:		c = '-';	break;	//keypad minus
	case KI_KP_PLUS:		c = '+';	break;	//keypad plus
	case KI_KP_EQUALS:	c = '=';	break;	//keypad equals
	}
	if (c) Insert_symbol (c);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Insert_symbol (char c)
{
	assert(Ok());
	data.insert (cursor_pos, 1, c);
	Cursor_right();
}
//--------------------------------------------------------------------------------------------------
Line_edit::~Line_edit()
{
	if (on_enter) delete on_enter;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Finish_input ()
{
	assert(Ok());
	cursor_pos = 0;
	start_view = 0;
	(*on_enter)(&data);
	data.clear();
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Cursor_left()
{
	assert(Ok());
	if (cursor_pos > 0)
	{
		cursor_pos--;
		if (cursor_pos < start_view) start_view = cursor_pos;
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Cursor_right()
{
	assert(Ok());
	if (cursor_pos < data.size())
	{
		cursor_pos++;
		while (Cursor_offset() > borders.w) start_view++;
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Cursor_home()
{
	assert(Ok());
	cursor_pos = 0;
	if (cursor_pos < start_view) start_view = cursor_pos;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Cursor_end()
{
	assert(Ok());
	cursor_pos = data.size();
	while (Cursor_offset() > borders.w) start_view++;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Delete_left()
{
	assert(Ok());
	if (cursor_pos > 0)
	{
		cursor_pos --;
		data.erase (cursor_pos, 1);
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Delete_right()
{
	assert(Ok());
	if (cursor_pos < data.size()) data.erase (cursor_pos, 1);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Draw (Canvas* c) const
{
	assert(Ok());
	borders.Draw (c, font.bg);
	Draw_text (c);
	Draw_cursor (c);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Draw_text (Canvas* c) const
{
	assert(Ok());
	Rect brd = borders;
	greeting.Draw (c, &brd);

	brd = borders;
	brd.Cut_left (grsize);
	data.Draw (c, &brd, start_view);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::Draw_cursor (Canvas* c) const
{
	assert(Ok());
	Rect cursor = borders;
	cursor.x += Cursor_offset() - cursor_width/2;
	cursor.w = cursor_width;

	Color colour = 0;
	Uint32 time = MSECS - last_actiont;
	if ((time/blink_interval) % 2) colour = font.bg;
	else colour = font.fg;

	cursor.Draw (c, colour);
}
//--------------------------------------------------------------------------------------------------
int Line_edit::Cursor_offset() const
{
	assert(Ok());
	string before = data.substr (start_view, cursor_pos - start_view);
	return grsize + font.Str_len (before.c_str());
}
//--------------------------------------------------------------------------------------------------
bool Line_edit::Ok() const
{
	return 0 <= cursor_pos && cursor_pos <= data.size() && 0 <= start_view && start_view <= data.size() &&
		data.Ok() && greeting.Ok() && grsize >= 0 && on_enter != 0;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::Init (const Rect& brd)
{
	assert(Ok());
	borders = brd;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::Push_string (const string &what)
{
	assert(Ok());
	data.push_back (Stringc (what, font));
}
//--------------------------------------------------------------------------------------------------
int Lines_view::Get_height (const Stringc& what) const
{
	assert(Ok());
	Point size = what.pSize();
	float nlines = float(size.x)/borders.w;

	if (nlines - float(int(nlines)) > 0.01) nlines += 1;//for tail

	int num_lines = int (nlines);
	if (num_lines < 1) num_lines = 1;
	return size.y*num_lines;
}
//--------------------------------------------------------------------------------------------------
int Lines_view::Draw_tolerable_line (Canvas* screen, const Stringc& text, int offset, Rect brd) const
{
	assert(Ok());
	Stringc buf = text.Get_bordered_substring (brd.w, offset);
	if (buf.Draw (screen, &brd))
		return buf.size();
	return false;
}
//--------------------------------------------------------------------------------------------------
int Lines_view::Draw_text (Canvas* screen, const Stringc& text, int start_offset) const
{
	assert(Ok());
	int result_height = 0;
	int total_symbols = text.size();
	int symbols_showed = 0;

	Rect brd = borders;
	brd.Cut_top (start_offset);

	int fheight = font.Height ();

	while (symbols_showed < total_symbols)
	{
		int num_sym_in_line = Draw_tolerable_line (screen, text, symbols_showed, brd);

		if (num_sym_in_line == 0) return false;

		symbols_showed += num_sym_in_line;
		result_height += fheight;

		if (brd.h < fheight) break;
		brd.Cut_top (fheight);
	}
    return result_height;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::Draw (Canvas* c) const
{
	assert(Ok());
	int start_y = 0;
	list <Stringc>::const_iterator start = data.begin();
	int height = 0;
	bool lesser = true;

	for (list <Stringc>::const_reverse_iterator i = data.rbegin(); i != data.rend(); ++i)
	{
		height += Get_height (*i);
		if (height > borders.h)
		{
			start_y = height - borders.h;
			start = (++i).base();
			lesser = false;
			break;
		}
	}
	if (lesser) start_y = height - borders.h;//snuggle bottom

	borders.Draw (c, font.bg);

	int cur_height = 0;
	int total_height = -start_y;
	for (list <Stringc>::const_iterator i = start; i != data.end(); ++i)
	{
		cur_height = Draw_text (c, *i, total_height);
		if (cur_height == 0 && i->size() > 0) return;//!!! Error
		total_height += cur_height;
	}

}
//--------------------------------------------------------------------------------------------------
bool Lines_view::Ok() const
{
	for (list<Stringc>::const_iterator i = data.begin(); i != data.end(); ++i)
		if (!i->Ok ()) return false;
	return font.Ok();
}
//--------------------------------------------------------------------------------------------------
Stringc Stringc::Get_bordered_substring (int width, int offset) const
{
	int ret = font.Approximate_num_symbols (width);
	if (0 >= ret || ret > size()) ret = size();
	if (ret > 500) ret = 500;

	char buf[512];
	strncpy (buf, data() + offset, ret);
	buf[ret] = 0;

	int w = font.Str_len (buf);

	while (w > width)
	{
		float std_w = (float)(w)/ret;
		ret -= (w - width)/std_w + 0.5;
		ret -= ret%2; //for double char symbols
		buf[ret] = 0;//cut string
		w = font.Str_len (buf);
	}
	return Stringc (buf, font);
}
//--------------------------------------------------------------------------------------------------