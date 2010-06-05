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
	Button ();
	virtual ~Button();
	
	void init (Eventman* eve, Functor* on_press, Point pos);

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
		if (st == PUSHED && on_press) (*on_press)();
		st = ACTIVE;
	}

	void draw (Canvas c) const;
};

class Btnlist :private list <Button>
{
	Eventman* sense;

public:
	void init (Eventman* eve);
	void addButton (Functor* on_press, Point pos);

	void draw (Canvas c) const;
};

#endif	/* _BUTTON_H */

