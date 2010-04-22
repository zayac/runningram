/* 
 * File:   Control.cpp
 * Author: necto
 * 
 * Created on April 22, 2010, 11:00 AM
 */

#include "Control.h"
#include "Car.h"
#include "Eventman.h"

//--------------------------------------------------------------------------------------------------
void Control::Applay_events()
{
	if (!object) return;//car doesn't exist

	if (events & UP_PRESS)		object->Forwards();
	if (events & UP_RELEASE)	object->Forwardf();
	if (events & DOWN_PRESS)	object->Backwards();
	if (events & DOWN_RELEASE)	object->Backwardf();
	if (events & LEFT_PRESS)	object->Turn_lefts();
	if (events & LEFT_RELEASE)	object->Turn_leftf();
	if (events & RIGHT_PRESS)	object->Turn_rights();
	if (events & RIGHT_RELEASE)	object->Turn_rightf();
}
//--------------------------------------------------------------------------------------------------
int Control::Export (char* buffer, int size) const
{
	if (sizeof (events) > size) return -1;
	memcpy (buffer, &events, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
int Control::Import (char* buffer, int size)
{
	if (sizeof (events) > size) return -1;
	memcpy (&events, buffer, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
void Key_storage::Set_control (Car* c)
{
	Control::Set_control (c);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Up_press), EV_KEYDOWN, up);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Up_release), EV_KEYUP, up);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Down_press), EV_KEYDOWN, down);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Down_release), EV_KEYUP, down);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Left_press), EV_KEYDOWN, left);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Left_release), EV_KEYUP, left);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Right_press), EV_KEYDOWN, right);
	evman->Register_key_action (new Arg_Method<void, void, Control> (this, &Control::Right_release), EV_KEYUP, right);
}
//--------------------------------------------------------------------------------------------------
Key_storage* Key_storage::Create_copy()
{
	Key_storage* rez = new Key_storage;
	rez->down = down;
	rez->up = up;
	rez->left = left;
	rez->right = right;
	rez->evman = evman;
	return rez;
}
//--------------------------------------------------------------------------------------------------