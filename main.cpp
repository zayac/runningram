#include "Game_manager.h"

#include "Battlefield.h"
#include "Exception.h"
#include "Console.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
//--------------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	try
	{
		Game_manager gm (argc, argv);
		if (!gm.ok()) return -1;
		gm.mainLoop();

		if (!gm.cleanup()) return -1;
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