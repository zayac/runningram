/* 
 * File:   Button.h
 * Author: necto
 *
 * Created on June 3, 2010, 8:16 PM
 */

#ifndef _BUTTON_H
#define	_BUTTON_H

#include "Canvas.h"
#include "Functor.h"
class Eventman;

class Button
{
	Rect brd;
	Canvas rest_img;
	Canvas active_img;
	Canvas pushed_img;
	Functor* on_press;

	enum State
	{
		REST,
		ACTIVE,
		PUSHED
	} st;

public:
	Button();
	Button(const Button& orig);
	virtual ~Button();
	
	void init (Eventman* eve, Functor* on_press);

	void onAway (Point p)
	{
		st = REST;
	}
	void onMove (Point p)
	{
		if (st != PUSHED) st = ACTIVE;
	}
	void onPress (Point p)
	{
		st = PUSHED;
	}
	void onRelease (Point p)
	{
		st = ACTIVE;
		if (on_press) (*on_press)();
	}

	void draw (Canvas c) const;
};

#endif	/* _BUTTON_H */

