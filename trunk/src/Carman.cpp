/* 
 * File:   Carman.cpp
 * Author: necto
 * 
 * Created on February 16, 2010, 4:11 PM
 */

#include "Carman.h"
#include "Car.h"
#include "Player_manager.h"
#include "Sprite.h"
#include "Activeman.h"
#include "Control.h"
#include "initparser.h"

// <editor-fold defaultstate="collapsed" desc="From file Initializer">


class Carman::Initializer : public Sectionp
{
public:
	string spritefname;
	int nsprites;
	Point sprite_offset;
	Carman* host;
	Car_creator data;

protected:
	virtual bool afterRead (ifstream &file)
	{
		if (spritefname.size() > 0 && nsprites > 0)
		{
			Sprite *sp = new Sprite (spritefname.c_str(), nsprites, 1, true);
			//sp->setTransparency (Color (255, 0, 255));//!!! only for a time
			sp->setPos (-sprite_offset);
			data.setPicture (sp);
		}
		host->addModel (data.Create_copy ());
		return true;
	}
public:

	Initializer (const char* name, Carman* chost)
	: Sectionp (name, '='), host (chost), data(), spritefname (""), nsprites(0)
	{
		addParam (new St_loader<string> ("name", &data.model_name));
		addParam (new St_loader<int>	("id", &data.model_id));
		addParam (new St_loader<float> ("health", &data.start_health));
		addParam (new St_loader<float> ("motor power", &data.motor_force));
		addParam (new St_loader<float> ("back rmass", &data.rmass1));
		addParam (new St_loader<float> ("front rmass", &data.rmass2));
		addParam (new St_loader<float> ("lenght", &data.lenght));
		addParam (new St_loader<float> ("back size", &data.r1));
		addParam (new St_loader<float> ("front size", &data.r2));
		addParam (new St_loader<float> ("bouncy", &data.bouncy));
		addParam (new St_loader<float> ("rudder speed", &data.angular_vel));
		addParam (new St_loader<float> ("rudder spring", &data.rudder_spring));
		addParam (new St_loader<float> ("turn transfer", &data.turn_transfer));
		addParam (new St_loader<float> ("along friction", &data.fric.x));
		addParam (new St_loader<float> ("across friction", &data.fric.y));
		addParam (new St_loader<string> ("sprite file", &spritefname));
		addParam (new St_loader<int> ("number of frames", &nsprites));
		addParam (new St_loader<int> ("sprite centre x", &sprite_offset.x));
		addParam (new St_loader<int> ("sprite centre y", &sprite_offset.y));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>

Carman::Carman():Transmitted ('C', true), hosts(0), objs(0), effm(0) { }

Carman::~Carman ()
{
	for (iter i = models.begin(); i != models.end(); ++i)
		delete *i;
}
//--------------------------------------------------------------------------------------------------
Serializator* Carman::newParcer()
{
	parser = new Carman::Initializer ("[Model]", this);
	return parser;
}
//--------------------------------------------------------------------------------------------------
Car* Carman::createCar (int model, Vector2f pos, Orient start_orient, Player* host, int id)
{
	for (citer i = models.begin(); i != models.end(); ++i)
	{
		Car_creator* cc = *i;
		if ((**i).model_id == model)
		{
			Car* ncar = (**i).New_car (pos, start_orient, host, effm, id);
			last_creations.push_back (Creation (model, pos, start_orient, host->id(), ncar->id()));
			important_ = true;
			return ncar;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
void Carman::clesrLastActions()
{
	last_creations.clear();
	important_ = false;
}
//--------------------------------------------------------------------------------------------------
int Carman::exp (char* buffer, int size) const
{
	int offset = 0;
	for (list <Creation>::const_iterator i = last_creations.begin();
		i != last_creations.end(); ++i)
	{
		*(buffer + offset++) = 'c';//continue
		int len = i->exp (buffer + offset, size - offset);
		if (len == -1) return -1;
		offset += len;
		if (offset + 1 > size) return -1;
	}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Carman::imp (char* buffer, int size)
{
	int offset = 0;
	Creation cur;
	while (*(buffer + offset++) == 'c')
	{
		int len = cur.imp (buffer + offset, size - offset);
		if (len == -1) return -1;
		offset += len;
		if (offset > size) return -1;
		objs->push_back (createCar (cur.model, cur.pos, cur.start_orient, hosts->get (cur.pl_id), cur.id));
	}
	return offset;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Car_creator::Car_creator() :
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
	turn_transfer (0.1),
	fric (0.01, 1.0),
	picture(0)
{}
//--------------------------------------------------------------------------------------------------
Car* Car_creator::New_car (Vector2f pos, Orient start_orient, Player* host, Effects_manager* em, int id) const
{
	Car* ret = new Car (pos, start_health, motor_force, bouncy, angular_vel,
					 rudder_spring, rmass1, rmass2, lenght, r1, r2, turn_transfer, fric, start_orient, id, picture, host, em);
	Control* contr = host->getControl ();
//	contr->evman = sense;
	contr->setControl (ret);

	return ret;
}
//--------------------------------------------------------------------------------------------------
Car_creator* Car_creator::Create_copy() const
{
	Car_creator* ret = new Car_creator();
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
	ret->turn_transfer = turn_transfer;
	ret->fric = fric;
	ret->picture = picture;
	return ret;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Carman::Creation::Creation() : model(0), pos(), start_orient(0), pl_id(0) {}
//--------------------------------------------------------------------------------------------------
Carman::Creation::Creation (int model_, Vector2f pos_, Orient start_orient_, int pl_id_, int car_id)
:model (model_), pos (pos_), start_orient (start_orient_), pl_id (pl_id_), id (car_id)
{
}
//--------------------------------------------------------------------------------------------------
Carman::Creation::Creation (const Carman::Creation& orig)
:model (orig.model), pos (orig.pos), start_orient (orig.start_orient), pl_id (orig.pl_id), id (orig.id)
{
}
//--------------------------------------------------------------------------------------------------
int Carman::Creation::exp (char* buffer, int size) const
{
	if (sizeof (Creation) > size) return -1;
	memcpy (buffer, this, sizeof (Creation));
	return sizeof (Creation);
}
//--------------------------------------------------------------------------------------------------
int Carman::Creation::imp (char* buffer, int size)
{
	if (sizeof (Creation) > size) return -1;
	memcpy (this, buffer, sizeof (Creation));
	return sizeof (Creation);
}
//--------------------------------------------------------------------------------------------------