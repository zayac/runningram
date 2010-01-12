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

class Game_manager
{
	Graphic_subsystem *pic;
	Eventman *sense;
	Camera* look;
	Battlefield* ground;
	Console* cmd;
	Activeman* cars;

public:
	Game_manager (int argc, char *argv[]);
	virtual ~Game_manager();

	bool Init (int argc, char *argv[]);
	bool Main_loop();
	bool Cleanup();

	bool Ok() const;
};

#endif	/* _GAME_MANAGER_H */

