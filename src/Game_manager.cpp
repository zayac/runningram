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
#include "Effects_manager.h"

#include "Client.h"
#include "Server.h"

enum NET_STATUS
{
	net0,
	netclient,
	netserver
} nstate = net0;

Game_manager::Game_manager (int argc, char *argv[])
: pic (new Graphic_subsystem), sense (new Eventman), look (new Camera), ground (new Battlefield),
cmd (new Console), cars (new Activeman), clie (new Client), models (new Carman)
, eff (new Effects_manager)
{
	co = new Output_cerr;
	Exception::setOutput (co);

	players = new Player_manager (sense);

	init (argc, argv);
	assert (ok ());
}
//--------------------------------------------------------------------------------------------------

Game_manager::~Game_manager ()
{
	if (players) delete players;
	players = 0;
	if (ground) delete ground;
	ground = 0;
	if (models) delete models;
	models = 0;
	if (sense) delete sense;
	sense = 0;
	if (look) delete look;
	look = 0;
	if (cars) delete cars;
	cars = 0;
	if (cmd) delete cmd;
	cmd = 0;
	if (pic) delete pic;
	pic = 0;
	if (co) delete co;
	co = 0;
	if (eff) delete eff;
	eff = 0;

	if (serv) delete serv;
	serv = 0;
	if (clie) delete clie;
	clie = 0;
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
		gen.addParam (pic->getParser ());
		File_loader fl ((char*) "./settings.cfg");
		fl.readSector (&gen);
		result = result && pic->init (); //Graphic subsystem must be initialaised previously
		pic->splashScreen ();

		gen.addParam (cmd->getParser ());
		gen.addParam (ground->getParser ());
		gen.addParam (models->getParcer ());
		gen.addParam (players->getParser ());
		gen.addParam (eff->getParser ());
		fl.readSector (&gen);

		models->setAM (cars);
		int ggg = cars->size ();
		models->setPM (players);
		models->setEM (eff);

		sense->registerKeyOper (new Arg_Method<void, Kbd_event, Console > (cmd, &Console::operate));
		sense->registerKeyAction (new Arg_Method<void, void, Console > (cmd, &Console::turn),
								EV_KEYDOWN, KI_BACKQUOTE);

		sense->registerKeyAction (new Arg_Function<void, void> (DBG_switch),
								  EV_KEYDOWN, KI_s);
		sense->registerKeyAction (new Arg_Method<void, void, Game_manager > (this, &Game_manager::tmpExport),
								EV_KEYDOWN, KI_z);
		sense->registerKeyAction (new Arg_Method<void, void, Game_manager > (this, &Game_manager::tmpImport),
								EV_KEYDOWN, KI_x);


		font.openfont ("default.ttf", 16);
		font.setFG (Color (100, 100, 200)); //!!! deprecated

		result = result && cmd->init (pic);
		result = result && ground->init ();
		result = result && eff->init ();

		cmd->registerProcessor ("quit", new Arg_Method <int, lua_State*, Game_manager > (this, &Game_manager::stop));

		//                result = result && eff->Init();
		//		if (co) delete co;
		//		co = new Console_output (cmd);
		//		Exception::Set_output (co);


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

bool Game_manager::mainLoop ()
{
	try
	{
		unsigned int last_time = SDL_GetTicks ();
		float dt = 0;


		while (!sense->stopped ())
		{
			sense->acts ();
			dt = 0.001 * (SDL_GetTicks () - last_time);
			cars->activate (dt);

			if (nstate != netclient)
			{
				cars->collisBrd (ground);
				cars->processCollisions ();
			}

			last_time = SDL_GetTicks ();

			fillZbuffer ();
			ground->draw (pic);
			cmd->draw (pic);

			eff->expDraw (pic->getScreen ());
			eff->expClean ();

			players->drawCompTable (pic->getScreen (), &font);

			if (!look->hasTarget () && cars->size () > 0) look->setTarget (*cars->begin ());
			look->actions ();
			cars->deleteDeadalives ();

			Draw_fps (dt);

			pic->draw (look);
			if (nstate != netclient) players->createCarsForPoors (models, cars, ground);

			if (nstate == netclient) getServerContext ();
			if (nstate == netserver) sendContext ();
			models->clesrLastActions ();
			players->clearEvents ();
		}
	}
	catch (Exception& ex)
	{
		ex.print ();
	}
	return ok ();
}
//--------------------------------------------------------------------------------------------------
void Game_manager::fillZbuffer ()
{
	ground->cleanZbuffer ();
	for (Activeman::iterator i = cars->begin (); i != cars->end (); ++i)
		ground->addDrawable (Canvas::transform ((**i).getPos ()).y - (**i).getHeight () / 2, *i);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::Draw_fps (float dt) const
{
	Point screen_pos = pic->getScreen ()->getPos ();

	static int frames = 0;
	static int fps = 0;
	static int last_time = SDL_GetTicks ();

	//	static float mid_dt = dt;
	//	mid_dt = (49*mid_dt + dt)/50;

	++frames;
	if (SDL_GetTicks () - last_time > 100)
	{
		fps = (10 * frames + fps * 9) / 10;
		frames = 0;
		last_time = SDL_GetTicks ();
	}

	Rect brd (screen_pos.x + 10, screen_pos.y + 10, 100, 100);
	char buf[128];
	sprintf (buf, "fps: %.2f", (float) fps);
	font.drawLine (pic->getScreen (), buf, &brd);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::tmpExport () {
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

void Game_manager::tmpImport () {
	//	cars->Import (buffer, 1048576);
	//	players->Import (buffer, 1048576);
	//	models->Import (buffer, 1048576);
	//	players->Clear_events();
	//	players->Import_events (buffer, 1048576);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::getServerContext ()
{
	char buffer[Buffer_size];
	clie->receiveNext (buffer, Buffer_size);
}
//--------------------------------------------------------------------------------------------------

void Game_manager::sendContext ()
{
	serv->sendNext ();
}
//--------------------------------------------------------------------------------------------------

bool Game_manager::cleanup ()
{
	return ok ();
	pic->cleanup ();
	cmd->cleanup ();
}
//--------------------------------------------------------------------------------------------------

int Game_manager::stop (lua_State*)
{
	sense->stop ();
	return 0;
}
//--------------------------------------------------------------------------------------------------

bool Game_manager::ok () const
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