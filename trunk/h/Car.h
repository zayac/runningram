/* 
 * File:   Body.h
 * Author: necto
 *
 * Created on January 10, 2010, 3:39 PM
 */

#ifndef _BODY_H
#define	_BODY_H
#include "Vec.h"
#include "Canvas.h"
#include "Orient.h"
#include "Collision_detection.h"
//#include "Console.h"

const float VeryBigMass = 1e10;

enum Active_objs
{
	No_type = 0,
	Car_type
};

class Limited
{
protected:
	Vector2f pos;
	float r;
public:
	Limited (Vector2f position, float r_):pos(position), r(r_){}
	
	inline float Get_r() const {return r;}
	inline Vector2f Get_pos() const {return pos;}
	
	virtual bool Ok() const {return r >= 0;}
};

class Active :public Limited
{
protected:
	virtual int Sign_date_len() const {return sizeof(pos) + sizeof(r);}
public:
	Active (Vector2f position, float r_):Limited (position, r_){}
	virtual void Actions (float dt) = 0;
	virtual void Draw (Canvas*) = 0;
	virtual void Collis_brd (Rect width, float fric) = 0;
	virtual void Drive_sand (Rect width, float fric) = 0;
	virtual void Collis_obj (Active* that) = 0;

	virtual int Export (char* buffer, int size) const;
	virtual int Import (char* buffer, int size);
	
	virtual int My_type() const {return No_type;}
	virtual bool Dead() const = 0;

	virtual bool Ok() const = 0;
};

class Body :public Limited
{
protected:
	float rev_mass;
	Vector2f vel;
	Vector2f force;
	Vector2f resist;

protected:
	inline Vector2f Get_vel() {return vel;}
	virtual Vector2f Resistance(){};

public:
	Body (float rmass, float r_, Vector2f position)
	:Limited (position, r_), rev_mass (rmass){}
	virtual ~Body();


	inline void Applay_motion (float dt)
	{
		pos += vel*dt + force*rev_mass*dt*dt/2;
		vel += force*rev_mass*dt;
		force = Vector2f();
		Appl_resistance (Resistance());
		Vector2f resdv = resist*rev_mass*dt;
		if (resdv.Lensq() > vel.Lensq())
			vel -= vel.Proj(resdv);
		else
			vel += resdv;
		resist = Vector2f();
//		int iterations = 1;
//		while (resdv.Lensq() > vel.Lensq())
//		{
//			iterations *= 2;
//			resdv /= 2;
//			dt /= 2;
//		}
//		for (; iterations > 0; --iterations)
//		{
//			resdv = Resistance()*rev_mass*dt;
//			vel += resdv;
//		}

//		Resistance();			//order is important
//		Vector2f vel_addition = force*rev_mass*dt;
//		Vector2f vel_add_along = vel_addition.Proj (vel);
//		Vector2f vel_add_across = vel_addition - vel_add_along;
//
//		if (vel_add_along.Lensq() > vel.Lensq()) vel_add_along = -vel;//for limit resistance force
//		vel += vel_add_across + vel_add_along;
//		force = Vector2f();
	}
	inline Vector2f Get_impulse()
	{
		if (rev_mass) return vel/rev_mass;
		return vel*VeryBigMass;
	}

	inline void Appl_force (Vector2f f) {force += f;}
	inline void Appl_resistance (Vector2f f) {resist += f;}
	inline void Appl_force (Vector2f f, bool resistancive)
	{
		if (resistancive) Appl_resistance(f);
		else Appl_force (f);
	}
	inline void Appl_impulse (Vector2f imp) {vel += imp*rev_mass;}

	inline bool Ok() const {return rev_mass > 0;}
};

class Dir_body :public Body
{
	friend class Car;
protected:
	Orient orient;
	Vector2f fric;//[0] = x = parallel, [1] = y = normal friction

	virtual Vector2f Resistance()
	{
		Vector2f dir = orient.Get_dir();
		Vector2f proj = (Get_vel()^dir)*dir;
		return -fric[0]*proj - fric[1]*(Get_vel() - proj);
	}
public:
	Dir_body (float rmass, float r_, Vector2f coor, Vector2f friction, Orient start_orient)
		:Body (rmass, r_, coor), orient (start_orient), fric (friction) {}

	Vector2f Get_resist (Vector2f velocity)
	{
		Vector2f dir = orient.Get_dir();
		Vector2f proj = (velocity^dir)*dir;
		return -fric[0]*proj - fric[1]*(velocity - proj);
	}

	inline bool Ok() const {return Body::Ok() && fric.x > 0 && fric.y > 0 && orient.Ok();}
};

//const float Angular_vel = 5;
const float Max_ang_dev = 1.;
//const float Rudder_spring = 8;
//const float Bouncy = 2.0;

class Player;
class Sprite;
const int Health_indicator_len = 50;
const int Health_indicator_offset = -20;
const int Health_indicator_height = 5;

class Car :public Active
{
	Player* host;

	bool rp, lp, fp, bp;//right, left, forward, backward procedure states

	Dir_body back;
	Dir_body front;

	float rmass;
	float lenght;
	float health;
	float motor_force;
	float bouncy;
	float angular_vel;
	float rudder_spring;

	Sprite* pic;

	static float max_health;

	void Process_gestures (float dt);
	void Motory_force (float f);
	void Turn_front (float dfi);
	void Normalise_front_orient();
	void Rudder_correction (float dt);

	void Applay_brd_collision (Collision_vector cv, float fric);
	void Applay_obj_collision (Car* with, Collision_vector cv);

	static const bool Back_rect = true;
	static const bool Front_rect = false;
	void Applay_sand_friction (Collision_vector cv, float fric, bool bORf);

	void Move (Vector2f disp) {back.pos += disp; front.pos += disp;}

protected:
	virtual int Sign_date_len() const;

public:
	Car (Vector2f pos, float health, float motor_force, float bouncy, float angular_vel, float rudder_spring,
		 float rmass1, float rmass2, float lenght, float r1, float r2, Vector2f fric, Orient start_orient,
		 Sprite* pic = 0, Player* host = 0);

	virtual void Actions (float dt);
	virtual void Draw (Canvas*);
	virtual void Collis_obj (Active* that);
	virtual void Collis_brd (Rect with, float fric);
	virtual void Drive_sand (Rect width, float fric);

	void Get_my_verticies (Vector2f* four);
	void Get_my_front_verticies (Vector2f* four);
	void Get_my_back_verticies (Vector2f* four);
	Vector2f Get_vel (Vector2f p);
	Vector2f Get_imp (Vector2f p);
	bool Damage (Vector2f imp, Vector2f papp, float destructive_k);			//returns true if killed
	void Appl_impulse (Vector2f imp, Vector2f papp, float destructive_k = 1);
	void Appl_force (Vector2f f, Vector2f papp, bool resistancive);

	virtual int Export (char* buffer, int size) const;
	virtual int Import (char* buffer, int size);
	
	virtual int My_type() const {return Car_type;}
	virtual bool Dead() const;

//	Vector2f Collis_rectangle (Vector2f one, Vector2f two, Vector2f three, Vector2f four);

	void Turn_rights()	{rp = true;}
	void Turn_lefts()	{lp = true;}
	void Forwards()		{fp = true;}
	void Backwards()	{bp = true;}
							//s = start procedure, f = finish
	void Turn_rightf()	{rp = false;}
	void Turn_leftf()	{lp = false;}
	void Forwardf()		{fp = false;}
	void Backwardf()	{bp = false;}

	bool Ok() const;
};
void DBG_switch();
#endif	/* _BODY_H */

