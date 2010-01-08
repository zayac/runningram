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

using std::list;

class Eventman
{
	struct Kbd_action
	{
		Functor *fun;
		Uint8 event;
		SDLKey key;
	};
	typedef list <Kbd_action>::iterator kiter;

	list <Kbd_action> kbdacts;
	bool stop;

	void Applay_event (SDL_KeyboardEvent& ev);
	void Clear_actions();

public:
	Eventman();
	virtual ~Eventman();

	void Register_key_action (Functor* fun, Uint8 event, SDLKey key);

	void Acts();
	bool Stopped() const;

	bool Ok() const;
};

#endif	/* _EVENTMAN_H */

