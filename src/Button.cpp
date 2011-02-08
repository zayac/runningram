/* 
 * File:   Button.cpp
 * Author: necto
 * 
 * Created on June 3, 2010, 8:16 PM
 */

#include "Button.h"
#include "Game_manager.h"
#include "GUEventman.h"

Button::Button() :on_press(0)
{
}
//--------------------------------------------------------------------------------------------------
Button::~Button ()
{
	if (on_press) delete on_press;
}
//--------------------------------------------------------------------------------------------------
void Button::init (GUEventman* eve, Functor* on_pr, Point pos)
{
	on_press = on_pr;
	rest_img = Canvas ("textures/blueexit.jpg");
	active_img = Canvas ("textures/greenexit.jpg");
	pushed_img = Canvas ("textures/redexit.jpg");
	brd = rest_img.getClipRect();
	brd.move (pos);
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onMove), brd);
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onPress),
							 Mouse_btn_event (MBTN_LEFT, brd));
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onRelease),
							 Mouse_btn_event (MBTN_LEFT, brd, MBTN_RELEASED));
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onAway),
							 Mouse_btn_event (MBTN_AWAY, brd, 0));
	st=REST;
}
//--------------------------------------------------------------------------------------------------
void Button::draw (Canvas c) const
{
	switch (st)
	{
	case REST:
		rest_img.draw (c, brd.getLUp() + c.getPos ());
		break;
	case ACTIVE:
		active_img.draw (c, brd.getLUp() + c.getPos ());
		break;
	case PUSHED:
		pushed_img.draw (c, brd.getLUp() + c.getPos ());
		break;
	}
}
//--------------------------------------------------------------------------------------------------
void Btnlist::init (GUEventman* eve)
{
	sense = eve;
}
//--------------------------------------------------------------------------------------------------
void Btnlist::addButton (Functor* on_press, Point pos)
{
	push_front (Button());
	begin()->init (sense, on_press, pos);
}
//--------------------------------------------------------------------------------------------------
void Btnlist::draw (Canvas cancan) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		i->draw (cancan);
}
//--------------------------------------------------------------------------------------------------
