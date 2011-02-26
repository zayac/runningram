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

//extern "C"
//{
//	#include "lua.h"
//	#include "lauxlib.h"
//	#include "lualib.h"
//}

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

#define MSECS (clock()*1000/CLOCKS_PER_SEC)


//!!! only for debug
Console* interface = 0;

//int Lua_execute (lua_State* ls)
//{
//	typedef Arg_Functor <int, lua_State*> Tfun;
//	Tfun* function = (Tfun*)(long)lua_tonumber(ls, lua_upvalueindex(1));
//	if (function)
//		return (*function)(ls);
//	lua_error (ls);
//	return 0;
//}

Console::Console ()
:history (printFont, 50), input (cmdFont)
{
    //!!! only for debug
    interface = this;
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
//	font.Open_font (parser->font_fname.c_str(), parser->height);

//	font.Set_fg (Color(10, 200, 20));
//	font.Set_bg (Color(10, 20, 20));

	Rect borders = c->getScreen()->getClipRect();
	borders.h = 210;
	history.init (borders);
	history.pushString ("You are welcome!");

	borders.y = borders.h;
	borders.h = printFont.height();
	input.init  (borders, new Arg_Method<void, const string&, Console> (this, &Console::onEnterString), "You:>");

	
	history.captureLastString();
	interp = interp_;
	interp->regOutput (new Arg_Method<void, const string&, Lines_view>
								(&history, &Lines_view::changeCurrentString),
					   new Arg_Method<void, void, Lines_view>
								(&history, &Lines_view::copyCurrentString));

	enabled = false;

	assert(ok());

	return true;
}
//--------------------------------------------------------------------------------------------------
void Console::cleanup()
{
	assert(ok());//!!! It may be needed to close opend font
        FontcCleanUp();
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
void Line_edit::init (const Rect& brd, Arg_Functor <void, const string&> *enter, string gr)
{
	on_enter = enter;
	data = "";
	data.setFont (font);
	cursor_pos = 0;
	start_view = 0;
	borders = brd;
	last_actiont = MSECS;
	greeting = gr;
	greeting.setFont (font);
	grsize = greeting.width();
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
void Line_edit::operate (Kbd_event ev)
{
	assert(ok());
	if (ev.type == EV_KEYUP) return;
	if (ev.ki == KI_BACKQUOTE) return; //!!! it's ugly. this key switches console

	last_actiont = MSECS;

	bool num_lock = ev.mod & KM_NUM;

	if (!num_lock)
		switch (ev.ki)
		{
		case KI_KP1:			cursorEnd();	return;
		case KI_KP2:							return;
		case KI_KP3:							return;
		case KI_KP4:			cursorLeft();	return;
		case KI_KP5:							return;
		case KI_KP6:			cursorRight();	return;
		case KI_KP7:			cursorHome();	return;
		case KI_KP8:							return;
		case KI_KP9:							return;
		case KI_KP_PERIOD:						return;
		case KI_KP_DIVIDE:						return;
		case KI_KP_MULTIPLY:					return;
		case KI_KP_MINUS:						return;
		case KI_KP_PLUS:						return;
		case KI_KP_EQUALS:						return;
		}

	switch (ev.ki)
	{
	case KI_LEFT:			cursorLeft();		break;
	case KI_RIGHT:			cursorRight();		break;
	case KI_HOME:			cursorHome();		break;
	case KI_END:			cursorEnd();		break;

	case KI_BACKSPACE:		deleteLeft();		break;
	case KI_DELETE:			deleteRight();		break;

	case KI_RETURN:			finishInput();		break;
	case KI_KP_ENTER:		finishInput();		break;
	default:
		typeChar (ev);
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::typeChar (Kbd_event& ev)
{
	assert(ok());
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
	if (c) insertSymbol (c);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::insertSymbol (char c)
{
	assert(ok());
	data.insert (cursor_pos, 1, c);
	cursorRight();
}
//--------------------------------------------------------------------------------------------------
Line_edit::~Line_edit()
{
	if (on_enter) delete on_enter;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::finishInput()
{
	assert(ok());
	cursor_pos = 0;
	start_view = 0;
	(*on_enter)(data);
	data.clear();
}
//--------------------------------------------------------------------------------------------------
void Line_edit::cursorLeft()
{
	assert(ok());
	if (cursor_pos > 0)
	{
		cursor_pos--;
		if (cursor_pos < start_view) start_view = cursor_pos;
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::cursorRight()
{
	assert(ok());
	if (cursor_pos < data.size())
	{
		cursor_pos++;
		while (cursorOffset() > borders.w) start_view++;
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::cursorHome()
{
	assert(ok());
	cursor_pos = 0;
	if (cursor_pos < start_view) start_view = cursor_pos;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::cursorEnd()
{
	assert(ok());
	cursor_pos = data.size();
	while (cursorOffset() > borders.w) start_view++;
}
//--------------------------------------------------------------------------------------------------
void Line_edit::deleteLeft()
{
	assert(ok());
	if (cursor_pos > 0)
	{
		cursor_pos --;
		data.erase (cursor_pos, 1);
	}
}
//--------------------------------------------------------------------------------------------------
void Line_edit::deleteRight()
{
	assert(ok());
	if (cursor_pos < data.size()) data.erase (cursor_pos, 1);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::draw (Canvas* c) const
{
	assert(ok());
	c->fillRect (borders, font.bg);
	drawText (c);
	drawCursor (c);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::drawText (Canvas* c) const
{
	assert(ok());
	Rect brd = borders;
	greeting.draw (c, &brd);

	brd = borders;
	brd.cutLeft (grsize);
	data.draw (c, &brd, start_view);
}
//--------------------------------------------------------------------------------------------------
void Line_edit::drawCursor (Canvas* c) const
{
	assert(ok());
	Rect cursor = borders;
	cursor.x += cursorOffset() - cursor_width/2;
	cursor.w = cursor_width;

	Color colour = 0;
	Uint32 time = MSECS - last_actiont;
	if ((time/blink_interval) % 2) colour = font.bg;
	else colour = font.fg;

	c->fillRect (cursor, colour);
}
//--------------------------------------------------------------------------------------------------
int Line_edit::cursorOffset() const
{
	assert(ok());
	string before = data.substr (start_view, cursor_pos - start_view);
	return grsize + font.strLen (before.c_str());
}
//--------------------------------------------------------------------------------------------------
bool Line_edit::ok() const
{
	return 0 <= cursor_pos && cursor_pos <= data.size() && 0 <= start_view && start_view <= data.size() &&
		data.ok() && greeting.ok() && grsize >= 0 && on_enter != 0;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::init (const Rect& brd)
{
	assert(ok());
	borders = brd;
	last_string_is_captured = false;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::pushString (Stringc what)
{
	assert(ok());
	if (last_string_is_captured)
	{
		Stringc current = data.back();
		data.pop_back();
		data.push_back (what);//don't understand why, but if i give a
		data.push_back (current);
	}
	else
		data.push_back (what);// reference, i receive an error, connected with font
	if (data.size () > max_lines)
		data.erase (data.begin());
}
//--------------------------------------------------------------------------------------------------
void Lines_view::pushString (const string& str)
{
	pushString (Stringc (str, fontDefault));
}
//--------------------------------------------------------------------------------------------------
void Lines_view::captureLastString()
{
	last_string_is_captured = true;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::changeCurrentStringc (Stringc what)
{
	assert(ok());
	data.pop_back();
	data.push_back (Stringc(what));
	last_string_is_captured = true;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::changeCurrentString (const string& str)
{
	changeCurrentStringc (Stringc (str, fontDefault));
}
//--------------------------------------------------------------------------------------------------
void Lines_view::releaseCurrentString()
{
	last_string_is_captured = false;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::copyCurrentString()
{
	if (data.size() > 0)
		pushString (data.back());
}
//--------------------------------------------------------------------------------------------------
//int Lines_view::getHeight (const Stringc& what) const
//{
//	assert(ok());
//	Point size = what.pSize();
//	float nlines = float(size.x)/borders.w;
//
//	if (nlines - float(int(nlines)) > 0.01) nlines += 1;//for tail
//
//	int num_lines = int (nlines);
//	if (num_lines < 1) num_lines = 1;
//	return size.y*num_lines;
//}
//--------------------------------------------------------------------------------------------------
int Lines_view::Draw_tolerable_line (Canvas* screen, const Stringc& text, int offset, Rect brd) const
{
	assert(ok());
	Stringc buf = text.getBorderedSubstring (brd.w, offset);
	if (buf.draw (screen, &brd))
		return buf.size();
	return false;
}
//--------------------------------------------------------------------------------------------------
int Lines_view::drawText (Canvas* screen, const Stringc& text, int start_offset) const
{
	assert(ok());
	int result_height = 0;
	int total_symbols = text.size();
	int symbols_showed = 0;

	Rect brd = borders;
	brd.cutTop (start_offset);

	int fheight = fontDefault.height ();

	while (symbols_showed < total_symbols)
	{
		int num_sym_in_line = Draw_tolerable_line (screen, text, symbols_showed, brd);

		if (num_sym_in_line == 0) return false;

		symbols_showed += num_sym_in_line;
		result_height += fheight;

		if (brd.h < fheight) break;
		brd.cutTop (fheight);
	}
    return result_height;
}
//--------------------------------------------------------------------------------------------------
void Lines_view::draw (Canvas* c) const
{
	assert(ok());
	int start_y = 0;
	list <Stringc>::const_iterator start = data.begin();
	int height = 0;
	bool lesser = true;

	for (list <Stringc>::const_reverse_iterator i = data.rbegin(); i != data.rend(); ++i)
	{
		if (i->empty()) continue;
		height += i->getFullHeight (borders.w);
		if (height > borders.h)
		{
			start_y = height - borders.h;
			start = (++i).base();
			lesser = false;
			break;
		}
	}
	if (lesser) start_y = height - borders.h;//snuggle bottom

	c->fillRect (borders, fontDefault.bg);
	int cur_height = 0;
	int total_height = -start_y;
	for (list <Stringc>::const_iterator i = start; i != data.end(); ++i)
	{
		if (i->empty()) continue;
		cur_height = drawText (c, *i, total_height);
		if (cur_height == 0 && i->size() > 0) return;//!!! Error
		total_height += cur_height;
	}

}
//--------------------------------------------------------------------------------------------------
bool Lines_view::ok() const
{
#ifdef FULL_DEBUG
	for (list<Stringc>::const_iterator i = data.begin(); i != data.end(); ++i)
		if (!i->Ok ()) return false;
#endif
	return fontDefault.ok();
}
//--------------------------------------------------------------------------------------------------
void Stringc::updateLab()
{
	lab = font.createLabel (c_str(), false);
	lab_upd = true;
}
//--------------------------------------------------------------------------------------------------
int Stringc::getFullHeight (Uint16 max_width) const
{
	assert(ok());
	Point size = pSize();
	float nlines = float(size.x)/max_width;

	if (nlines - float(int(nlines)) > 0.01) nlines += 1;//for tail

	int num_lines = int (nlines);
	if (num_lines < 1) num_lines = 1;
	return size.y*num_lines;
}
//--------------------------------------------------------------------------------------------------
Stringc Stringc::getBorderedSubstring (int width, int offset) const
{
	int ret = font.approximateNumSymbols (width);
	if (0 >= ret || ret > size()) ret = size();
	if (ret > 500) ret = 500;

	char buf[512];
	strncpy (buf, data() + offset, ret);
	buf[ret] = 0;

	int w = font.strLen (buf);

	while (w > width)
	{
		float std_w = (float)(w)/ret;
		ret -= (w - width)/std_w + 0.5;
		ret -= ret%2; //for double char symbols
		buf[ret] = 0;//cut string
		w = font.strLen (buf);
	}
	return Stringc (buf, font);
}
//--------------------------------------------------------------------------------------------------