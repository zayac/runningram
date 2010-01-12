/* 
 * File:   Body.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 3:39 PM
 */

#include "Car.h"
#include "Orient.h"
#include "Canvas.h"
#include "Eventman.h"
//
//Body::Body () { }
//
//Body::Body (const Body& orig) { }
//
Body::~Body () { }

//--------------------------------------------------------------------------------------------------
Car::Car (Eventman* sense, Vector2f coor, float rmass1, float rmass2, float len, float r1, float r2, Vector2f fric, Orient start_orient)
:rp(0), lp(0), fp(0), bp(0), lenght (len), pos (coor), rmass (1/(1/rmass1 + 1/rmass2)),
back (rmass1, r1, coor - start_orient.Get_dir()*len/(1 + rmass1/rmass2), fric, start_orient),
front (rmass2, r2, coor + start_orient.Get_dir()*len/(1 + rmass2/rmass1), fric, start_orient)
{
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Forwards), SDL_KEYDOWN, SDLK_UP);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Forwardf), SDL_KEYUP, SDLK_UP);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Backwards), SDL_KEYDOWN, SDLK_DOWN);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Backwardf), SDL_KEYUP, SDLK_DOWN);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Turn_lefts), SDL_KEYDOWN, SDLK_LEFT);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Turn_leftf), SDL_KEYUP, SDLK_LEFT);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Turn_rights), SDL_KEYDOWN, SDLK_RIGHT);
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::Turn_rightf), SDL_KEYUP, SDLK_RIGHT);

	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
void Car::Actions (float dt)
{
	assert(Ok());
	front.Applay_motion (dt);
	back.Applay_motion (dt);
	
	Orient ori(front.pos - back.pos);

	pos = (back.pos/back.rev_mass + front.pos/front.rev_mass)*rmass;
	front.pos = pos + ori.Get_dir()*lenght/(1 + front.rev_mass/back.rev_mass);
	back.pos = pos - ori.Get_dir()*lenght/(1 + back.rev_mass/front.rev_mass);

	back.orient = ori;
	Process_gestures (dt);
	Normalise_front_orient();

	Rudder_correction (dt);

}
//--------------------------------------------------------------------------------------------------
void Car::Draw (Canvas* c)
{
	assert(Ok());
	Point begin = back.pos.To<int>();
	Point end = front.pos.To<int>();//(front.orient.Get_dir()*front.r + pos).To<int> ();

	Point leftback = Point (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + begin;
	Point leftfront = Point (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + end;
	Point rightback = Point (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + begin;
	Point rightfront = Point (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + end;

	Point one_trg = (front.r*front.orient.Get_dir() + front.pos).To<int>();
	Point two_trg = (front.r*(front.orient + Orient(2*PI/3)).Get_dir() + front.pos).To<int>();
	Point three_trg = (front.r*(front.orient - Orient(2*PI/3)).Get_dir() + front.pos).To<int>();

	c->Line (leftback, leftfront, Color (150, 200, 200));
	c->Line (leftfront, rightfront, Color (150, 200, 200));
	c->Line (rightfront, rightback, Color (150, 200, 200));
	c->Line (rightback, leftback, Color (150, 200, 200));

	c->Line (one_trg, two_trg, Color (200, 150, 150));
	c->Line (two_trg, three_trg, Color (200, 150, 150));
	c->Line (three_trg, one_trg, Color (200, 150, 150));
	c->Line (end, one_trg, Color (200, 150, 150));
}
//--------------------------------------------------------------------------------------------------
void Car::Process_gestures (float dt)
{
	assert(Ok());
	if (fp && !bp) Motory_force ( Motor_force);
	if (bp && !fp) Motory_force (-Motor_force);
	if (rp && !lp) Turn_front ( Angular_vel*dt);
	if (lp && !rp) Turn_front (-Angular_vel*dt);
}
//--------------------------------------------------------------------------------------------------
void Car::Motory_force (float f)
{
	assert(Ok());
	Vector2f accl = f*rmass*back.orient.Get_dir ();
	back.Appl_force (accl/back.rev_mass);
	front.Appl_force (accl/front.rev_mass);
}
//--------------------------------------------------------------------------------------------------
void Car::Turn_front (float dfi)
{
	assert(Ok());
//	Orient delta = front.orient - back.orient;
//	if (delta + dfi > Max_ang_dev) dfi = Max_ang_dev - delta;
//	if (delta + dfi < -Max_ang_dev) dfi = -Max_ang_dev - delta;

	front.orient += dfi;
}
//--------------------------------------------------------------------------------------------------
void Car::Normalise_front_orient()
{
	assert(Ok());
	float delta = In_range ((front.orient - back.orient).Get_angle ());
	if (delta > Max_ang_dev) front.orient += Max_ang_dev - delta;
	if (delta < -Max_ang_dev) front.orient += -Max_ang_dev - delta;
}
//--------------------------------------------------------------------------------------------------
void Car::Rudder_correction (float dt)
{
	assert(Ok());
	float delta = In_range ((front.orient - back.orient).Get_angle ());
	front.orient = back.orient + delta*(1 - Rudder_spring*dt);
}
//--------------------------------------------------------------------------------------------------
inline bool Boolly (bool arg) {return arg == true || arg == false;}
bool Car::Ok() const
{
	return Boolly (fp) && Boolly (bp) && Boolly (lp) && Boolly (rp) &&
		back.Ok () && front.Ok () && lenght > 0 && rmass > 0;
}
//--------------------------------------------------------------------------------------------------