/* 
 * File:   Mouse_event.h
 * Author: necto
 *
 * Created on May 28, 2010, 2:37 PM
 */

#ifndef _MOUSE_EVENT_H
#define	_MOUSE_EVENT_H
#include "mstdint.h"
#include "Rect.h"

enum Mouse_btn_masks
{
	MBTN_LEFT_MASK =	0b00000001,
	MBTN_MIDDLE_MASK =	0b00000010,
	MBTN_RIGHT_MASK =	0b00000100,
	MBTN_WHUP_MASK =	0b00001000,
	MBTN_WHDOWN_MASK =	0b00010000,
	MBTN_X1_MASK =		0b01000000,
	MBTN_X2_MASK =		0b10000000
};

enum Mouse_btns
{
	MBTN_LEFT =			1,
	MBTN_MIDDLE =		2,
	MBTN_RIGHT =		3,
	MBTN_WHEEL_UP =		4,
	MBTN_WHEEL_DOWN =	5,
	MBTN_X1 =			6,
	MBTN_X2 =			7
};

enum Mbtn_events
{
	MBTN_PRESSED = 0,
	MBTN_RELEASED = 1
};
//==================================================================================
struct Mouse_btn
{
	Uint8 button;	/**< The mouse button index */
	Uint8 state;	/**< MBTN_PRESSED or MBTN_RELEASED */
	Point pos;		/**< The X/Y coordinates of the mouse at press time */

	Mouse_btn (Uint8 but, Uint8 st, Point pos_)
	:button(but), state(st), pos (pos_)	{}
	Mouse_btn (const Mouse_btn& orig)
	:button (orig.button), state (orig.state), pos (orig.pos)	{}
#ifdef _SDL_events_h
	Mouse_btn (const SDL_MouseButtonEvent& orig)
	:button (orig.button), state (orig.state), pos (orig.x, orig.y)	{}
#endif
};
//==================================================================================
struct Mouse_move
{
	Uint8 state;	/**< The current button state */
	Point pos;		/**< The X/Y coordinates of the mouse */
	Point rel;		/**< The relative motion in the X/Y direction */

	Mouse_move (Uint8 st, Point pos_, Point rel_)
	:state (st), pos (pos_), rel (rel_)	{}
	Mouse_move (const Mouse_move& orig)
	:state (orig.state), pos (orig.pos), rel (orig.rel)	{}

#ifdef _SDL_events_h
	Mouse_move (const SDL_MouseMotionEvent& orig)
	:state (orig.state), pos (orig.x, orig.y), rel (orig.xrel, orig.yrel)	{}
#endif
};
//==================================================================================

struct Mouse_move_event
{
	Rect brd;
	Uint8 state;

	Mouse_move_event (Rect brd_ = Rect(), Uint8 st = 0) :brd (brd_), state (st) {}
	Mouse_move_event (const Mouse_move_event& orig):brd (orig.brd), state (orig.state){}

	inline bool Contain (const Mouse_move& ev)
	{
		return ((state&ev.state) == state) &&
			((brd.w == 0 && brd.h == 0) || brd.Contain (ev.pos));
	}
};
//==================================================================================
struct Mouse_btn_event
{
	Rect brd;
	Uint8 button;
	Uint8 state;

	Mouse_btn_event (Uint8 but, Rect brd_ = Rect(), Uint8 st = MBTN_PRESSED)
	:brd (brd_), button (but), state (st) {}
	Mouse_btn_event (const Mouse_btn_event& orig)
	:brd (orig.brd), button (orig.button), state (orig.state){}

	inline bool Contain (const Mouse_btn& ev)
	{
		return ev.button == button && ev.state == state &&
			((brd.w == 0 && brd.h == 0) || brd.Contain (ev.pos));
	}
};
//==================================================================================

#endif	/* _MOUSE_EVENT_H */

