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

using std::list;
using std::string;

class Eventman;
class Car;
class Player;
class Key_storage;
class Sprite;
class Activeman;
class Player_manager;

class Car_creator
{
public:
	string model_name;
	int model_id;

	Eventman* sense;

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

	Sprite* picture;

public:
	Car_creator (Eventman* sens);
	Car* New_car (Vector2f pos, Orient start_orient, Player* host, int id) const;

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

		int Export (char* buffer, int size) const;
		int Import (char* buffer, int size);
	};
	
	list<Car_creator*> models;//May be it needed vector here for speed???

	list<Creation> last_creations;

	typedef list<Car_creator*>::iterator iter;
	typedef list<Car_creator*>::const_iterator citer;

	class Initialaiser;
	Initialaiser* parser;

	Player_manager* hosts;
	Activeman* objs;

public:
	Carman (Eventman* sense);
	virtual ~Carman();

	Serializator* Get_parser();

	void Add_model (Car_creator* m) {models.push_back(m);}
	
	Car* Create_car (int model, Vector2f pos, Orient start_orient, Player* host, int id = 0);
	void Clear_last_creations();
	
	void Set_pm (Player_manager* pm) {hosts = pm;}
	void Set_am (Activeman* am) {objs = am;}

	int Export (char* buffer, int size) const;
	int Import (char* buffer, int size);

};

#endif	/* _CARMAN_H */

