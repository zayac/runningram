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
#include "GUEventman.h"
#include "initparser.h"
#include "Camera.h"
#include "Functor.h"
#include "Battlefield.h"
#include "Console.h"
#include "Car.h"
#include "Activeman.h"
#include "Carman.h"
#include "Player_manager.h"
#include "Effects_manager.h"
#include "Interpreter.h"
#include "GUI.h"
#include "Key_storage.h"
#include "MouseTargeter.h"
#include "Client.h"
#include "Server.h"
#include "Button.h"
#include "GUI.h"
#include "Timer.h"
#include "BotControl.h"
#include "DebugHelper.h"

enum NET_STATUS
{
	net0,
	netclient,
	netserver
} nstate = net0;

Game_manager::Game_manager (int argc, char *argv[])
: pic (new Graphic_subsystem), sense (GUEventman::getInstance()), look (new Camera), ground (new Battlefield),
cmd (new Console), cars (new Activeman), clie (new Client), models (new Carman),
eff (new Effects_manager), interp(Interpreter::create(argc, argv)), gui(new GUI), time (Timer::getInstance()),
state (RUNNING), show_frag_table (false)
{
	co = new Output_cerr;
	Exception::setOutput (co);
	Key_storage::preInit (sense);
	MouseTargeter::preInit (sense, time, pic);
	BotControl::preInit (time);

	players = new Player_manager();

	init (argc, argv);
	assert (ok ());
}
//--------------------------------------------------------------------------------------------------

Game_manager::~Game_manager ()
{
	if (players)	delete players;	players = 0;
	if (ground)		delete ground;	ground = 0;
	if (models)		delete models;	models = 0;
//	if (sense)		delete sense;	sense = 0; - deleting in GUEventman.cpp (singleton)
	if (look)		delete look;	look = 0;
	if (cars)		delete cars;	cars = 0;
	if (cmd)		delete cmd;		cmd = 0;
	if (pic)		delete pic;		pic = 0;
	if (co)			delete co;		co = 0;
	if (eff)		delete eff;		eff = 0;
	if (gui)		delete gui;		gui = 0;
	if (interp)		Interpreter::destroy(); interp = 0;

	if (serv)		delete serv;	serv = 0;
	if (clie)		delete clie;	clie = 0;
}
//--------------------------------------------------------------------------------------------------

