/* 
 * File:   Eventman.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 10:27 PM
 */
#include <SDL/SDL.h>
#include "GUEventman.h"
#include "Console.h"

GUEventman GUEventman::instance;

GUEventman::GUEventman () :stop_ (false)
{
}
//--------------------------------------------------------------------------------------------------
GUEventman* GUEventman::getInstance()
{
	return &instance;
}
//--------------------------------------------------------------------------------------------------
GUEventman::~GUEventman ()
{
	clearActions();
}
//--------------------------------------------------------------------------------------------------
void GUEventman::applayEvent (const Kbd_event& ev)
{
	assert(ok());
	for (koiter i = kbd_opers.begin(); i != kbd_opers.end(); ++i)
		(**i)(ev);
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (ev.contain (i->ev))
			(*i->fun)();
}
//--------------------------------------------------------------------------------------------------
void GUEventman::applayEvent (const Mouse_btn& ev)
{
	assert(ok());
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		if (i->ev.contain (ev))
			(*i->mh)(ev.pos);
}
//--------------------------------------------------------------------------------------------------
void GUEventman::applayEvent (const Mouse_move& ev)
{
	assert(ok());
	for (mmiter i = mmove_acts.begin(); i != mmove_acts.end(); ++i)
		if (i->ev.contain (ev))
			(*i->mh)(ev.pos);
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		if (i->ev.contain (ev))//process "away" event
			(*i->mh)(ev.pos);
}
//--------------------------------------------------------------------------------------------------
void GUEventman::clearActions()
{
	assert(ok());
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		delete i->fun;
	for (koiter i = kbd_opers.begin(); i != kbd_opers.end(); ++i)
		delete *i;
	kbdacts.clear();
	kbd_opers.clear();
	
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		delete i->mh;
	for (mmiter i = mmove_acts.begin(); i != mmove_acts.end(); ++i)
		delete i->mh;
	mbtn_acts.clear ();
	mmove_acts.clear ();//!!! not debugged
}
//--------------------------------------------------------------------------------------------------
int GUEventman::registerKeyAction (Functor* fun, Uint8 event, Key_id key, Key_mode mod)
{
	assert(ok());
	kbdacts.push_back (Kbd_action (fun, key, event, mod));
	return kbdacts.rbegin()->id();
}
//--------------------------------------------------------------------------------------------------
int GUEventman::registerMouseAction (Mevent_handler* fun, Mouse_move_event mme)
{
	assert(ok());
	mmove_acts.push_back (Mmove_action (fun, mme));
	return mmove_acts.rbegin()->id();
}
//--------------------------------------------------------------------------------------------------
int GUEventman::registerMouseAction (Mevent_handler* fun, Mouse_btn_event mbe)
{
	assert(ok());
	mbtn_acts.push_back (Mbtn_action (fun, mbe));
	return mbtn_acts.rbegin()->id();
}
//--------------------------------------------------------------------------------------------------
void GUEventman::registerKeyOper (Kbd_oper op)
{
	assert(ok());
	kbd_opers.push_back (op);
}
//--------------------------------------------------------------------------------------------------
void GUEventman::unregisterKeyAction (int id)
{
	for (kiter i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (i->id() == id)
		{
			delete i->fun;
			kbdacts.erase (i);
			return;
		}
}
//--------------------------------------------------------------------------------------------------
void GUEventman::unregisterMouseMoveAction (int id)
{
	for (mmiter i = mmove_acts.begin(); i != mmove_acts.end(); ++i)
		if (i->id() == id)
		{
			delete i->mh;
			mmove_acts.erase (i);
			return;
		}
}
//--------------------------------------------------------------------------------------------------
void GUEventman::unregisterMouseBtnAction (int id)
{
	for (mbiter i = mbtn_acts.begin(); i != mbtn_acts.end(); ++i)
		if (i->id() == id)
		{
			delete i->mh;
			mbtn_acts.erase (i);
			return;
		}
}
//--------------------------------------------------------------------------------------------------
void GUEventman::acts()
{
	assert(ok());
    SDL_Event event;
	/* look for an event */
	if (SDL_PollEvent (&event))
	{
		/* an event was found */
		switch (event.type)
		{
			/* close button clicked */
		case SDL_QUIT:
			stop_ = 1;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			applayEvent (Mouse_btn (event.button));
			break;
		case SDL_MOUSEMOTION:
			applayEvent (Mouse_move (event.motion));
			break;

			/* handle the keyboard */
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			applayEvent (Kbd_event (event.key));
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
bool GUEventman::stopped() const
{
	assert(ok());
	return stop_;
}
//--------------------------------------------------------------------------------------------------
inline bool Boolly (bool arg) {return arg == true || arg == false;}
bool GUEventman::ok() const
{
	for (list <Kbd_action>::const_iterator i = kbdacts.begin(); i != kbdacts.end(); ++i)
		if (!i->ok()) return false;
	return Boolly (stop_);
}
//--------------------------------------------------------------------------------------------------