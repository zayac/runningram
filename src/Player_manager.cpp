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

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Player_manager::Initialaiser : public Sectionp
{
public:
	Player_manager* host;
	string pname;
	int model;

protected:
	virtual bool After_read (ifstream &file)
	{
		host->push_back (new Player (pname, model));
		return true;
	}
public:

	Initialaiser (char* name, Player_manager* chost)
	: Sectionp (name, '='), host (chost)
	{
		Add_param (new St_loader<string> ("name", &pname));
		Add_param (new St_loader<int>	("model", &model));
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
Player::Player (string name_, int pref_model) :name (name_), preffered_model (pref_model), mobile(0), frags(0)
{
}
//--------------------------------------------------------------------------------------------------
Car* Player::Create_car (Carman* shop, Vector2f where)
{
	mobile = shop->Create_car (preffered_model, where, 0, this);
	return mobile;
}
//--------------------------------------------------------------------------------------------------