/* 
 * File:   Eventman.h
 * Author: necto
 *
 * Created on January 7, 2010, 10:27 PM
 */

#ifndef _EVENTMAN_H
#define	_EVENTMAN_H

#include <list>
#include <assert.h>
#include "Identified.h"
#include "Functor.h"
#include "Key_event.h"

using std::list;

typedef Arg_Functor <void, Kbd_event>* Kbd_oper;

class Eventman
{
	struct Kbd_action :public Identified <Kbd_action>
	{
		Functor *fun;
		Kbd_event ev;

		Kbd_action (Functor* f, Key_id ki, Uint8 type, Key_mode mod = KM_NONE)
		:fun (f), ev (type, mod, ki) {assert(Ok());}
		Kbd_action (Functor* f, Kbd_event event)
		:fun (f), ev (event) {assert(Ok());}

		bool Ok() const {return fun != 0;}
	};
	typedef list <Kbd_action>::iterator kiter;
	typedef list <Kbd_oper>::iterator koiter;

	list <Kbd_action> kbdacts;
	list <Kbd_oper> kbd_opers;
	bool stop;

	void Applay_event (const Kbd_event& ev);
	void Clear_actions();

public:
	Eventman();
	virtual ~Eventman();

	int Register_key_action (Functor* fun, Uint8 event, Key_id key, Key_mode mod = KM_NONE);
	void Register_key_oper (Kbd_oper op);

	void Unregister_key_action (int id);

	void Acts();
	bool Stopped() const;
	void Stop() {stop = true;}

	bool Ok() const;
};

#endif	/* _EVENTMAN_H */

