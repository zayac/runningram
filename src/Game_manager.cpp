/* 
 * File:   Game_manager.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:45 PM
 */

#include <unistd.h>
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

#include "Client.h"
#include "Server.h"
enum NET_STATUS
{
	net0,
	netclient,
	netserver
} nstate = net0;

Game_manager::Game_manager (int argc, char *argv[])
:pic (new Graphic_subsystem), sense (new Eventman), look(new Camera), ground (new Battlefield),
 cmd (new Console), cars (new Activeman), players (new Player_manager),
 clie (new Client)
{
	co = new Output_cerr;
	Exception::Set_output (co);

	models = new Carman (sense);

	if (nstate == netserver) serv =new Server(4334);

	Init (argc, argv);
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
Game_manager::~Game_manager()
{
	if (players) delete players; players = 0;
	if (ground) delete ground; ground = 0;
	if (models) delete models; models = 0;
	if (sense) delete sense; sense = 0;
	if (look) delete look; look = 0;
	if (cars) delete cars; cars = 0;
	if (cmd) delete cmd; cmd = 0;
	if (pic) delete pic; pic = 0;
	if (co) delete co; co = 0;
	
	if (serv) delete serv; serv = 0;
	if (clie) delete clie; clie = 0;
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Init (int argc, char *argv[])
{
	bool result = true;
	try
	{
	    Sectionp gen ("gensec", '\n');
		gen.Add_param (pic->Get_parser ());
		File_loader fl ((char*)"./settings.cfg");
	    fl.Read_sector (&gen);
		result = result && pic->Init();			//Graphic subsystem must be initialaised previously
                pic->SplashScreen();
//		gen.Delete_props ();
		gen.Add_param (cmd->Get_parser ());
		gen.Add_param (ground->Get_parser ());
		gen.Add_param (models->Get_parser ());
		gen.Add_param (players->Get_parser ());
		fl.Read_sector (&gen);

                sense->Register_key_action (new Arg_Function<void, void> (DBG_switch), EV_KEYDOWN, KI_s);

		sense->Register_key_action (new Arg_Method<void, void, Console> (cmd, &Console::Switch),
																			EV_KEYDOWN, KI_BACKQUOTE);
		sense->Register_key_oper (new Arg_Method<void, Kbd_event, Console> (cmd, &Console::Operate));

		sense->Register_key_action (new Arg_Method<void, void, Game_manager> (this, &Game_manager::tmpExport),
																			EV_KEYDOWN, KI_z);
		sense->Register_key_action (new Arg_Method<void, void, Game_manager> (this, &Game_manager::tmpImport),
																			EV_KEYDOWN, KI_x);


		font.Open_font ("default.ttf", 16);
		font.Set_fg (Color(100, 100, 200));//!!! deprecated

	    result = result && cmd->Init (pic);
		result = result && ground->Init();

//		if (co) delete co;
//		co = new Console_output (cmd);
//		Exception::Set_output (co);
		
		switch (nstate)
		{
		case netclient:
			clie->Connect ("localhost", 4334);
			break;
		case netserver:
			serv->accept_one();
			break;
		};
                
	}
	catch (Exception& ex)
	{
		ex.print();
	}

	return result && Ok();
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Main_loop()
{
	try
	{
		unsigned int last_time = SDL_GetTicks ();
		float dt = 0;
		
		while (!sense->Stopped ())
		{
			sense->Acts ();
			dt = 0.001 * (SDL_GetTicks () - last_time);
			cars->Activate (dt);
			cars->Collis_brd (ground);
			cars->Process_collisions ();

			last_time = SDL_GetTicks ();
			
			
			
			ground->Draw (pic);
			cars->Draw (pic->Get_screen ());
			cmd->Draw (pic);

			players->Draw_comp_table (pic->Get_screen (), &font);

			if (!look->Has_target ()) look->Set_target (*cars->begin ());
			look->Actions ();
			bool found = cars->Delete_deadalives ();

			Draw_fps (dt);

			pic->Draw (look);
			players->Create_cars_for_poors (models, cars, ground);
			if (nstate == netclient) Get_server_context();
			if (nstate == netserver) Send_context();
		}
	}
	catch (Exception& ex)
	{
		ex.print();
	}
	return Ok();
}
//--------------------------------------------------------------------------------------------------
void Game_manager::Draw_fps (float dt) const
{
	Point screen_pos = pic->Get_screen()->getPos ();

	static float mid_dt = dt;
	mid_dt = (49*mid_dt + dt)/50;

	Rect brd (screen_pos.x + 10, screen_pos.y + 10, 100, 100);
	char buf[128];
	sprintf (buf, "fps: %.2f", 1/mid_dt);
	font.Draw_line (pic->Get_screen(), buf, &brd);
}
//--------------------------------------------------------------------------------------------------
void Game_manager::tmpExport()
{
	cars->Export (buffer, 1048576);
//	players->Export (buffer, 1048576);
	Activeman::iterator i, j;
	i = cars->begin ();
	j = cars->begin();
	++j;
	std::swap (*i, *j);

}
//--------------------------------------------------------------------------------------------------
void Game_manager::tmpImport()
{
	cars->Import (buffer, 1048576);
//	players->Import (buffer, 1048576);
}
//--------------------------------------------------------------------------------------------------
void Game_manager::Get_server_context()
{
//	static bool confirmed = false;
//	if (!confirmed)
		clie->Confirm (1);
	int received = clie->Receive (buffer, 1048576);
//	cmd->Push_string (buffer);
	if (received != 0)
	{
		int ccsize = cars->Import (buffer, received);
		players->Import (buffer + ccsize, received - ccsize);
//		confirmed = true;
	}
//	else confirmed = false;
}
//--------------------------------------------------------------------------------------------------
void Game_manager::Send_context()
{
//	static int guri = 0;
//	guri++;
//	sprintf (buffer, "guri: %d", guri);
//	int size = strlen (buffer);
	int conf = serv->Get_confirmation ();
	if (conf != 1) throw Exception ("wrong confirmation");

	int size = cars->Export (buffer, 1048576);
	size += players->Export (buffer + size, 1048576 - size);
	serv->send (buffer, size);
}
//--------------------------------------------------------------------------------------------------
bool Game_manager::Cleanup()
{
	return Ok();
	pic->Cleanup();
	cmd->Cleanup();
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