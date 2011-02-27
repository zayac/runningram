/* 
 * File:   Timer.cpp
 * Author: necto
 * 
 * Created on February 27, 2011, 1:11 PM
 */

#include <SDL/SDL.h>
#include "Timer.h"

Timer Timer::instance;

Timer::Timer()
{
	update();
}
//--------------------------------------------------------------------------------------------------
Timer::~Timer()
{
	clearEvents();
}
//--------------------------------------------------------------------------------------------------

msecs Timer::update()
{
	last = SDL_GetTicks();
	runEvents (last);
}
//--------------------------------------------------------------------------------------------------
float Timer::elapsed() const
{
	return 0.001*(SDL_GetTicks() - last);
}
//--------------------------------------------------------------------------------------------------
void Timer::runEvents (msecs current)
{
	for (vector<PeriodicEvent>::iterator i = events.begin(); i!= events.end(); ++i)
		i->run (current);
}
//--------------------------------------------------------------------------------------------------
void Timer::clearEvents()
{
	for (vector<PeriodicEvent>::const_iterator i = events.begin(); i!= events.end(); ++i)
		delete i->fun;
	events.clear();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Timer::registerPeriodicEvent (msecs period, Functor* fun)
{
	events.push_back(PeriodicEvent (period, fun));
}
//--------------------------------------------------------------------------------------------------
