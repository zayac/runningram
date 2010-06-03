/* 
 * File:   Button.cpp
 * Author: necto
 * 
 * Created on June 3, 2010, 8:16 PM
 */

#include "Button.h"
#include "Game_manager.h"
#include "Eventman.h"

Button::Button()
{
}

Button::Button (const Button& orig) { }

Button::~Button ()
{
	if (on_press) delete on_press;
}

void Button::init (Eventman* eve, Functor* on_pr)
{
	on_press = on_pr;
	rest_img = Canvas ("textures/blueexit.jpg");
	active_img = Canvas ("textures/greenexit.jpg");
	pushed_img = Canvas ("textures/redexit.jpg");
	brd = rest_img.getClipRect();
	brd.move (Point(500, 432));
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onMove), brd);
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onPress),
							 Mouse_btn_event (MBTN_LEFT, brd));
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onRelease),
							 Mouse_btn_event (MBTN_LEFT, brd, MBTN_RELEASED));
	eve->registerMouseAction (new Arg_Method <void, Point, Button> (this, &Button::onAway),
							 Mouse_btn_event (MBTN_AWAY, brd, 0));
}

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