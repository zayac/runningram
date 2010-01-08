/* 
 * File:   Eventman.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 10:27 PM
 */
#include <SDL/SDL.h>
#include "Eventman.h"

Eventman::Eventman () :stop (false)
{
}
//--------------------------------------------------------------------------------------------------
Eventman::~Eventman ()
{
	Clear_actions();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Applay_event (SDL_KeyboardEvent& ev)
{
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (ev.keysym.sym == i->key && ev.type == i->event)
			(*i->fun)();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Clear_actions()
{
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		delete i->fun;
	kbdacts.clear();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Register_key_action (Functor* fun, Uint8 event, SDLKey key)
{
	kbdacts.push_back (Kbd_action {fun, event, key});
}
//--------------------------------------------------------------------------------------------------
void Eventman::Acts()
{
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
			Applay_event (event.key);
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Stopped() const
{
	return stop;
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Ok() const
{
	return true;
}
//--------------------------------------------------------------------------------------------------