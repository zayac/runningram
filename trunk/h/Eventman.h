/* 
 * File:   Eventman.h
 * Author: necto
 *
 * Created on January 7, 2010, 10:27 PM
 */

#ifndef _EVENTMAN_H
#define	_EVENTMAN_H

#include <SDL/SDL.h>
#include "Functor.h"
#include <list>
#include <assert.h>

using std::list;

class Console;

class Eventman
{
	struct Kbd_action
	{
		Functor *fun;
		Uint8 event;
		SDLKey key;

		Kbd_action (Functor* f, Uint8 ev, SDLKey k)
		:fun (f), event (ev), key(k) {assert(Ok());}

		bool Ok() const {return fun != 0;}
	};
	typedef list <Kbd_action>::iterator kiter;

	list <Kbd_action> kbdacts;
	bool stop;

	bool console_active;
	Console* cmd;

	void Applay_event (SDL_KeyboardEvent& ev);
	void Clear_actions();

public:
	Eventman();
	virtual ~Eventman();

	void Register_key_action (Functor* fun, Uint8 event, SDLKey key);

	void Set_console (Console* c);
	void Switch_console();
	bool Console_enabled();

	void Acts();
	bool Stopped() const;

	bool Ok() const;
};

#endif	/* _EVENTMAN_H */

