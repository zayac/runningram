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
#include "Transmitted.h"
#include "Control.h"
#include "Key_event.h"
#include "Vec.h"
#include "Identified.h"

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
class Control;

struct Player :public Identified<Player>
{
	int frags;
	int prefered_model;
	string name;
	Car* mobile;
	Control* ks;
	
public:
	Player (string name, int pref_model, Control* ks);
	~Player() {delete ks;}
	
	Car* Create_car (Carman* shop, Vector2f where);
	
	void Add_frag() {frags++;}
	void Sub_frag() {frags--;}
	int Get_frags() const {return frags;}
	string Get_name() const {return name;}

	int My_size() const;
	int Export (char* buffer, int size) const;
	int Import (char* buffer, int size);

	inline Control* Get_control() const {return ks;}
	
	bool Is_poor() const {return mobile == 0;}
	void Car_crashed() {if (ks) ks->Car_crushed (mobile); mobile = 0;}
};

class Player_manager :public list<Player*>, public Transmitted
{

	class Initialaiser;
	Initialaiser* parser;

	void Erase (iterator start, iterator finish);

public:
	Player_manager (Eventman* sense);
	virtual ~Player_manager();

	Serializator* Get_parser();
	void Draw_comp_table (Canvas* where, Fontc* font);

	int Export_events (char* buffer, int size) const;
	int Import_events (char* buffer, int size);
	void Clear_events();

	Player* Get (int id) const;

	int Export (char* buffer, int size) const;
	int Import (char* buffer, int size);

	void Create_cars_for_poors (Carman* shop, Activeman* dest, Battlefield* site);

};

#endif	/* _PLAYER_MANAGER_H */

