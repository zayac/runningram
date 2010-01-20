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

const float VeryBigMass = 1e10;

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
public:
	Active (Vector2f position, float r_):Limited (position, r_){}
	virtual void Actions (float dt) = 0;
	virtual void Draw (Canvas*) = 0;
	virtual Vector2f Collis_brd (Rect width) = 0;

	virtual bool Ok() const = 0;
};

class Body :public Limited
{
protected:
	float rev_mass;
	Vector2f vel;
	Vector2f force;

protected:
	inline Vector2f Get_vel() {return vel;}
	virtual void Resistance(){};

public:
	Body (float rmass, float r_, Vector2f position)
	:Limited (position, r_), rev_mass (rmass){}
	virtual ~Body();


	inline void Applay_motion (float dt)
	{
		Resistance();
		pos += vel*dt + force*rev_mass*dt*dt/2;
		vel += force*rev_mass*dt;
		force = Vector2f();
	}
	inline Vector2f Get_impulse()
	{
		if (rev_mass) return vel/rev_mass;
		return vel*VeryBigMass;
	}

	inline void Appl_force (Vector2f f) {force += f;}

	inline bool Ok() const {return rev_mass > 0;}
};

class Dir_body :public Body
{
	friend class Car;
protected:
	Orient orient;
	Vector2f fric;//[0] = x = parallel, [1] = y = normal friction

	virtual void Resistance()
	{
		Vector2f dir = orient.Get_dir();
		Vector2f proj = (Get_vel()^dir)*dir;
		Appl_force (-fric[0]*proj - fric[1]*(Get_vel() - proj));
	}
public:
	Dir_body (float rmass, float r_, Vector2f coor, Vector2f friction, Orient start_orient)
		:Body (rmass, r_, coor), orient (start_orient), fric (friction) {}

	inline bool Ok() const {return Body::Ok() && fric.x > 0 && fric.y > 0 && orient.Ok();}
};

const float Motor_force = 10.;
const float Angular_vel = 5;
const float Max_ang_dev = 1.;
const float Rudder_spring = 8;

class Eventman;

class Car :public Active
{
	bool rp, lp, fp, bp;//right, left, forward, backward procedure states

	Dir_body back;
	Dir_body front;

	float rmass;

	float lenght;

	void Process_gestures (float dt);
	void Motory_force (float f);
	void Turn_front (float dfi);
	void Normalise_front_orient();
	void Rudder_correction (float dt);

public:
	Car (Eventman* sense, Vector2f pos, float rmass1, float rmass2, float lenght, float r1, float r2, Vector2f fric, Orient start_orient);

	virtual void Actions (float dt);
	virtual void Draw (Canvas*);
	virtual Vector2f Collis_brd (Rect with);

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

//	void Applay_force (Vector2d f, Vector2d papp);

	bool Ok() const;
};

#endif	/* _BODY_H */

