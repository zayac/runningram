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

using std::string;
using std::list;

class Carman;
class Activeman;
class Serializator;
class Car;

struct Player
{
	int frags;
	int preffered_model;
	string name;
	Car* mobile;
	
public:
	Player (string name, int pref_model);
	
	Car* Create_car (Carman* shop);
	
	void Add_frag() {frags++;}
	void Sub_frag() {frags--;}
	int Get_frags() const {return frags;}
	string Get_name() const {return name;}
	
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

	void Create_cars_for_poors (Carman* shop, Activeman* dest);

};

#endif	/* _PLAYER_MANAGER_H */

