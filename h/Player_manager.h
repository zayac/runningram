/* 
 * File:   Player_manager.h
 * Author: necto
 *
 * Created on February 18, 2010, 7:16 PM
 */

#ifndef _PLAYER_MANAGER_H
#define	_PLAYER_MANAGER_H
#include <list>
#include <string>
#include "Key_event.h"
#include "Vec.h"

using std::string;
using std::list;

class Carman;
class Activeman;
class Serializator;
class Car;
class Canvas;
class Fontc;
class Battlefield;
class Eventman;


struct Key_storage
{
	Key_id up;
	Key_id down;
	Key_id left;
	Key_id right;

	void Set_control (Car*, Eventman*);
};

struct Player
{
	int id;
	int frags;
	int prefered_model;
	string name;
	Car* mobile;
	Key_storage ks;

	static int max_id;
	
public:
	Player (string name, int pref_model, const Key_storage& ks);
	
	Car* Create_car (Carman* shop, Vector2f where);
	
	void Add_frag() {frags++;}
	void Sub_frag() {frags--;}
	int Get_frags() const {return frags;}
	string Get_name() const {return name;}

	int My_size() const;
	int Export (char* buffer, int size) const;
	int Import (char* buffer, int size);

	inline int Id() const {return id;}

	Key_storage Get_control() const {return ks;}
	
	bool Is_poor() const {return mobile == 0;}
	void Car_crashed() {mobile = 0;}
};

class Player_manager :public list<Player*>
{

	class Initialaiser;
	Initialaiser* parser;

public:
	Player_manager();
	virtual ~Player_manager();

	Serializator* Get_parser();
	void Draw_comp_table (Canvas* where, Fontc* font);

	Player* Get (int id) const;

	int Export (char* buffer, int size) const;
	int Import (char* buffer, int size);

	void Create_cars_for_poors (Carman* shop, Activeman* dest, Battlefield* site);

};

#endif	/* _PLAYER_MANAGER_H */

