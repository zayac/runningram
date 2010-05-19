/**
  * @mainpage Running Ram: A simple action net game.
  * @version 0.3
  * @author Arseniy Zaostrovnih, Pavel Zaichenkov, Vasiliy Oreshko, Yekaterina Shulzenko
  * @date 19.05.10
  * 
  * There are many rams (or cars) on the buttlefield moving, and crashes to obstacles, and other rams. And it's the aim for players to breake as more as possible enemies's rams. The game processes on a server, and each player connects there, by using a client. Game uses the isometric projection in sprites to draw picture.
  * Game will be written using SDL libraries, so it can be transferred to any other platforms, not only linux.
  * Obstacles are the set of rectangle walls, described in special file. This file loading at the begin of level, and contain map of it, defined by characters in rectangle. Any character draws in background by texture, mentioned in this file. Before it will be drawn, any texture is transformed to isometric projection. Also this map file includes point of resurrection for rams.
  * Rams is a cars, conduct as staffs, with massive ends, and quadrangle solid shell. Their controllability is provided by difference of friction forces in different directions. They are smaller than one cell on playground. They draw with set of sprites, presenting a ram images in isometric projection. Rams have limited level of health, and it decrease, when they are crashed. Decrease of health depends on an impulse, and a point of it application. When one ram will destroy other, it scores one "frag". When ram has broken up, it ressurects in one of the ressurection points on map.
  *  Player can control rudder of ram and its engine. Using this he moves his ram on the ground. He can use the server keyboard, or connect to any server in net. 
  */

#include "Game_manager.h"

#include "Battlefield.h"
#include "Exception.h"
//--------------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	try
	{
		Game_manager gm (argc, argv);
		if (!gm.Ok()) return -1;
		gm.Main_loop();

		if (!gm.Cleanup()) return -1;
	}
	catch (Exception& ex)
	{
		ex.print ();
	}
	catch (...)
	{
		assert(0);
	}

    return 0;
}
//--------------------------------------------------------------------------------------------------
