/*
 * Lineedit.cpp
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#include "Lineedit.h"


#define MSECS (clock()*1000/CLOCKS_PER_SEC)

//--------------------------------------------------------------------------------------------------
void LineEdit::init (const Rect& brd, Arg_Functor <void, const string&> *enter, string gr)
{
	onUp = 0;
	onDown = 0;
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
	greeting.updateLab();
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
void LineEdit::setContent (const string& text)
{
	assert(ok());
	data = text;
	correctCursor();
}
//--------------------------------------------------------------------------------------------------
void LineEdit::operate (Kbd_event ev)
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
		case KI_KP2:			processDown();	return;
		case KI_KP3:							return;
		case KI_KP4:			cursorLeft();	return;
		case KI_KP5:							return;
		case KI_KP6:			cursorRight();	return;
		case KI_KP7:			cursorHome();	return;
		case KI_KP8:			processUp();	return;
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
		typeChar (ev);							break;
	}
	data.updateLab();//!!! work around, must be shadowed
}
//--------------------------------------------------------------------------------------------------
void LineEdit::typeChar (Kbd_event& ev)
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
void LineEdit::insertSymbol (char c)
{
	assert(ok());
	data.insert (cursor_pos, 1, c);
	cursorRight();
}
//--------------------------------------------------------------------------------------------------
LineEdit::~LineEdit()
{
	if (on_enter) delete on_enter;
}
//--------------------------------------------------------------------------------------------------
void LineEdit::finishInput()
{
	assert(ok());
	cursor_pos = 0;
	start_view = 0;
	(*on_enter)(data);
	data.clear();
}
//--------------------------------------------------------------------------------------------------
void LineEdit::processUp()
{
	assert(ok());
	if (onUp) (*onUp)();
}
//--------------------------------------------------------------------------------------------------
void LineEdit::processDown()
{
	assert(ok());
	if (onDown) (*onDown)();
}
//--------------------------------------------------------------------------------------------------
void LineEdit::cursorLeft()
{
	assert(ok());
	if (cursor_pos > 0)
	{
		cursor_pos--;
		if (cursor_pos < start_view) start_view = cursor_pos;
	}
}
//--------------------------------------------------------------------------------------------------
void LineEdit::cursorRight()
{
	assert(ok());
	if (cursor_pos < data.length())
	{
		cursor_pos++;
		while (cursorOffset() > borders.w) start_view++;
	}
}
//--------------------------------------------------------------------------------------------------
void LineEdit::cursorHome()
{
	assert(ok());
	cursor_pos = 0;
	if (cursor_pos < start_view) start_view = cursor_pos;
}
//--------------------------------------------------------------------------------------------------
void LineEdit::cursorEnd()
{
	assert(ok());
	cursor_pos = data.length();
	while (cursorOffset() > borders.w) start_view++;
}
//--------------------------------------------------------------------------------------------------
void LineEdit::correctCursor()
{
	if (cursor_pos > data.length()) cursor_pos = data.length();
	if (cursor_pos < 0) 			cursor_pos = 0;
	if (cursor_pos < start_view) 	start_view = cursor_pos;

	while (cursorOffset() > borders.w) start_view++;
	assert(ok());
}
//--------------------------------------------------------------------------------------------------
void LineEdit::deleteLeft()
{
	assert(ok());
	if (cursor_pos > 0)
	{
		cursor_pos --;
		data.erase (cursor_pos, 1);
	}
}
//--------------------------------------------------------------------------------------------------
void LineEdit::deleteRight()
{
	assert(ok());
	if (cursor_pos < data.length()) data.erase (cursor_pos, 1);
}
//--------------------------------------------------------------------------------------------------
void LineEdit::draw (Canvas* c) const
{
	assert(ok());
	c->fillRect (borders, font.bg);
	drawText (c);
	drawCursor (c);
}
//--------------------------------------------------------------------------------------------------
void LineEdit::drawText (Canvas* c) const
{
	assert(ok());
	Rect brd = borders;
	greeting.draw (c, &brd);

	brd = borders;
	brd.cutLeft (grsize);
	data.draw (c, &brd, start_view);
}
//--------------------------------------------------------------------------------------------------
void LineEdit::drawCursor (Canvas* c) const
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
int LineEdit::cursorOffset() const
{
	assert(ok());
	string before = data.substr (start_view, cursor_pos - start_view);
	return grsize + font.strLen (before.c_str());
}
//--------------------------------------------------------------------------------------------------
bool LineEdit::ok() const
{
	return 0 <= cursor_pos && cursor_pos <= data.length() &&
		   0 <= start_view && start_view <= data.length() &&
		   data.ok() && greeting.ok() && grsize >= 0 && on_enter != 0;
}
//--------------------------------------------------------------------------------------------------

