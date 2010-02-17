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

using std::list;
using std::string;

class Eventman;
class Car;

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
	Key_id up;
	Key_id down;
	Key_id left;
	Key_id right;

public:
	Car_creator (Eventman* sens);
	Car* New_car (Vector2f pos, Orient start_orient) const;

	Car_creator* Create_copy() const;
};

class Carman
{
	list<Car_creator*> models;//May be it needed vector here for speed???

	typedef list<Car_creator*>::iterator iter;
	typedef list<Car_creator*>::const_iterator citer;

	class Initialaiser;
	Initialaiser* parser;

public:
	Carman (Eventman* sense);
//	Carman(const Carman& orig);
	virtual ~Carman();

	Serializator* Get_parser();

	void Add_model (Car_creator* m) {models.push_back(m);}
	
	Car* Create_car (int model, Vector2f pos, Orient start_orient) const;


};

#endif	/* _CARMAN_H */

