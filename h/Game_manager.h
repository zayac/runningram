/* 
 * File:   Game_manager.h
 * Author: necto
 *
 * Created on January 7, 2010, 8:45 PM
 */

#ifndef _GAME_MANAGER_H
#define	_GAME_MANAGER_H

class Graphic_subsystem;
class Eventman;
class Camera;
class Battlefield;
class Console;
class Activeman;
class Carman;
class Player_manager;
class Console_output;
class Effects_manager;
class Button;

class Server;
class Client;
class lua_State;

#include <Fontc.h>

#include "Exception.h"//!!! deprecated

class Game_manager
{
	Graphic_subsystem *pic;
	Eventman *sense;
	Camera* look;
	Battlefield* ground;
	Console* cmd;
	Activeman* cars;
	Carman* models;
	Player_manager* players;
	Effects_manager* eff;
	Server* serv;
	Client* clie;

	Std_output_err* co;//for Exceptions

	Button* butt;

	char buffer[1048576];

	void Draw_fps (float dt) const;

public:
	Game_manager (int argc, char *argv[]);
	virtual ~Game_manager();

	bool init (int argc, char *argv[]);
	bool mainLoop();
	void fillZbuffer();
	bool cleanup();
	int stop (lua_State*);
	

	void tmpExport();
	void tmpImport();

	void getServerContext();
	void sendContext();

	bool ok() const;


Fontc font;//!!! deprecated, must be deleted
};

#endif	/* _GAME_MANAGER_H */

