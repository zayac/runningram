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

#include <Fontc.h>

#include "Exeption.h"//!!! deprecated

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

	Std_output_err* co;//for exeptions

	char buffer[1048576];

public:
	Game_manager (int argc, char *argv[]);
	virtual ~Game_manager();

	bool Init (int argc, char *argv[]);
	bool Main_loop();
	bool Cleanup();

	void tmpExport();
	void tmpImport();

	bool Ok() const;


Fontc font;//!!! deprecated, must be deleted
};

#endif	/* _GAME_MANAGER_H */

