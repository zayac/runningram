/* 
 * File:   Eventman.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 10:27 PM
 */
#include <SDL/SDL.h>
#include "Eventman.h"
#include "Console.h"

Eventman::Eventman () :stop (false), console_active(false), cmd (0)
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
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (ev.ki == i->key && ev.type == i->event)
			(*i->fun)();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Clear_actions()
{
	assert(Ok());
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		delete i->fun;
	kbdacts.clear();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Register_key_action (Functor* fun, Uint8 event, Key_id key)
{
	assert(Ok());
	kbdacts.push_back (Kbd_action (fun, event, key));
}
//--------------------------------------------------------------------------------------------------
void Eventman::Set_console (Console* c)
{
	cmd = c;
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
void Eventman::Switch_console()
{
	assert(Ok());
	console_active = !console_active;

	if (console_active) SDL_EnableKeyRepeat (300, 50);
	else SDL_EnableKeyRepeat (0, 0);
}
//--------------------------------------------------------------------------------------------------
bool Eventman::Console_enabled()
{
	assert(Ok());
	return console_active;
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
			if (console_active) cmd->Operate (event.key);
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
	if (console_active && cmd == 0) return false;
	return (cmd == 0 || cmd->Ok()) && Boolly (stop) && Boolly (console_active);
}
//--------------------------------------------------------------------------------------------------