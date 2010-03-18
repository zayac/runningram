/* 
 * File:   Carman.cpp
 * Author: necto
 * 
 * Created on February 16, 2010, 4:11 PM
 */

#include "Carman.h"
#include "Car.h"
#include "Eventman.h"
#include "Player_manager.h"
#include "Sprite.h"

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">


class Carman::Initialaiser : public Sectionp
{
public:
	string spritefname;
	int nsprites;
	Point sprite_offset;
	Carman* host;
	Car_creator data;

protected:
	virtual bool After_read (ifstream &file)
	{
		if (spritefname.size() > 0 && nsprites > 0)
		{
			Sprite *sp = new Sprite (spritefname.c_str(), nsprites, 1);
			sp->setTransparency (Color (255, 0, 255));//!!! only for a time
			sp->setPos (-sprite_offset);
			data.setPicture (sp);
		}
		host->Add_model (data.Create_copy ());
		return true;
	}
public:

	Initialaiser (char* name, Carman* chost, Eventman* sense)
	: Sectionp (name, '='), host (chost), data (sense), spritefname (""), nsprites(0)
	{
		Add_param (new St_loader<string> ("name", &data.model_name));
		Add_param (new St_loader<int>	("id", &data.model_id));
		Add_param (new St_loader<float> ("health", &data.start_health));
		Add_param (new St_loader<float> ("motor power", &data.motor_force));
		Add_param (new St_loader<float> ("back rmass", &data.rmass1));
		Add_param (new St_loader<float> ("front rmass", &data.rmass2));
		Add_param (new St_loader<float> ("lenght", &data.lenght));
		Add_param (new St_loader<float> ("back size", &data.r1));
		Add_param (new St_loader<float> ("front size", &data.r2));
		Add_param (new St_loader<float> ("bouncy", &data.bouncy));
		Add_param (new St_loader<float> ("rudder speed", &data.angular_vel));
		Add_param (new St_loader<float> ("rudder spring", &data.rudder_spring));
		Add_param (new St_loader<float> ("along friction", &data.fric.x));
		Add_param (new St_loader<float> ("across friction", &data.fric.y));
		Add_param (new St_loader<string> ("sprite file", &spritefname));
		Add_param (new St_loader<int> ("number of frames", &nsprites));
		Add_param (new St_loader<int> ("sprite centre x", &sprite_offset.x));
		Add_param (new St_loader<int> ("sprite centre y", &sprite_offset.y));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>

Carman::Carman (Eventman* sense):parser (new Carman::Initialaiser ("[Model]", this, sense)) { }

Carman::~Carman ()
{
	delete parser;
	for (iter i = models.begin(); i != models.end(); ++i)
		delete *i;
}
//--------------------------------------------------------------------------------------------------
Serializator* Carman::Get_parser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
Car* Carman::Create_car (int model, Vector2f pos, Orient start_orient, Player* host) const
{
	for (citer i = models.begin(); i != models.end(); ++i)
	{
		Car_creator* cc = *i;
		if ((**i).model_id == model)
			return (**i).New_car (pos, start_orient, host);
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Car_creator::Car_creator (Eventman* sens) :sense (sens),
	start_health (100),
	motor_force (10),
	bouncy (1.44),
	angular_vel (5),
	rudder_spring (8),
	rmass1 (40),
	rmass2 (40),
	lenght (30),
	r1 (10),
	r2 (10),
	fric (0.01, 1.0),
	picture(0)
{}
//--------------------------------------------------------------------------------------------------
Car* Car_creator::New_car (Vector2f pos, Orient start_orient, Player* host) const
{
	Car* ret = new Car (pos, start_health, motor_force, bouncy, angular_vel,
					 rudder_spring, rmass1, rmass2, lenght, r1, r2, fric, start_orient, picture, host);
	Key_storage contr = host->Get_control ();
	contr.Set_control (ret, sense);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Forwards), EV_KEYDOWN, up);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Forwardf), EV_KEYUP, up);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Backwards), EV_KEYDOWN, down);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Backwardf), EV_KEYUP, down);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_lefts), EV_KEYDOWN, left);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_leftf), EV_KEYUP, left);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_rights), EV_KEYDOWN, right);
//	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_rightf), EV_KEYUP, right);

	return ret;
}
//--------------------------------------------------------------------------------------------------
Car_creator* Car_creator::Create_copy() const
{
	Car_creator* ret = new Car_creator (sense);
	ret->model_name = model_name;
	ret->model_id = model_id;
	ret->start_health = start_health;
	ret->motor_force = motor_force;
	ret->bouncy = bouncy;
	ret->angular_vel = angular_vel;
	ret->rudder_spring = rudder_spring;
	ret->rmass1 = rmass1;
	ret->rmass2 = rmass2;
	ret->lenght = lenght;
	ret->r1 = r1;
	ret->r2 = r2;
	ret->fric = fric;
	ret->picture = picture;
	return ret;
}
//--------------------------------------------------------------------------------------------------