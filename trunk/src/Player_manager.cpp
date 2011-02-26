/* 
 * File:   Player_manager.cpp
 * Author: necto
 * 
 * Created on February 18, 2010, 7:16 PM
 */

#include "Player_manager.h"
#include "Activeman.h"
#include "Carman.h"
#include "Car.h"
#include "Battlefield.h"
#include "GUEventman.h"
#include "Control.h"

// <editor-fold defaultstate="collapsed" desc="From file Initializer">

class Player_manager::Initializer : public Sectionp
{
public:
	Player_manager* host;
	string pname;
	int model;
	bool is_target;

protected:
	virtual bool afterRead (ifstream &file)
	{
	    host->push_front (new Player (pname, model, Control::createControl()));
	    if (is_target) host->setFirst2CamTarget();
	    return true;
	}
	virtual bool beforeRead (ifstream &file)
	{
	    is_target = false;
		return true;
	}
public:

	Initializer (const char* name, Player_manager* chost)
	: Sectionp (name, '='), host (chost), is_target (false)
	{
		addParam (new St_loader<string> ("name", &pname));
		addParam (new St_loader<int>	("model", &model));
		addParam (Control::newParser ("control"));
		addParam (new St_loader<bool>   ("attract cam", &is_target));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>

//--------------------------------------------------------------------------------------------------
Player_manager::Player_manager()
:Transmitted ('P', false) { }

//--------------------------------------------------------------------------------------------------
Player_manager::~Player_manager ()
{
}
//--------------------------------------------------------------------------------------------------
Serializator* Player_manager::newParser()
{
	parser = new Initializer ("[Player]", this);
	return parser;
}
//--------------------------------------------------------------------------------------------------
Player* Player_manager::get (int id) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if ((**i).id () == id) return *i;
	return 0;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::createCarsForPoors (Carman* shop, Activeman* dest, Battlefield* site)
{
	for (iterator i = begin(); i != end(); ++i)
		if ((**i).isPoor ()) dest->push_back ((**i).Create_car (shop, site->getNextResPoint().to<float>()));
}
//--------------------------------------------------------------------------------------------------
int Player_manager::exp (char* buffer, int size) const
{
	int offset = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		*(buffer + offset++) = 'c';//continue

		*((int*)(buffer + offset)) = (**i).id ();
		offset += sizeof(int);

		int len = (**i).exp (buffer + offset, size - offset);
		if (len == -1) return -1;
		
		offset += len;
		if (offset > size) return -1;
	}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Player_manager::imp (char* buffer, int size)
{
	int offset = 0;
	for (iterator i = begin();/* i != end()*/;)
	{
		char next = *(buffer + offset++);
		if (next == 's')
		{
			if (i != end()) erase (i, end());
			return offset;
		}
		if (next != 'c') return -1;			//if not 's' and not 'c' I can't determine what it is
		int id = *((int*)(buffer + offset));
		offset += sizeof(int);

		int len = -1;

		if (i == end() || id != (**i).id())
		{
			bool found = false;
			iterator j = i;
			for (j = i; j != end(); ++j)
				if ((**j).id() == id)
				{
					found = true;
					break;
				}
			if (found)
			{
				std::swap (*i, *j);
				len = (**i).imp (buffer + offset, size - offset);
			}
			else
			{
				j = insert (i, new Player ("no name", 0, 0));//here must be smth like exeption!!! (I think)// new Key_storage));
				len = (**j).imp (buffer + offset, size - offset);
			}
		}
		else	len = (**i).imp (buffer + offset, size - offset);

		if (len == -1) return -1;
		offset += len;

		if (i != end()) ++i;
	}
	return offset;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::erase (iterator start, iterator finish)
{
	for (iterator i = start; i != finish; ++i)
		delete *i;
	erase (start, finish);
}
//--------------------------------------------------------------------------------------------------
#include "Canvas.h"
#include "Fontc.h"
void Player_manager::drawCompTable (Canvas* where, Fontc* font)
{
	where->pushPos(Point());
	
	Point size = where->getSize();
	Point pos = size/10;

	Canvas c = where->createCompatible (size - 2*pos);
	c.setTotalTransparency (200);
	c.fill (Color (160, 160, 180));

	Rect label_brd;
	label_brd.x = 0;
	label_brd.y = 0;
	label_brd.w = 100;
	label_brd.h = 100;
	for (iterator i = begin(); i != end(); ++i)
	{
		label_brd.x = 0;

		font->drawLine (&c, (**i).name.c_str(), &label_brd);
		label_brd.x += 100;

		char buf[256];
		#include <stdlib.h>
		sprintf (buf, "%i", (**i).getFrags ());
		font->drawLine (&c, buf, &label_brd);
		label_brd.y += font->height();
	}
	c.draw (*where, pos);
	where->popPos();
}
//--------------------------------------------------------------------------------------------------
int Player_manager::exportEvents (char* buffer, int size) const
{
	int offset = 0;
	for (const_iterator i = begin(); i != end(); ++i)
		if ((**i).getControl()->eventExists())			//only considerable events
		{

			*(buffer + offset++) = 'c';//continue

			*((	int*)(buffer + offset)) = (**i).id ();
			offset += sizeof(int);

			int len = (**i).getControl()->exp (buffer + offset, size - offset);
			if (len == -1) return -1;

			offset += len;
			if (offset > size) return -1;
		}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Player_manager::importEvents (char* buffer, int size)
{
	int offset = 0;
	int cur_id = 0;
	int len = -1;
	for (const_iterator i = begin(); ; ++i)
		if (*(buffer + offset++) == 'c')
		{
			cur_id = *(int*)(buffer + offset);
			offset += sizeof (int);

			while ((**i).id() != cur_id) ++i;//corresponding player must be here!!!
			len = (**i).getControl ()->imp (buffer + offset, size - offset);

			if (-1 == len) return -1;
			offset += len;

			(**i).getControl ()->applayEvents ();
		}
		else break;
	return offset;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::clearEvents()
{
	for (iterator i = begin(); i != end(); ++i)
		(**i).getControl()->clearEvents();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Player::Player (string name_, int pref_model, Control* ks_)
	:name (name_), prefered_model (pref_model), mobile(0), frags(0), ks (ks_)
{
}
//--------------------------------------------------------------------------------------------------
Car* Player::Create_car (Carman* shop, Vector2f where)
{
	mobile = shop->createCar (prefered_model, where, 0, this);
	return mobile;
}
//--------------------------------------------------------------------------------------------------
int Player::mySize() const
{
	return sizeof (frags) + sizeof (prefered_model) + (1 + name.size())*sizeof (string::value_type);
}
//--------------------------------------------------------------------------------------------------
int Player::exp (char* buffer, int size) const
{
	if (mySize() > size) return -1;
	memcpy (buffer, &frags, sizeof(frags));
	memcpy (buffer + sizeof (frags), &prefered_model, sizeof(prefered_model));
	const char* name_str = name.c_str();
	int name_size = sizeof(char)*(name.size() + 1);
	memcpy (buffer + sizeof (frags) + sizeof (prefered_model), name_str, name_size);
	return mySize ();
}
//--------------------------------------------------------------------------------------------------
int Player::imp (char* buffer, int size)
{
	if (mySize() > size) return -1;

	int offset = 0;
	memcpy (&frags, buffer, sizeof(frags));
	offset += sizeof (frags);
	memcpy (&prefered_model, buffer + offset, sizeof(prefered_model));
	offset += sizeof (prefered_model);

	name.clear ();
	char c = 0;
	while (1)
	{
		c = *(buffer + offset++);
		if (c == 0) break;
		name += c;
	}
	return offset;
}
//--------------------------------------------------------------------------------------------------