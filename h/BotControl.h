/* 
 * File:   BotControl.h
 * Author: necto
 *
 * Created on February 27, 2011, 3:58 PM
 */

#ifndef BOTCONTROL_H
#define	BOTCONTROL_H

#include "Control.h"
#include "Functor.h"

class Timer;

class BotControl :public Control
{
public:
	struct Command
	{
		struct LOR
		{
			enum LEFT_OR_RIGHT
			{
				LEFT = -1,
				STRAIGHT = 0,
				RIGHT = 1
			} val;
			void set (int nval);
			void applay (BotControl* executor);
		} lor;

		struct FOB
		{
			enum FORWARD_OR_BACKWARD
			{
				FORWARD = -1,
				STOP = 0,
				BACKWARD = 1
			} val;
			void set (int nval);
			void applay (BotControl* executor);
		} fob;

		inline void applay (BotControl* executor)
		{
			lor.applay (executor);
			fob.applay (executor);
		}

		Command (int lor_val, int fob_val)
		{
			lor.set (lor_val);
			fob.set (fob_val);
		}
	};

	typedef Arg_Functor<Command, void, void> Decider;
private:
	Decider* mind;
	Timer* time;

	class Initializer;
	class ParserHolder;

	void defineContext();

public:
	void decide();

public:
	BotControl (Decider* mind, Timer* time);
	virtual ~BotControl();

	virtual void setControl (Car* obj);

	static void preInit (Timer* time);
};

#endif	/* BOTCONTROL_H */

