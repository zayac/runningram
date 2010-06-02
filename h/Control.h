/* 
 * File:   Control.h
 * Author: necto
 *
 * Created on April 22, 2010, 11:00 AM
 */

#ifndef _CONTROL_H
#define	_CONTROL_H

#include "Transmitted.h"
#include "Identified.h"
#include "mstdint.h"
#include "Key_event.h"

class Car;
class Eventman;

enum Button_cases
{
	UP_PRESS =			0x00000001,
	UP_RELEASE =		0x00000010,
	DOWN_PRESS =		0x00000100,
	DOWN_RELEASE =		0x00001000,
	LEFT_PRESS =		0x00010000,
	LEFT_RELEASE =		0x00100000,
	RIGHT_PRESS =		0x01000000,
	RIGHT_RELEASE =		0x10000000
};

class Control
{
	Car* object;
	Uint32 events;

protected:
	inline void setEvent (Uint32 ev) {events |= ev;}

public:
	void upPress()		{setEvent (UP_PRESS);		applayEvents();}
	void downPress()	{setEvent (DOWN_PRESS);	applayEvents();}
	void leftPress()	{setEvent (LEFT_PRESS);	applayEvents();}
	void rightPress()	{setEvent (RIGHT_PRESS);	applayEvents();}

	void upRelease()	{setEvent (UP_RELEASE);	applayEvents();}
	void downRelease()	{setEvent (DOWN_RELEASE);	applayEvents();}
	void leftRelease()	{setEvent (LEFT_RELEASE);	applayEvents();}
	void rightRelease(){setEvent (RIGHT_RELEASE);	applayEvents();}

public:
	Control ():events(0){}
	virtual void setControl (Car* obj) {object = obj;}
	virtual void carCrushed (Car*) {object = 0;}

	void applayEvents();
	bool eventExists() {return events != 0;}
	void clearEvents() {events = 0;}

	virtual int exp (char* buffer, int size) const;
	virtual int imp (char* buffer, int size);
};
//============================================================================
struct Key_storage :public Control
{
	Key_id up;
	Key_id down;
	Key_id left;
	Key_id right;

	Eventman* evman;

	void setControl (Car*);
	Key_storage* createCopy();
};

#endif	/* _CONTROL_H */

