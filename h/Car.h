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
#include "Identified.h"
#include "Effects_manager.h"
#include "Drawable.h"

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
	
	inline float getR() const {return r;}
	inline Vector2f getPos() const {return pos;}
	
	virtual bool ok() const {return r >= 0;}
};

class Active :public Limited, public Identified<Active>, public Drawable
{
protected:
	virtual int signDataLen() const {return sizeof(pos) + sizeof(r);}
public:
	Active (Vector2f position, float r_, int id = 0):Limited (position, r_), Identified<Active> (id) {}
	virtual ~Active() {}
	virtual void actions (float dt) = 0;
	virtual void draw (Canvas*) = 0;
	virtual void collisBrd (Rect width, float fric) = 0;
	virtual void driveSand (Rect width, float fric) = 0;
	virtual void collisObj (Active* that) = 0;

	virtual int exp (char* buffer, int size) const;
	virtual int imp (char* buffer, int size);
	
	virtual int myType() const {return No_type;}
	virtual bool dead() const = 0;
	virtual void die() = 0;
};

class Body :public Limited
{
protected:
	float rev_mass;
	Vector2f vel;
	Vector2f force;
	Vector2f resist;

protected:
	inline Vector2f getVel() {return vel;}
	virtual Vector2f resistance(){};

public:
	Body (float rmass, float r_, Vector2f position)
	:Limited (position, r_), rev_mass (rmass){}
	virtual ~Body();


	inline void applayMotion (float dt)
	{
		pos += vel*dt + force*rev_mass*dt*dt/2;
		vel += force*rev_mass*dt;
		force = Vector2f();
		applResistance (resistance());
		Vector2f resdv = resist*rev_mass*dt;
		if (resdv.lenSq() > vel.lenSq())
			vel -= vel.proj(resdv);
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
	inline Vector2f getImpulse()
	{
		if (rev_mass) return vel/rev_mass;
		return vel*VeryBigMass;
	}

	inline void applForce (Vector2f f) {force += f;}
	inline void applResistance (Vector2f f) {resist += f;}
	inline void applForce (Vector2f f, bool resistancive)
	{
		if (resistancive) applResistance(f);
		else applForce (f);
	}
	inline void applImpulce (Vector2f imp) {vel += imp*rev_mass;}

	inline bool ok() const {return rev_mass > 0;}
};

class Dir_body :public Body
{
	friend class Car;
protected:
	Orient orient;
	Vector2f fric;//[0] = x = parallel, [1] = y = normal friction

	virtual Vector2f resistance()
	{
		Vector2f dir = orient.getDir();
		Vector2f proj = (getVel()^dir)*dir;

		return -fric[0]*proj - fric[1]*(getVel() - proj);
	}
public:
	Dir_body (float rmass, float r_, Vector2f coor, Vector2f friction, Orient start_orient)
		:Body (rmass, r_, coor), orient (start_orient), fric (friction) {}

	Vector2f getResist (Vector2f velocity)
	{
		Vector2f dir = orient.getDir();
		Vector2f proj = (velocity^dir)*dir;
		return -fric[0]*proj - fric[1]*(velocity - proj);
	}

	inline bool ok() const {return Body::ok() && fric.x > 0 && fric.y > 0 && orient.ok();}
};

//const float Angular_vel = 5;
const float Max_ang_dev = 1.;
//const float Rudder_spring = 8;
//const float Bouncy = 2.0;

class Player;
class Effects_manager;
class Sprite;
const int Health_indicator_len = 50;
const int Health_indicator_offset = -20;
const int Health_indicator_height = 5;

class Car :public Active
{
	Player* host;
	Effects_manager* em;

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
	float turn_transfer;

	Sprite* pic;
        
	static float max_health;

	void processGestures (float dt);
	void motoryForce (float f);
	void turnFront (float dfi);
	void normalizeFrontOrient();
	void rudderCorrection (float dt);

	void applayBrdCollision (Collision_vector cv, float fric);
	void applayObjCollision (Car* with, Collision_vector cv);

	static const bool Back_rect = true;
	static const bool Front_rect = false;
	void applaySandFriction (Collision_vector cv, float fric, bool bORf);

	void move (Vector2f disp) {back.pos += disp; front.pos += disp;}

protected:
	virtual int signDataLen() const;

public:
	Car (Vector2f pos, float health, float motor_force, float bouncy, float angular_vel, float rudder_spring,
		 float rmass1, float rmass2, float lenght, float r1, float r2, float turn_transfer, Vector2f fric,
		 Orient start_orient, int id_ = 0,
		 Sprite* pic = 0, Player* host = 0, Effects_manager* em_ = 0);

	~Car();

	virtual void actions (float dt);
	virtual void draw (Canvas*);
	virtual void collisObj (Active* that);
	virtual void collisBrd (Rect with, float fric);
	virtual void driveSand (Rect width, float fric);

	void getMyVerticies (Vector2f* four);
	void getMyFrontVerticies (Vector2f* four);
	void getMyBackVerticies (Vector2f* four);
	Vector2f getVel (Vector2f p);
	Vector2f getImp (Vector2f p);
	bool damage (Vector2f imp, Vector2f papp, float destructive_k);			//returns true if killed
	void applImpulse (Vector2f imp, Vector2f papp, float destructive_k = 2);
	void applForce (Vector2f f, Vector2f papp, bool resistancive);

	virtual int exp (char* buffer, int size) const;
	virtual int imp (char* buffer, int size);
	
	virtual int myType() const {return Car_type;}
	virtual bool dead() const;
	virtual void die();

//	Vector2f Collis_rectangle (Vector2f one, Vector2f two, Vector2f three, Vector2f four);

	void turnRights()	{rp = true;}
	void turnLefts()	{lp = true;}
	void forwards()		{fp = true;}
	void backwards()	{bp = true;}
							//s = start procedure, f = finish
	void turnRightf()	{rp = false;}
	void turnLeftf()	{lp = false;}
	void forwardf()		{fp = false;}
	void backwardf()	{bp = false;}

	bool ok() const;
};
void DBG_switch();
#endif	/* _BODY_H */

