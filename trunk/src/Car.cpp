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

#ifndef max
inline float max (float a, float b) {return a < b ? b : a;}
#endif

float aboutnull = 0.000000001;

struct Collision_vector
{
	Vector2f papp;
	Vector2f depth;

	inline float Weight() {return depth.Lensq();}
	inline bool Vital() {return Weight() > aboutnull;}
};
Collision_vector Collis_rectangles (Vector2f one[4], Vector2f two[4], bool recalled = false);

//
//Body::Body () { }
//
//Body::Body (const Body& orig) { }
//
Body::~Body () { }

//--------------------------------------------------------------------------------------------------
Car::Car (Eventman* sense, Vector2f coor, float rmass1, float rmass2, float len, float r1, float r2, Vector2f fric, Orient start_orient)
:Active (coor, len + max(r1, r2)), rp(0), lp(0), fp(0), bp(0), lenght (len), rmass (1/(1/rmass1 + 1/rmass2)),
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

	process_collisions = true;
	sense->Register_key_action (new Arg_Method<void, void, Car> (this, &Car::SwitchPC), SDL_KEYUP, SDLK_c);

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

	c->line (leftback, leftfront, Color (150, 200, 200));
	c->line (leftfront, rightfront, Color (150, 200, 200));
	c->line (rightfront, rightback, Color (150, 200, 200));
	c->line (rightback, leftback, Color (150, 200, 200));

	Color headc (200, 150, 150);
	if (!process_collisions) headc = Color (150, 200, 250);

	c->line (one_trg, two_trg, headc);
	c->line (two_trg, three_trg, headc);
	c->line (three_trg, one_trg, headc);
	c->line (end, one_trg, headc);
}
//--------------------------------------------------------------------------------------------------
#include <iostream>
void Car::Collis_brd (Rect with)
{
	if (!process_collisions) return;

	Vector2f lu (with.x, with.y);//left-up
	Vector2f ru (with.x + with.w, with.y);//right-up
	Vector2f ld (with.x, with.y + with.h);//left-down
	Vector2f rd (with.x + with.w, with.y + with.h);//right-down

	Vector2f lb = Vector2f (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + back.pos;
	Vector2f lf = Vector2f (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + front.pos;
	Vector2f rb = Vector2f (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + back.pos;
	Vector2f rf = Vector2f (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + front.pos;

	Vector2f one[4] = {lb, rb, rf, lf};
	Vector2f two[4] = {lu, ru, rd, ld};

	Collision_vector curv = Collis_rectangles (one, two);
	float papp_weight = curv.Weight();
	Vector2f rez_papp = curv.papp;

	int num_iters = 20;

	while (num_iters-- > 0 && curv.Vital())
	{
		back.pos += curv.depth;
		front.pos += curv.depth;

		one[0] = Vector2f (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + back.pos;
		one[1] = Vector2f (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + back.pos;
		one[2] = Vector2f (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + front.pos;
		one[3] = Vector2f (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + front.pos;
		
		curv = Collis_rectangles (one, two);
		rez_papp = (rez_papp*papp_weight + curv.papp*curv.Weight())/(papp_weight + curv.Weight()); //averaging point of application
		papp_weight += curv.Weight();
	}
}
//--------------------------------------------------------------------------------------------------
inline Vector2f From_point_to_stline (Vector2f end, Vector2f delta, Vector2f p)
{
	return (delta^(p-end))*delta/delta.Lensq() - (p - end);
}
inline bool Same_sign (float a, float b) {return a==0 ? b==0 : (a>0 ? b>0 : b<0);}
inline bool Same_side (Vector2f end, Vector2f delta, Vector2f a, Vector2f b)
{
	return Same_sign (delta.x*(a.y - end.y) - delta.y*(a.x- end.x),
					  delta.x*(b.y - end.y) - delta.y*(b.x- end.x));
}
#define IN(one, two, three, four, centre, what) Same_side (one, two-one, centre, what) &&		\
												Same_side (two, three-two, centre, what) &&		\
												Same_side (three, four-three, centre, what) &&	\
												Same_side (four, one-four, centre, what)
//-------------
inline Vector2f Get_depth (Vector2f rect[4], Vector2f centre, Vector2f p, Vector2f hiscentre)
{
	Vector2f ret (1/aboutnull, 1/aboutnull);

	if (Same_side (rect[3], rect[0] - rect[3], centre, p))
		ret = From_point_to_stline (rect[3], rect[0] - rect[3], p);
//	if ((ret^(p-hiscentre)) > 0) ret = Vector2f();//depth only from centre

	for (int i = 0; i < 3; ++i)
		if (Same_side (rect[i], rect[i + 1] - rect[i], centre, p))
		{
			Vector2f cur = From_point_to_stline (rect[i], rect[i + 1] - rect[i], p);
			if (/*(cur^(p-hiscentre)) < 0 &&   */ //depth only from centre
			    cur.Lensq() < ret.Lensq()) ret = cur;
		}
	return ret;
}
//-------------
Collision_vector Collis_rectangles (Vector2f one[4], Vector2f two[4], bool recalled)
{
	Vector2f cent1 = (one[0] + one[1] + one[2] + one[3])/4;
	Vector2f cent2 = (two[0] + two[1] + two[2] + two[3])/4;

	Collision_vector ret;

//	for (int i = 0; i < 4; ++i)
//		for (int j = 0; j < 4; ++j)
//			if ((one[i] - two[j]).Lensq () < aboutnull) return Vector2f();

	for (int i = 0; i < 4; ++i)
		if (IN(two[0], two[1], two[2], two[3], cent2, one[i]))
		{
			Vector2f depth = Get_depth (two, cent2, one[i], cent1);
			if (depth.Lensq() > ret.depth.Lensq())
			{
				ret.depth = depth;
				ret.papp = one[i];
			}
		}

	if (!recalled)
		for (int j = 0; j < 4; ++j)
			if (IN(one[0], one[1], one[2], one[3], cent1, two[j]))
				if (Get_depth (one, cent1, two[j], cent2).Lensq() > ret.depth.Lensq())
				{
					ret = Collis_rectangles (two, one, true);
					ret.depth = -ret.depth;
//					Vector2f cur_depth = -Collis_rectangles (two, one, true);
//					if (true)
//						max_depth = cur_depth;
				}
	return ret;
}
#undef IN
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