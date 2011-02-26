/* 
 * File:   Control.cpp
 * Author: necto
 * 
 * Created on April 22, 2010, 11:00 AM
 */

#include <map>

#include "Control.h"
#include "Car.h"
#include "GUEventman.h"
#include "initparser.h"


Control::ParserContainer* Control::current_parser = 0;
vector<Control::ParserContainer*> Control::parsers;

// <editor-fold defaultstate="collapsed" desc="From file Initializer">

class Control::Initializer : public St_loader <string>
{
public:
	string parser_name;

protected:
	virtual bool beforeRead (ifstream& file)
	{
		parser_name = "";
	}
	virtual bool afterRead (ifstream &file)
	{
		Serializator* ser = chooseParser (parser_name);
		if (!ser) return false;
		return ser->unserialise (file);
	}
public:

	Initializer (const char* name)
	:St_loader<string> (name, &parser_name)
	{}
}; // </editor-fold>


//--------------------------------------------------------------------------------------------------
void Control::applayEvents()
{
	if (!object) return;//car doesn't exist

	if (events & UP_PRESS)		object->forwards();
	if (events & UP_RELEASE)	object->forwardf();
	if (events & DOWN_PRESS)	object->backwards();
	if (events & DOWN_RELEASE)	object->backwardf();
	if (events & LEFT_PRESS)	object->turnLefts();
	if (events & LEFT_RELEASE)	object->turnLeftf();
	if (events & RIGHT_PRESS)	object->turnRights();
	if (events & RIGHT_RELEASE)	object->turnRightf();
}
//--------------------------------------------------------------------------------------------------
int Control::exp (char* buffer, int size) const
{
	if (sizeof (events) > size) return -1;
	memcpy (buffer, &events, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
int Control::imp (char* buffer, int size)
{
	if (sizeof (events) > size) return -1;
	memcpy (&events, buffer, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
Serializator* Control::newParser (const char* name)
{
	current_parser = 0;
	return new Initializer (name);
}
//--------------------------------------------------------------------------------------------------
Control* Control::createControl()
{
	return current_parser->createControl();
}
//--------------------------------------------------------------------------------------------------
void Control::registerParser (ParserContainer* instance)
{
	parsers.push_back (instance);
}
//--------------------------------------------------------------------------------------------------
Serializator* Control::chooseParser (const string& name)
{
	for (vector<ParserContainer*>::
				const_iterator i = parsers.begin();
			i != parsers.end(); ++i)
			if ((**i).isItMyName (name))
			{
				current_parser = *i;
				return current_parser->getParser();
			}
	return 0;
}
//--------------------------------------------------------------------------------------------------