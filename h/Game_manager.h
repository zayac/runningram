/* 
 * File:   Game_manager.h
 * Author: necto
 *
 * Created on January 7, 2010, 8:45 PM
 */

#ifndef _GAME_MANAGER_H
#define	_GAME_MANAGER_H

class Graphic_subsystem;
class GUEventman;
class Camera;
class Battlefield;
class Console;
class Activeman;
class Carman;
class Player_manager;
class Console_output;
class Effects_manager;
class Button;
class Interpreter;
class GUI;
class Timer;

class Server;
class Client;

#include <Fontc.h>

#include "Exception.h"
#include "Button.h"//!!! deprecated

enum GameState
{
	MENU,
	RUNNING,
	WIN
};

class Game_manager
{
	Graphic_subsystem *pic;
	GUEventman *sense;
	Camera* look;
	Battlefield* ground;
	Console* cmd;
	Activeman* cars;
	Carman* models;
	Player_manager* players;
	Effects_manager* eff;
	Server* serv;
	Client* clie;
	Interpreter* interp;
	GUI* gui;
	Timer* time;
	GameState state;
	bool show_frag_table;

	Std_output_err* co;//for Exceptions

	Btnlist btl;

	char buffer[1048576];

	void Draw_fps (float dt) const;

public:
	Game_manager (int argc, char *argv[]);
	virtual ~Game_manager();

	bool init (int argc, char *argv[]);
	bool mainLoop();
	void fillZbuffer();
	bool cleanup();
	void stop();
	void showFragTable();
	void hideFragTable();
	
	void tmpExport();
	void tmpImport();

	void getServerContext();
	void sendContext();

	bool ok() const;


Fontc font;//!!! deprecated, must be deleted
};

#endif	/* _GAME_MANAGER_H */

