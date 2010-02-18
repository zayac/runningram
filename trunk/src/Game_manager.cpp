/* 
 * File:   Game_manager.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:45 PM
 */

#include <SDL/SDL.h>

#include "Console.h"
#include "Game_manager.h"
#include "Graphic_subsystem.h"
#include "Eventman.h"
#include "initparser.h"
#include "Camera.h"
#include "Functor.h"
#include "Battlefield.h"
#include "Console.h"
#include "Car.h"
#include "Activeman.h"
#include "Carman.h"
#include "Player_manager.h"

Game_manager::Game_manager (int argc, char *argv[])
:pic (new Graphic_subsystem), sense (new Eventman), look(new Camera), ground (new Battlefield),
 cmd (new Console), cars (new Activeman), players (new Player_manager)
{
	models = new Carman (sense);

	Init (argc, argv);
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
Game_manager::~Game_manager()
{
	if (!players) delete players; players = 0;
	if (!ground) delete ground; ground = 0;
	if (!models) delete models; models = 0;
	if (!sense) delete sense; sense = 0;
	if (!look) delete look; look = 0;
	if (!cars) delete cars; cars = 0;
	if (!cmd) delete cmd; cmd = 0;
	if (!pic) delete pic; pic = 0;
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Init (int argc, char *argv[])
{
    Sectionp gen ("gensec", '\n');
	gen.Add_param (pic->Get_parser ());
	gen.Add_param (ground->Get_parser ());
	gen.Add_param (models->Get_parser ());
	gen.Add_param (players->Get_parser ());
    File_loader fl ((char*)"./settings.cfg");
    fl.Read_sector (&gen);
	
	sense->Register_key_action (new Arg_Method<void, void, Eventman> (sense, &Eventman::Switch_console),
																			EV_KEYDOWN, KI_BACKQUOTE);

	bool result = true;

	players->Create_cars_for_poors (models, cars);
	look->Set_target (*cars->begin ());

//	Car* ncar = models->Create_car (2,Vector2f (50, 50), 0);
//	look->Set_target (ncar);
//	cars->push_back (ncar);
//
//	cars->push_back (models->Create_car (1, Vector2f (150, 50), 0));

	result = result && pic->Init();
	result = result && ground->Init();
	result = result && cmd->Init (pic);
	
	sense->Set_console (cmd);

	return result && Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Main_loop()
{
	unsigned int last_time = SDL_GetTicks ();
	float dt = 0;
    while (!sense->Stopped())
    {
		sense->Acts();
		look->Actions();
		dt = 0.001*(SDL_GetTicks() - last_time);
		cars->Activate (dt);
				cars->Collis_brd (ground);
                cars->Process_collisions();

		last_time = SDL_GetTicks();
		ground->Draw (pic);
		cars->Draw (pic->Get_screen ());
		if (sense->Console_enabled ()) cmd->Draw (pic);

		pic->Draw (look);
    }
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Cleanup()
{
	pic->Cleanup();
	cmd->Cleanup();
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Ok() const
{
	return	(pic != 0) && pic->Ok() &&
			(sense != 0)  && sense->Ok() &&
			(ground != 0)  && ground->Ok() &&
			(look != 0) && look->Ok() &&
			(cmd != 0) && cmd->Ok() &&
			(cars != 0) && cars->Ok();
}
//--------------------------------------------------------------------------------------------------