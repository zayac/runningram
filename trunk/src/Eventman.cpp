/* 
 * File:   Eventman.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 10:27 PM
 */
#include <SDL/SDL.h>
#include "Eventman.h"
#include "Console.h"

Eventman::Eventman () :stop (false)
{
}
//--------------------------------------------------------------------------------------------------
Eventman::~Eventman ()
{
	Clear_actions();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Applay_event (const Kbd_event& ev)
{
	assert(Ok());
	for (koiter i = kbd_opers.begin(); i != kbd_opers.end(); ++i)
		(**i)(ev);
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (ev.Contain (i->ev))
			(*i->fun)();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Clear_actions()
{
	assert(Ok());
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		delete i->fun;
	for (koiter i = kbd_opers.begin(); i != kbd_opers.end(); ++i)
		delete *i;
	kbdacts.clear();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Register_key_action (Functor* fun, Uint8 event, Key_id key, Key_mode mod)
{
	assert(Ok());
	kbdacts.push_back (Kbd_action (fun, key, event, mod));
}
//--------------------------------------------------------------------------------------------------
void Eventman::Register_key_oper (Kbd_oper op)
{
	assert(Ok());
	kbd_opers.push_back (op);
}
//--------------------------------------------------------------------------------------------------
void Eventman::Acts()
{
	assert(Ok());
    SDL_Event event;
	/* look for an event */
	if (SDL_PollEvent (&event))
	{
		/* an event was found */
		switch (event.type)
		{
			/* close button clicked */
		case SDL_QUIT:
			stop = 1;
			break;

			/* handle the keyboard */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			Applay_event (Kbd_event (event.key));
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Stopped() const
{
	assert(Ok());
	return stop;
}
//--------------------------------------------------------------------------------------------------
inline bool Boolly (bool arg) {return arg == true || arg == false;}
bool Eventman::Ok() const
{
	for (list <Kbd_action>::const_iterator i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (!i->Ok()) return false;
	return Boolly (stop);
}
//--------------------------------------------------------------------------------------------------