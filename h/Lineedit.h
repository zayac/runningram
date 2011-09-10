/*
 * Lineedit.h
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#ifndef LINEEDIT_H_
#define LINEEDIT_H_


const int cursor_width = 2;
const int blink_interval = 300;

#include "Functor.h"
#include "Stringc.h"
#include "Key_event.h"

class LineEdit
{
	Fontc& font;
	Stringc greeting;
	int grsize;
	Stringc data;
	int cursor_pos;
	int start_view;
	Rect borders;
	Arg_Functor <void, const string&> *on_enter;
	Functor* onUp;
	Functor* onDown;

	Uint32 last_actiont;

	void drawText (Canvas* c) const;
	void drawCursor (Canvas* c) const;

	void processUp();
	void processDown();

	void cursorLeft();
	void cursorRight();
	void cursorHome();
	void cursorEnd();
	void correctCursor();
	void deleteLeft();
	void deleteRight();
	void finishInput();
	void typeChar (Kbd_event& ev);
	void insertSymbol (char);

	int cursorOffset() const;

public:
	LineEdit (Fontc& f) :font(f), on_enter(0){}
	~LineEdit();

	const string& getGreeting() const {return greeting;}

	void init (const Rect& brd, Arg_Functor <void, const string&> *on_enter, string greeting = "");
	inline void setOnUp (Functor* on_up) {onUp = on_up;}
	inline void setOnDown (Functor* on_down) {onDown = on_down;}
	void setContent (const string& text);

	void operate (Kbd_event ev);
	void draw (Canvas* c) const;

	bool ok() const;
};


#endif /* LINEEDIT_H_ */
