/* 
 * File:   MouseTargeter.cpp
 * Author: necto
 * 
 * Created on February 27, 2011, 11:00 AM
 */

#include "MouseTargeter.h"
#include "GUEventman.h"
#include "Graphic_subsystem.h"
#include "Car.h"
#include "Timer.h"


// <editor-fold defaultstate="collapsed" desc="From file Initializer">


class MouseTargeter::Initializer : public Serializator
{
public:
	static GUEventman* evman;
	static Timer* time;
	static const Graphic_subsystem* graphics;

protected:
	virtual bool readFrag (ifstream &file) { return true;}
public:

	Initializer (const char* name)
	: Serializator (name){}

	MouseTargeter* createMouseTargeter() const
	{
		return new MouseTargeter (evman, time, graphics->getCenter());
	}
};

GUEventman* MouseTargeter::Initializer::evman = 0;
Timer* MouseTargeter::Initializer::time = 0;
const Graphic_subsystem* MouseTargeter::Initializer::graphics = 0;

//--------------------------------------------------------------------------------------------------
class MouseTargeter::ParserHolder :public Control::ParserContainer
{
	MouseTargeter::Initializer parser;
public:
	ParserHolder (const char* name) :parser (name){}

	virtual Serializator* getParser()
	{
		return &parser;
	}
	virtual Control* createControl()
	{
		return parser.createMouseTargeter();
	}
};

// </editor-fold>

MouseTargeter::MouseTargeter (GUEventman* evman_, Timer* time_, Point center_)
:evman (evman_), time (time_), center (center_), target(0), error_sum(0)
{
}
//--------------------------------------------------------------------------------------------------
MouseTargeter::~MouseTargeter() {
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::mLUpHandler (Point)
{
	mKeysPressed &= ~LEFT;
	upRelease();
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::mLDownHandler (Point)
{
	mKeysPressed |= LEFT;
	upPress();
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::mRUpHandler (Point)
{
	mKeysPressed &= ~RIGHT;
	downRelease();
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::mRDownHandler (Point)
{
	mKeysPressed |= RIGHT;
	downPress();
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::mMoveHandler (Point pos)
{
	target = Orient((pos - center).to<float>()).getAngle() - PI/4;
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::directionUpdate()
{
	Orient current = object->getOri();
	float delta = inRange (current.getAngle() - target);
	error_sum += delta;

	if (error_sum < -infelicity)
	{
		rightPress();
		leftRelease();
		error_sum = 0;
	}
	else if (error_sum > infelicity)
	{
		rightRelease();
		leftPress();
		error_sum = 0;
	}
	else
	{
		rightRelease();
		leftRelease();
	}
}
//--------------------------------------------------------------------------------------------------
void MouseTargeter::setControl (Car* obj_)
{
	Control::setControl (obj_);
	time->registerPeriodicEvent (10, new Arg_Method<void, void, MouseTargeter>
											(this, &MouseTargeter::directionUpdate));

	evman->registerMouseAction (new Arg_Method <void, Point, MouseTargeter>
									(this, &MouseTargeter::mLDownHandler),
								Mouse_btn_event (MBTN_LEFT, Rect(), MBTN_PRESSED));
	evman->registerMouseAction (new Arg_Method <void, Point, MouseTargeter>
									(this, &MouseTargeter::mLUpHandler),
								Mouse_btn_event (MBTN_LEFT, Rect(), MBTN_RELEASED));
	evman->registerMouseAction (new Arg_Method <void, Point, MouseTargeter>
									(this, &MouseTargeter::mRDownHandler),
								Mouse_btn_event (MBTN_RIGHT, Rect(), MBTN_PRESSED));
	evman->registerMouseAction (new Arg_Method <void, Point, MouseTargeter>
									(this, &MouseTargeter::mRUpHandler),
								Mouse_btn_event (MBTN_RIGHT, Rect(), MBTN_RELEASED));

	evman->registerMouseAction (new Arg_Method <void, Point, MouseTargeter>
									(this, &MouseTargeter::mMoveHandler),
								Mouse_move_event());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void MouseTargeter::preInit (GUEventman* evman, Timer* time, const Graphic_subsystem* graphics)
{
	MouseTargeter::Initializer::evman = evman;
	MouseTargeter::Initializer::time = time;
	MouseTargeter::Initializer::graphics = graphics;
	Control::registerParser (new MouseTargeter::ParserHolder ("mouse"));
}
//--------------------------------------------------------------------------------------------------

