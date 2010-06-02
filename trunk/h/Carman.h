/* 
 * File:   Carman.h
 * Author: necto
 *
 * Created on February 16, 2010, 4:11 PM
 */

#ifndef _CARMAN_H
#define	_CARMAN_H
#include <list>
#include <string>
#include "Orient.h"
#include "initparser.h"
#include "Key_event.h"
#include "Player_manager.h"
#include "Effects_manager.h"

using std::list;
using std::string;

class Eventman;
class Car;
class Player;
class Key_storage;
class Sprite;
class Activeman;
class Player_manager;
class Effects_manager;

class Car_creator
{
public:
	string model_name;
	int model_id;

//	Eventman* sense;

	float start_health;
	float motor_force;
	float rmass1;
	float rmass2;
	float lenght;
	float r1;
	float r2;
	Vector2f fric;
	float bouncy;
	float angular_vel;
	float rudder_spring;
	float turn_transfer;

	Sprite* picture;

public:
	Car_creator();
	Car* New_car (Vector2f pos, Orient start_orient, Player* host, Effects_manager* em, int id) const;

	Car_creator* Create_copy() const;

	inline void setPicture (Sprite* pic) {picture = pic;}
};

class Carman: public Transmitted
{
	struct Creation
	{
		int model;
		Vector2f pos;
		Orient start_orient;
		int id;
		int pl_id;

		Creation();
		Creation (const Creation&);
		Creation (int model, Vector2f pos, Orient start_orient, int pl_id, int car_id);

		int exp (char* buffer, int size) const;
		int imp (char* buffer, int size);
	};
	
	list<Car_creator*> models;//May be it needed vector here for speed???

	list<Creation> last_creations;

	typedef list<Car_creator*>::iterator iter;
	typedef list<Car_creator*>::const_iterator citer;

	class Initialaiser;
	Initialaiser* parser;

	Player_manager* hosts;
	Effects_manager* effm;
	Activeman* objs;

public:
	Carman();
	virtual ~Carman();

	Serializator* getParcer();

	void addModel (Car_creator* m) {models.push_back(m);}
	
	Car* createCar (int model, Vector2f pos, Orient start_orient, Player* host, int id = 0);
	void clesrLastActions();
	
	void setPM (Player_manager* pm) {hosts = pm;}
	void setAM (Activeman* am) {objs = am;}
	void setEM (Effects_manager* em_) {effm = em_;}

	int exp (char* buffer, int size) const;
	int imp (char* buffer, int size);

};

#endif	/* _CARMAN_H */

