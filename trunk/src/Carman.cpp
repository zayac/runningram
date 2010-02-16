/* 
 * File:   Carman.cpp
 * Author: necto
 * 
 * Created on February 16, 2010, 4:11 PM
 */

#include "Carman.h"
#include "Car.h"
#include "Eventman.h"
// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Carman::Initialaiser : public Sectionp
{
public:
	Carman* host;
	Car_creator data;

protected:
	virtual bool After_read (ifstream &file)
	{
		host->Add_model (data.Create_copy ());
		return true;
	}
public:

	Initialaiser (char* name, Carman* chost, Eventman* sense)
	: Sectionp (name, '='), host (chost), data (sense)
	{
		Add_param (new St_loader<string> ("name", &data.model_name));
		Add_param (new St_loader<int>	("id", &data.model_id));
		Add_param (new St_loader<float> ("health", &data.start_health));
		Add_param (new St_loader<float> ("back rmass", &data.rmass1));
		Add_param (new St_loader<float> ("front rmass", &data.rmass2));
		Add_param (new St_loader<float> ("lenght", &data.lenght));
		Add_param (new St_loader<float> ("back size", &data.r1));
		Add_param (new St_loader<float> ("front size", &data.r2));
		Add_param (new St_loader<float> ("along friction", &data.fric.x));
		Add_param (new St_loader<float> ("across friction", &data.fric.y));
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
Car* Carman::Create_car (int model, Vector2f pos, Orient start_orient) const
{
	for (citer i = models.begin(); i != models.end(); ++i)
	{
		Car_creator* cc = *i;
		if ((**i).model_id == model)
			return (**i).New_car (pos, start_orient);
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Car_creator::Car_creator (Eventman* sens) :sense (sens),
	start_health (100),
	rmass1 (40),
	rmass2 (40),
	lenght (30),
	r1 (10),
	r2 (10),
	fric (0.01, 1.0)
{}
//--------------------------------------------------------------------------------------------------
Car* Car_creator::New_car (Vector2f pos, Orient start_orient) const
{
	Car* ret = new Car (pos, start_health, rmass1, rmass2, lenght, r1, r2, fric, start_orient);

	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Forwards), SDL_KEYDOWN, SDLK_w);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Forwardf), SDL_KEYUP, SDLK_w);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Backwards), SDL_KEYDOWN, SDLK_s);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Backwardf), SDL_KEYUP, SDLK_s);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_lefts), SDL_KEYDOWN, SDLK_a);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_leftf), SDL_KEYUP, SDLK_a);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_rights), SDL_KEYDOWN, SDLK_d);
	sense->Register_key_action (new Arg_Method<void, void, Car> (ret, &Car::Turn_rightf), SDL_KEYUP, SDLK_d);

	return ret;
}
//--------------------------------------------------------------------------------------------------
Car_creator* Car_creator::Create_copy() const
{
	Car_creator* ret = new Car_creator (sense);
	ret->model_name = model_name;
	ret->model_id = model_id;
	ret->start_health = start_health;
	ret->rmass1 = rmass1;
	ret->rmass2 = rmass2;
	ret->lenght = lenght;
	ret->r1 = r1;
	ret->r2 = r2;
	ret->fric = fric;
	return ret;
}
//--------------------------------------------------------------------------------------------------