/* 
 * File:   BotControl.cpp
 * Author: necto
 * 
 * Created on February 27, 2011, 3:58 PM
 */

#include "BotControl.h"
#include "Interpreter.h"
#include "Timer.h"
#include "Logger.h"
#include "Car.h"


// <editor-fold defaultstate="collapsed" desc="From file Initializer">
#include <iostream>

class BotControl::Initializer : public Sectionp
{
public:
	static Timer* time;
	
	string fname;
	string command;

protected:
	virtual bool beforeRead (ifstream &file)
	{
		fname = "";
		command = "";
		return true;
	}
public:

	Initializer (const char* name)
	: Sectionp (name, '=')
	{
		addParam (new St_loader<string> ("file", &fname));
		addParam (new St_loader<string> ("command", &command));
	}

	virtual ~Initializer()
	{
		deleteProps();
	}

	BotControl* createBotControl() const
	{
		Interpreter::getInstance()->loadFile (fname.c_str());
		return new BotControl (new InfoTranS<Command> (command), time);
	}
};

Timer* BotControl::Initializer::time = 0;
//--------------------------------------------------------------------------------------------------
class BotControl::ParserHolder :public Control::ParserContainer
{
	BotControl::Initializer parser;
public:
	ParserHolder (const char* name) :parser (name){}

	virtual Serializator* getParser()
	{
		return &parser;
	}
	virtual Control* createControl()
	{
		return parser.createBotControl();
	}
};

// </editor-fold>


BotControl::BotControl (Decider* mind_, Timer* time_)
:time (time_), mind (mind_)
{
}
//--------------------------------------------------------------------------------------------------
BotControl::~BotControl()
{
	if (mind) delete mind;
}
//--------------------------------------------------------------------------------------------------
void BotControl::decide()
{return;
	CustomObject pos("vec");
	pos.set("x", UniValue::by (object->getPos().x));
	pos.set("y", UniValue::by (object->getPos().y));
	CustomObject vel("vec");
	vel.set("x", UniValue::by (object->getVel(object->getPos()).x));
	vel.set("y", UniValue::by (object->getVel(object->getPos()).y));
	CustomObject car("car-properties");
	car.set("pos", pos.convert());
	car.set("vel", vel.convert());
	Interpreter::getInstance()->functcall ("set-current-car", car.convert());
	Command act = (*mind)();
	act.applay (this);
}
//--------------------------------------------------------------------------------------------------
void BotControl::setControl (Car* obj)
{
	Control::setControl (obj);
	time->registerPeriodicEvent (10, new Arg_Method<void, void, BotControl>
											(this, &BotControl::decide));
}
//--------------------------------------------------------------------------------------------------
void BotControl::preInit (Timer* time)
{
	Initializer::time = time;
	registerParser (new ParserHolder ("script"));
}
//--------------------------------------------------------------------------------------------------
void BotControl::Command::LOR::set (int nval)
{
	switch (nval)
	{
		case -1:
			val = LEFT;
			break;
		case 0:
			val = STRAIGHT;
			break;
		case 1:
			val = RIGHT;
			break;
	}
}
//--------------------------------------------------------------------------------------------------
void BotControl::Command::LOR::applay (BotControl* executor)
{
	switch (val)
	{
		case LEFT:
			executor->rightRelease();
			executor->leftPress();
			break;
		case RIGHT:
			executor->rightPress();
			executor->leftRelease();
			break;
		case STRAIGHT:
			executor->rightRelease();
			executor->leftRelease();
			break;
	}
}
//--------------------------------------------------------------------------------------------------
void BotControl::Command::FOB::set (int nval)
{
	switch (nval)
	{
		case -1:
			val = FORWARD;
			break;
		case 0:
			val = STOP;
			break;
		case 1:
			val = BACKWARD;
			break;
	}
}
//--------------------------------------------------------------------------------------------------
void BotControl::Command::FOB::applay (BotControl* executor)
{
	switch (val)
	{
		case FORWARD:
			executor->downRelease();
			executor->upPress();
			break;
		case BACKWARD:
			executor->downPress();
			executor->upRelease();
			break;
		case STOP:
			executor->downRelease();
			executor->upRelease();
			break;
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
template <>
BotControl::Command UniValue::get<BotControl::Command>()
{
	if (isList()				&&//check if list satisfying
		head().is<int>()		&&//the template: "(lor fob)"
		tail().head().is<int>() )
	{
		int lor_val = head().get<int>();
		int fob_val = tail().head().get<int>();

		return BotControl::Command (lor_val, fob_val);
	}
	LOG(ERROR)<<"wrong command received!";
	return BotControl::Command (0, 0);
}
//--------------------------------------------------------------------------------------------------
