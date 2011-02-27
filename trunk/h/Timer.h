/* 
 * File:   Timer.h
 * Author: necto
 *
 * Created on February 27, 2011, 1:11 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <vector>
#include "Functor.h"

using std::vector;

typedef unsigned int msecs;

class Timer
{
	msecs last;

	struct PeriodicEvent
	{
		msecs period;
		Functor* fun;
		msecs last;

		PeriodicEvent (msecs period_, Functor* fun_)
				:period (period_), fun (fun_) {}

		inline void run (msecs current)
		{
			if ((current - last) > period)
			{
				(*fun)();
				last = current;
			}
		}
	};

	vector <PeriodicEvent> events;

	void runEvents (msecs current);
	void clearEvents();

private:
	Timer();
	static Timer instance;

public:
	static inline Timer* getInstance() {return &instance;}
	virtual ~Timer();

	msecs update();
	float elapsed() const;

	void registerPeriodicEvent (msecs period, Functor* fun);
};

#endif	/* TIMER_H */

