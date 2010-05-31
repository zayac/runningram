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
#include "Eventman.h"
#include "Control.h"

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

/*-----------------------------------------------------------------------------------------------*/
template <>
bool St_loader<Key_id>::Read_frag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	Cut_end_spaces (istr);

	*_val = Get_key_id (istr);

	return *_val != 0;
}
/*-----------------------------------------------------------------------------------------------*/

class Player_manager::Initialaiser : public Sectionp
{
public:
	Player_manager* host;
	string pname;
	int model;
	Key_storage ks;

protected:
	virtual bool After_read (ifstream &file)
	{
		host->push_back (new Player (pname, model, ks.Create_copy ()));
		return true;
	}
public:

	Initialaiser (const char* name, Player_manager* chost, Eventman* sense)
	: Sectionp (name, '='), host (chost)
	{
		ks.evman = sense;
		Add_param (new St_loader<string> ("name", &pname));
		Add_param (new St_loader<int>	("model", &model));
		Add_param (new St_loader<Key_id> ("up key", &ks.up));
		Add_param (new St_loader<Key_id> ("down key", &ks.down));
		Add_param (new St_loader<Key_id> ("left key", &ks.left));
		Add_param (new St_loader<Key_id> ("right key", &ks.right));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>

Player_manager::Player_manager (Eventman* sense)
:Transmitted('P', false), parser (new Initialaiser ("[Player]", this, sense)) { }

Player_manager::~Player_manager ()
{
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Player_manager::Get_parser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
Player* Player_manager::Get (int id) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if ((**i).Id () == id) return *i;
	return 0;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::Create_cars_for_poors (Carman* shop, Activeman* dest, Battlefield* site)
{
	for (iterator i = begin(); i != end(); ++i)
		if ((**i).Is_poor ()) dest->push_back ((**i).Create_car (shop, site->Get_next_res_point().To<float>()));
}
//--------------------------------------------------------------------------------------------------
int Player_manager::Export (char* buffer, int size) const
{
	int offset = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		*(buffer + offset++) = 'c';//continue

		*((int*)(buffer + offset)) = (**i).Id ();
		offset += sizeof(int);

		int len = (**i).Export (buffer + offset, size - offset);
		if (len == -1) return -1;
		
		offset += len;
		if (offset > size) return -1;
	}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Player_manager::Import (char* buffer, int size)
{
	int offset = 0;
	for (iterator i = begin();/* i != end()*/;)
	{
		char next = *(buffer + offset++);
		if (next == 's')
		{
			Erase (i, end());
			return offset;
		}
		if (next != 'c') return -1;			//if not 's' and not 'c' I can't determine what it is
		int id = *((int*)(buffer + offset));
		offset += sizeof(int);

		int len = -1;

		if (i == end() || id != (**i).Id())
		{
			bool found = false;
			iterator j = i;
			for (j = i; j != end(); ++j)
				if ((**j).Id() == id)
				{
					found = true;
					break;
				}
			if (found)
			{
				std::swap (*i, *j);
				len = (**i).Import (buffer + offset, size - offset);
			}
			else
			{
				j = insert (i, new Player ("no name", 0, new Key_storage));
				len = (**j).Import (buffer + offset, size - offset);
			}
		}
		else	len = (**i).Import (buffer + offset, size - offset);

		if (len == -1) return -1;
		offset += len;

		if (i != end()) ++i;
	}
	return offset;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::Erase (iterator start, iterator finish)
{
	for (iterator i = start; i != finish; ++i)
		delete *i;
	erase (start, finish);
}
//--------------------------------------------------------------------------------------------------
#include "Canvas.h"
#include "Fontc.h"
void Player_manager::Draw_comp_table (Canvas* where, Fontc* font)
{
	Point where_pos = where->getPos ();
	where->setPos (Point());

	int xpos = 100;
	int ypos = 100;
	for (iterator i = begin(); i != end(); ++i)
	{
		Rect label_brd;
		label_brd.x = xpos;
		label_brd.y = ypos;
		label_brd.w = 100;
		label_brd.h = 100;

		font->Draw_line (where, (**i).name.c_str(), &label_brd);
		label_brd.x += 100;

		char buf[256];
		#include <stdlib.h>
		sprintf (buf, "%i", (**i).Get_frags ());
		font->Draw_line (where, buf, &label_brd);
		ypos += 20;
	}

	where->setPos (where_pos);
}
//--------------------------------------------------------------------------------------------------
int Player_manager::Export_events (char* buffer, int size) const
{
	int offset = 0;
	for (const_iterator i = begin(); i != end(); ++i)
		if ((**i).Get_control()->Event_exists())			//only considerable events
		{

			*(buffer + offset++) = 'c';//continue

			*((	int*)(buffer + offset)) = (**i).Id ();
			offset += sizeof(int);

			int len = (**i).Get_control()->Export (buffer + offset, size - offset);
			if (len == -1) return -1;

			offset += len;
			if (offset > size) return -1;
		}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Player_manager::Import_events (char* buffer, int size)
{
	int offset = 0;
	int cur_id = 0;
	int len = -1;
	for (const_iterator i = begin(); ; ++i)
		if (*(buffer + offset++) == 'c')
		{
			cur_id = *(int*)(buffer + offset);
			offset += sizeof (int);

			while ((**i).Id() != cur_id) ++i;//corresponding player must be here!!!
			len = (**i).Get_control ()->Import (buffer + offset, size - offset);

			if (-1 == len) return -1;
			offset += len;

			(**i).Get_control ()->Applay_events ();
		}
		else break;
	return offset;
}
//--------------------------------------------------------------------------------------------------
void Player_manager::Clear_events()
{
	for (iterator i = begin(); i != end(); ++i)
		(**i).Get_control()->Clear_events();
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
	mobile = shop->Create_car (prefered_model, where, 0, this);
	return mobile;
}
//--------------------------------------------------------------------------------------------------
int Player::My_size() const
{
	return sizeof (frags) + sizeof (prefered_model) + (1 + name.size())*sizeof (string::value_type);
}
//--------------------------------------------------------------------------------------------------
int Player::Export (char* buffer, int size) const
{
	if (My_size() > size) return -1;
	memcpy (buffer, &frags, sizeof(frags));
	memcpy (buffer + sizeof (frags), &prefered_model, sizeof(prefered_model));
	const char* name_str = name.c_str();
	int name_size = sizeof(char)*(name.size() + 1);
	memcpy (buffer + sizeof (frags) + sizeof (prefered_model), name_str, name_size);
	return My_size ();
}
//--------------------------------------------------------------------------------------------------
int Player::Import (char* buffer, int size)
{
	if (My_size() > size) return -1;

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