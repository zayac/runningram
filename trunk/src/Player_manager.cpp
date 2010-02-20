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
		host->push_back (new Player (pname, model, ks));
		return true;
	}
public:

	Initialaiser (char* name, Player_manager* chost)
	: Sectionp (name, '='), host (chost)
	{
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

Player_manager::Player_manager ():parser (new Initialaiser ("[Player]", this)) { }

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
void Player_manager::Create_cars_for_poors (Carman* shop, Activeman* dest, Battlefield* site)
{
	for (iterator i = begin(); i != end(); ++i)
		if ((**i).Is_poor ()) dest->push_back ((**i).Create_car (shop, site->Get_next_res_point().To<float>()));
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
Player::Player (string name_, int pref_model, const Key_storage& ks_)
	:name (name_), preffered_model (pref_model), mobile(0), frags(0), ks (ks_)
{
}
//--------------------------------------------------------------------------------------------------
Car* Player::Create_car (Carman* shop, Vector2f where)
{
	mobile = shop->Create_car (preffered_model, where, 0, this);
	return mobile;
}

//--------------------------------------------------------------------------------------------------
void Key_storage::Set_control (Car* c, Eventman* sense)
{
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Forwards), EV_KEYDOWN, up);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Forwardf), EV_KEYUP, up);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Backwards), EV_KEYDOWN, down);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Backwardf), EV_KEYUP, down);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Turn_lefts), EV_KEYDOWN, left);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Turn_leftf), EV_KEYUP, left);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Turn_rights), EV_KEYDOWN, right);
	sense->Register_key_action (new Arg_Method<void, void, Car> (c, &Car::Turn_rightf), EV_KEYUP, right);
}
//--------------------------------------------------------------------------------------------------