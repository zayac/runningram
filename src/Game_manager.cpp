/* 
 * File:   Game_manager.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:45 PM
 */

#include <SDL/SDL.h>
#include "Game_manager.h"
#include "Graphic_subsystem.h"
#include "Eventman.h"
#include "initparser.h"
#include "Camera.h"
#include "Functor.h"

Game_manager::Game_manager (int argc, char *argv[])
:canvas (new Graphic_subsystem), sense (new Eventman), look(new Camera)
{
	Init (argc, argv);
}
//--------------------------------------------------------------------------------------------------
Game_manager::~Game_manager()
{
	if (!canvas) delete canvas;
	if (!sense) delete sense;
	if (!look) delete look;
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Init (int argc, char *argv[])
{
//	if (!canvas) canvas = new Graphic_subsystem;

    Sectionp gen ("gensec", '\n');
	gen.Add_param (canvas->Get_parser ());
    File_loader fl ("./settings.cfg");
    fl.Read_sector (&gen);

	sense->Register_key_action (new Arg_Method<void, void, Camera> (look, &Camera::Move_left), SDL_KEYDOWN, SDLK_LEFT);
	sense->Register_key_action (new Arg_Method<void, void, Camera> (look, &Camera::Move_right), SDL_KEYDOWN, SDLK_RIGHT);
	sense->Register_key_action (new Arg_Method<void, void, Camera> (look, &Camera::Move_up), SDL_KEYDOWN, SDLK_UP);
	sense->Register_key_action (new Arg_Method<void, void, Camera> (look, &Camera::Move_down), SDL_KEYDOWN, SDLK_DOWN);

	canvas->Init();
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Main_loop()
{
    int gameover = 0;
    /* message pump */
    while (!sense->Stopped())
    {
		sense->Acts();
		canvas->Draw (look);
    }
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Cleanup()
{
	canvas->Cleanup();
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Ok() const
{
	return (canvas != 0) && canvas->Ok() &&
		   (sense != 0)  && sense->Ok();
}
//--------------------------------------------------------------------------------------------------