bool Game_manager::init (int argc, char *argv[])
{
	if (argc > 1)
		if (strlen (argv[1]) < 8)
			nstate = netserver;
		else
			nstate = netclient;
	bool result = true;
	try
	{
		Sectionp gen ("gensec", '\n');
		gen.addParam (pic->newParser ());
		File_loader fl ((char*) "./settings.cfg");
		fl.readSector (&gen);
		result = result && pic->init (); //Graphic subsystem must be initialaised previously
		pic->splashScreen();
		DebugHelper::c = pic->getScreen();
		DebugHelper::showDBG = true;

		gen.addParam (cmd->newParser ());
		gen.addParam (ground->newParser ());
		gen.addParam (models->newParcer ());
		gen.addParam (players->newParser ());
		gen.addParam (eff->newParser ());
		gen.addParam (gui->newParser ());
		gen.addParam (look->newParcer());
		fl.readSector (&gen);

		models->setAM (cars);
		models->setPM (players);
		models->setEM (eff);

		sense->registerKeyOper (new Arg_Method<void, Kbd_event, Console > (cmd, &Console::operate));
		sense->registerKeyAction (new Arg_Method<void, void, Console > (cmd, &Console::turn),
								EV_KEYDOWN, KI_BACKQUOTE);

		sense->registerKeyAction (new Arg_Method<void, void, Game_manager>
										(this, &Game_manager::showFragTable),
										EV_KEYDOWN, KI_TAB);
		sense->registerKeyAction (new Arg_Method<void, void, Game_manager>
										(this, &Game_manager::hideFragTable),
										EV_KEYUP, KI_TAB);

		sense->registerKeyAction (new Arg_Function<void, void> (DBG_switch),
								  EV_KEYDOWN, KI_s);
		sense->registerKeyAction (new Arg_Method<void, void, Game_manager > (this, &Game_manager::tmpExport),
								EV_KEYDOWN, KI_z);
		sense->registerKeyAction (new Arg_Method<void, void, Game_manager > (this, &Game_manager::tmpImport),
								EV_KEYDOWN, KI_x);

		result = result && cmd	 ->init (pic, interp);
		result = result && ground->init (interp);
		result = result && eff	 ->init ();
		result = result && look  ->init ();
		result = result && gui	 ->init ();

		interp->regFun ("quit", new Arg_Method<void, void, Game_manager> (this, &Game_manager::stop));
		btl.init (sense);
		btl.addButton (new Arg_Method <void, void, GUEventman> (sense, &GUEventman::stop), Point(500, 433));
		btl.addButton (new Arg_Method <void, void, GUEventman> (sense, &GUEventman::stop), Point(400, 433));
		btl.addButton (new Arg_Method <void, void, GUEventman> (sense, &GUEventman::stop), Point(300, 433));

		switch (nstate)
		{
		case netclient:
			clie->connect (argv[1], 4334);
			clie->push_back (players);
			clie->push_back (cars);
			clie->push_back (models);
			clie->setPM (players);

			break;
		case netserver:
			serv = new Server (4334);

			int nclients = atoi (argv[1]);
			while (nclients-- > 0)
				serv->acceptOne ();

			serv->push_back (models);
			serv->push_back (players);
			serv->push_back (cars);
			serv->setPM (players);
			break;
		};

	}
	catch (Exception& ex)
	{
		ex.print ();
	}

	return result && ok ();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::mainLoop()
{
    try
    {
	float dt = 0;
	while (!sense->stopped())
	{
		switch (state)
		{
			case RUNNING:
				sense->acts();
				dt = time->elapsed();
				time->update();
				cars->activate (dt);

				if (nstate != netclient)
				{
					cars->collisBrd (ground);
					cars->processCollisions();
				}

				fillZbuffer();
				ground->draw (pic);
				gui->drawMiniMap (ground, cars, pic);
				cmd->draw (pic);

				eff->expDraw (pic->getScreen());
				eff->expClean();

				if (show_frag_table)
					pic->drawCompTable(players->getFragTable());

				if (!look->hasTarget() && cars->size() > 0)
					if (players->getCamTarget())
						look->setTarget (players->getCamTarget()->getCar());
					else
						look->setTarget (*cars->begin());
				look->actions();
				cars->deleteDeadalives();

				pic->drawFps (dt);

				btl.draw (*pic->getScreen());
				pic->draw (look);
				if (nstate != netclient)
					players->createCarsForPoors (models, cars, ground);

				if (nstate == netclient) getServerContext();
				if (nstate == netserver) sendContext();
				models->clesrLastActions();
				players->clearEvents();
				break;
			case WIN:
				pic->winScreen();
				pic->drawCompTable(players->getFragTable());
				break;
		}
	}
    }
    catch (Exception& ex)
    {
	ex.print ();
    }
    return ok ();
}
//--------------------------------------------------------------------------------------------------
void Game_manager::fillZbuffer()
{
	ground->cleanZbuffer ();
	for (Activeman::iterator i = cars->begin (); i != cars->end (); ++i)
		ground->addDrawable (Canvas::transform ((**i).getPos ()).y - (**i).getHeight () / 2, *i);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::tmpExport()
{
	//	cars->Export (buffer, 1048576);
	//	players->Export (buffer, 1048576);
	//	models->Export (buffer, 1048576);
	//	models->Clear_last_creations ();
	//	cars->clear ();
	//	players->Export_events (buffer, 1048576);
	//	players->Clear_events();
	//	Activeman::iterator i, j;
	//	i = cars->begin ();
	//	j = cars->begin();
	//	++j;
	//	std::swap (*i, *j);
 }
//--------------------------------------------------------------------------------------------------

void Game_manager::tmpImport()
{
	//	cars->Import (buffer, 1048576);
	//	players->Import (buffer, 1048576);
	//	models->Import (buffer, 1048576);
	//	players->Clear_events();
	//	players->Import_events (buffer, 1048576);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::getServerContext()
{
	char buffer[Buffer_size];
	clie->receiveNext (buffer, Buffer_size);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::sendContext()
{
	serv->sendNext ();
}
//--------------------------------------------------------------------------------------------------

bool Game_manager::cleanup()
{
	return ok ();
	pic->cleanup ();
	cmd->cleanup ();
}
//--------------------------------------------------------------------------------------------------

void Game_manager::stop()
{
	sense->stop ();
}
//--------------------------------------------------------------------------------------------------
void Game_manager::showFragTable()
{
	show_frag_table = true;
}
//--------------------------------------------------------------------------------------------------
void Game_manager::hideFragTable()
{
	show_frag_table = false;
}
//--------------------------------------------------------------------------------------------------

bool Game_manager::ok() const
{
	return (pic != 0) && pic->ok () &&
		(sense != 0) && sense->ok () &&
		(ground != 0) && ground->ok () &&
		(look != 0) && look->ok () &&
		(cmd != 0) && cmd->ok () &&
		(cars != 0) && cars->ok () &&
		(models != 0) && true;
}
//--------------------------------------------------------------------------------------------------
