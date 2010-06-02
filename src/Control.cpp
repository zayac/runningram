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
void Control::applayEvents()
{
	if (!object) return;//car doesn't exist

	if (events & UP_PRESS)		object->forwards();
	if (events & UP_RELEASE)	object->forwardf();
	if (events & DOWN_PRESS)	object->backwards();
	if (events & DOWN_RELEASE)	object->backwardf();
	if (events & LEFT_PRESS)	object->turnLefts();
	if (events & LEFT_RELEASE)	object->turnLeftf();
	if (events & RIGHT_PRESS)	object->turnRights();
	if (events & RIGHT_RELEASE)	object->turnRightf();
}
//--------------------------------------------------------------------------------------------------
int Control::exp (char* buffer, int size) const
{
	if (sizeof (events) > size) return -1;
	memcpy (buffer, &events, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
int Control::imp (char* buffer, int size)
{
	if (sizeof (events) > size) return -1;
	memcpy (&events, buffer, sizeof (events));
	return sizeof (events);
}
//--------------------------------------------------------------------------------------------------
void Key_storage::setControl (Car* c)
{
	Control::setControl (c);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::upPress), EV_KEYDOWN, up);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::upRelease), EV_KEYUP, up);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::downPress), EV_KEYDOWN, down);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::downRelease), EV_KEYUP, down);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::leftPress), EV_KEYDOWN, left);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::leftRelease), EV_KEYUP, left);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::rightPress), EV_KEYDOWN, right);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::rightRelease), EV_KEYUP, right);
}
//--------------------------------------------------------------------------------------------------
Key_storage* Key_storage::createCopy()
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