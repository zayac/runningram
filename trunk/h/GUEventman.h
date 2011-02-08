/* 
 * File:   Eventman.h
 * Author: necto
 *
 * Created on January 7, 2010, 10:27 PM
 */

#ifndef _GUEventman_H
#define	_GUEventman_H

#include <list>
#include <assert.h>
#include "Identified.h"
#include "Functor.h"
#include "Key_event.h"
#include "Mouse_event.h"

using std::list;

typedef Arg_Functor <void, Point> Mevent_handler;
typedef Arg_Functor <void, Kbd_event>* Kbd_oper;

class GUEventman
{
	struct Kbd_action :public Identified <Kbd_action>
	{
		Functor *fun;
		Kbd_event ev;

		Kbd_action (Functor* f, Key_id ki, Uint8 type, Key_mode mod = KM_NONE)
		:fun (f), ev (type, mod, ki) {assert(ok());}
		Kbd_action (Functor* f, Kbd_event event)
		:fun (f), ev (event) {assert(ok());}

		bool ok() const {return fun != 0;}
	};

	struct Mmove_action :public Identified<Mmove_action>
	{
		Mevent_handler *mh;
		Mouse_move_event ev;

		Mmove_action (Mevent_handler* f, Mouse_move_event event)
		:mh (f), ev (event) {assert(ok());}

		bool ok() const {return mh != 0;}
	};

	struct Mbtn_action :public Identified<Mbtn_action>
	{
		Mevent_handler *mh;
		Mouse_btn_event ev;

		Mbtn_action (Mevent_handler* f, Mouse_btn_event event)
		:mh (f), ev (event) {assert(ok());}

		bool ok() const {return mh != 0;}
	};

	typedef list <Kbd_action>::iterator kiter;
	typedef list <Mmove_action>::iterator mmiter;
	typedef list <Mbtn_action>::iterator mbiter;
	typedef list <Kbd_oper>::iterator koiter;

	list <Kbd_action> kbdacts;
	list <Kbd_oper> kbd_opers;
	list <Mmove_action> mmove_acts;
	list <Mbtn_action> mbtn_acts;
	bool stop_;

	void applayEvent (const Kbd_event& ev);
	void applayEvent (const Mouse_btn& ev);
	void applayEvent (const Mouse_move& ev);
	void clearActions();

public:
	GUEventman();
	virtual ~GUEventman();

	int registerKeyAction (Functor* fun, Uint8 event, Key_id key, Key_mode mod = KM_NONE);
	int registerMouseAction (Mevent_handler* fun, Mouse_move_event mme);
	int registerMouseAction (Mevent_handler* fun, Mouse_btn_event mbe);
	void registerKeyOper (Kbd_oper op);

	void unregisterKeyAction (int id);
	void unregisterMouseMoveAction (int id);
	void unregisterMouseBtnAction (int id);

	void acts();
	bool stopped() const;
	inline void stop() {stop_ = true;}

	bool ok() const;
};

#endif	/* _GUEventman_H */

