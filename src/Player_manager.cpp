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
void Player_manager::Create_cars_for_poors (Carman* shop, Activeman* dest)
{
	for (iterator i = begin(); i != end(); ++i)
	{
		if ((**i).Is_poor ()) dest->push_back ((**i).Create_car (shop));
	}
}
//--------------------------------------------------------------------------------------------------
Player::Player (string name_, int pref_model) :name (name_), preffered_model (pref_model), mobile(0), frags(0)
{
}
//--------------------------------------------------------------------------------------------------
Car* Player::Create_car (Carman* shop)
{
	mobile = shop->Create_car (preffered_model, Vector2f(), 0, this);
	return mobile;
}
//--------------------------------------------------------------------------------------------------