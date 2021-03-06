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
#include "initparser.h"

class Car;
class GUEventman;
class Serializator;

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
	Uint32 events;

protected:
	Car* object;
	inline void setEvent (Uint32 ev) {events |= ev;}

public:
	void upPress()		{setEvent (UP_PRESS);		applayEvents();}
	void downPress()	{setEvent (DOWN_PRESS);		applayEvents();}
	void leftPress()	{setEvent (LEFT_PRESS);		applayEvents();}
	void rightPress()	{setEvent (RIGHT_PRESS);	applayEvents();}

	void upRelease()	{setEvent (UP_RELEASE);		applayEvents();}
	void downRelease()	{setEvent (DOWN_RELEASE);	applayEvents();}
	void leftRelease()	{setEvent (LEFT_RELEASE);	applayEvents();}
	void rightRelease()	{setEvent (RIGHT_RELEASE);	applayEvents();}

public:
	Control ():events(0){}
	virtual void setControl (Car* obj_) {object = obj_;}
	virtual void carCrushed (Car*) {object = 0;}

	void applayEvents();
	bool eventExists() {return events != 0;}
	void clearEvents() {events = 0;}

	virtual int exp (char* buffer, int size) const;
	virtual int imp (char* buffer, int size);

	static Serializator* newParser (const char* name);
	static Control* createControl();

protected:
	class ParserContainer
	{
	public:
		ParserContainer(){}

		virtual Serializator* getParser() = 0;
		virtual Control* createControl() = 0;

		inline const bool isItMyName (const string& name)
		{return getParser()->isItMyName (name, false);}
	};

	static void registerParser (ParserContainer* instance);

private:
	static vector<ParserContainer*> parsers;
	static ParserContainer* current_parser;
	static Serializator* chooseParser (const string& name);

	class Initializer;
};
#endif	/* _CONTROL_H */

