/* 
 * File:   Game_manager.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:45 PM
 */

#include "Game_manager.h"
#include "Graphic_subsystem.h"
#include "Eventman.h"
#include "initparser.h"

Game_manager::Game_manager (int argc, char *argv[])
:canvas (new Graphic_subsystem), sense (new Eventman)
{
	Init (argc, argv);
}
//--------------------------------------------------------------------------------------------------
Game_manager::~Game_manager()
{
	Cleanup();
	if (!canvas) delete canvas;
	if (!sense) delete sense;
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Init (int argc, char *argv[])
{
//	if (!canvas) canvas = new Graphic_subsystem;

    Sectionp gen ("gensec", '\n');
	gen.Add_param (canvas->Get_parser ());
    File_loader fl ("./settings.cfg");
    fl.Read_sector (&gen);

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
		canvas->Draw ();
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