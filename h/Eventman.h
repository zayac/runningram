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
#include "Mouse_event.h"

using std::list;

typedef Arg_Functor <void, Point> Mevent_handler;
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

	struct Mmove_action :public Identified<Mmove_action>
	{
		Mevent_handler *mh;
		Mouse_move_event ev;

		Mmove_action (Mevent_handler* f, Mouse_move_event event)
		:mh (f), ev (event) {assert(Ok());}

		bool Ok() const {return mh != 0;}
	};

	struct Mbtn_action :public Identified<Mbtn_action>
	{
		Mevent_handler *mh;
		Mouse_btn_event ev;

		Mbtn_action (Mevent_handler* f, Mouse_btn_event event)
		:mh (f), ev (event) {assert(Ok());}

		bool Ok() const {return mh != 0;}
	};

	typedef list <Kbd_action>::iterator kiter;
	typedef list <Mmove_action>::iterator mmiter;
	typedef list <Mbtn_action>::iterator mbiter;
	typedef list <Kbd_oper>::iterator koiter;

	list <Kbd_action> kbdacts;
	list <Kbd_oper> kbd_opers;
	list <Mmove_action> mmove_acts;
	list <Mbtn_action> mbtn_acts;
	bool stop;

	void Applay_event (const Kbd_event& ev);
	void Applay_event (const Mouse_btn& ev);
	void Applay_event (const Mouse_move& ev);
	void Clear_actions();

public:
	Eventman();
	virtual ~Eventman();

	int Register_key_action (Functor* fun, Uint8 event, Key_id key, Key_mode mod = KM_NONE);
	int Register_mouse_action (Mevent_handler* fun, Mouse_move_event mme);
	int Register_mouse_action (Mevent_handler* fun, Mouse_btn_event mbe);
	void Register_key_oper (Kbd_oper op);

	void Unregister_key_action (int id);
	void Unregister_mouse_move_action (int id);
	void Unregister_mouse_btn_action (int id);

	void Acts();
	bool Stopped() const;
	inline void Stop() {stop = true;}

	bool Ok() const;
};

#endif	/* _EVENTMAN_H */

