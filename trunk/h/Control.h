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
	inline void Set_event (Uint32 ev) {events |= ev;}

public:
	void Up_press()		{Set_event (UP_PRESS);		Applay_events();}
	void Down_press()	{Set_event (DOWN_PRESS);	Applay_events();}
	void Left_press()	{Set_event (LEFT_PRESS);	Applay_events();}
	void Right_press()	{Set_event (RIGHT_PRESS);	Applay_events();}

	void Up_release()	{Set_event (UP_RELEASE);	Applay_events();}
	void Down_release()	{Set_event (DOWN_RELEASE);	Applay_events();}
	void Left_release()	{Set_event (LEFT_RELEASE);	Applay_events();}
	void Right_release(){Set_event (RIGHT_RELEASE);	Applay_events();}

public:
	Control ():events(0){}
	virtual void Set_control (Car* obj) {object = obj;}
	virtual void Car_crushed (Car*) {object = 0;}

	void Applay_events();
	bool Event_exists() {return events != 0;}
	void Clear_events() {events = 0;}

	virtual int Export (char* buffer, int size) const;
	virtual int Import (char* buffer, int size);
};
//============================================================================
struct Key_storage :public Control
{
	Key_id up;
	Key_id down;
	Key_id left;
	Key_id right;

	Eventman* evman;

	void Set_control (Car*);
	Key_storage* Create_copy();
};

#endif	/* _CONTROL_H */

