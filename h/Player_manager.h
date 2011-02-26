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
class GUEventman;
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
	inline Car* getCar() const {return mobile;}
	
	void addFrag() {frags++;}
	void subFrag() {frags--;}
	int getFrags() const {return frags;}
	string getName() const {return name;}

	int mySize() const;
	int exp (char* buffer, int size) const;
	int imp (char* buffer, int size);

	inline Control* getControl() const {return ks;}
	
	bool isPoor() const {return mobile == 0;}
	void carCrushed() {if (ks) ks->carCrushed (mobile); mobile = 0;}
};

class Player_manager :public list<Player*>, public Transmitted
{

	class Initializer;
	Initializer* parser;

	int cam_target;//!!! It isn't synchronized in network game

	void erase (iterator start, iterator finish);

public:
	Player_manager (GUEventman* sense);
	virtual ~Player_manager();

	Serializator* getParser();
	void drawCompTable (Canvas* where, Fontc* font);

	int exportEvents (char* buffer, int size) const;
	int importEvents (char* buffer, int size);
	void clearEvents();

	void setFirst2CamTarget()    {cam_target = empty()? -1 : (*begin())->id();}
	const Player* getCamTarget() const { return cam_target > 0 ? get(cam_target) : 0;}
	Player* get (int id) const;

	int exp (char* buffer, int size) const;
	int imp (char* buffer, int size);

	void createCarsForPoors (Carman* shop, Activeman* dest, Battlefield* site);

};

#endif	/* _PLAYER_MANAGER_H */

