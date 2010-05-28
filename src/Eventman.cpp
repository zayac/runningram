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
void Eventman::Applay_event (const Mouse_btn& ev)
{
	assert(Ok());
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		if (i->ev.Contain (ev))
			(*i->mh)(ev.pos);
}
//--------------------------------------------------------------------------------------------------
void Eventman::Applay_event (const Mouse_move& ev)
{
	assert(Ok());
	for (mmiter i = mmove_acts.begin(); i != mmove_acts.end(); ++i)
		if (i->ev.Contain (ev))
			(*i->mh)(ev.pos);
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

	kbd_opers.clear();//!!! not debugged
}
//--------------------------------------------------------------------------------------------------
int Eventman::Register_key_action (Functor* fun, Uint8 event, Key_id key, Key_mode mod)
{
	assert(Ok());
	kbdacts.push_back (Kbd_action (fun, key, event, mod));
	return kbdacts.rbegin()->Id();
}
//--------------------------------------------------------------------------------------------------
int Eventman::Register_mouse_action (Mevent_handler* fun, Mouse_move_event mme)
{
	assert(Ok());
	mmove_acts.push_back (Mmove_action (fun, mme));
	return mmove_acts.rbegin()->Id();
}
//--------------------------------------------------------------------------------------------------
int Eventman::Register_mouse_action (Mevent_handler* fun, Mouse_btn_event mbe)
{
	assert(Ok());
	mbtn_acts.push_back (Mbtn_action (fun, mbe));
	return mbtn_acts.rbegin()->Id();
}
//--------------------------------------------------------------------------------------------------
void Eventman::Register_key_oper (Kbd_oper op)
{
	assert(Ok());
	kbd_opers.push_back (op);
}
//--------------------------------------------------------------------------------------------------
void Eventman::Unregister_key_action (int id)
{
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (i->Id() == id)
		{
			delete i->fun;
			kbdacts.erase (i);
			return;
		}
}
//--------------------------------------------------------------------------------------------------
void Eventman::Unregister_mouse_move_action (int id)
{
	for (mmiter i = mmove_acts.begin(); i != mmove_acts.end(); ++i)
		if (i->Id() == id)
		{
			delete i->mh;
			mmove_acts.erase (i);
			return;
		}
}
//--------------------------------------------------------------------------------------------------
void Eventman::Unregister_mouse_btn_action (int id)
{
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		if (i->Id() == id)
		{
			delete i->mh;
			mbtn_acts.erase (i);
			return;
		}
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
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			Applay_event (Mouse_btn (event.button));
			break;
		case SDL_MOUSEMOTION:
			Applay_event (Mouse_move (event.motion));
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