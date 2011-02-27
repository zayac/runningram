/* 
 * File:   MouseTargeter.h
 * Author: necto
 *
 * Created on February 27, 2011, 11:00 AM
 */

#ifndef MOUSETARGETER_H
#define	MOUSETARGETER_H

#include "Control.h"
#include "Vec.h"
#include "Orient.h"
#include "GUEventman.h"

class Graphic_subsystem;
class GUEventman;
class Timer;

class MouseTargeter :public Control
{
	enum MKeys
	{
		LEFT =	1,
		RIGHT =	2
	};

	float target;
	float error_sum;
	Point center;
	int mKeysPressed;
	GUEventman* evman;
	Timer* time;

	class Initializer;
	class ParserHolder;

public:
	void mLUpHandler (Point);
	void mLDownHandler (Point);
	void mRUpHandler (Point);
	void mRDownHandler (Point);

	void mMoveHandler (Point);
	void directionUpdate();
	
public:
	MouseTargeter (GUEventman* evman, Timer* time, Point center);
	virtual ~MouseTargeter();

	virtual void setControl (Car* obj);

	static void preInit (GUEventman* evman, Timer* time, const Graphic_subsystem* graphics);
};

const float infelicity = 5;//TODO: May be it should be added to configuration file

#endif	/* MOUSETARGETER_H */

