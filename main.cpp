#include "Game_manager.h"

#include "Battlefield.h"
#include "Exception.h"

extern "C"
{
	#include "lua/include/lua.h"
	#include "lua/include/lualib.h"
	#include "lua/include/lauxlib.h"
}
//--------------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	try
	{
		lua_State* luaVM = luaL_newstate ();//lua_open();
//		cout <<"dfte";
		lua_close (luaVM);

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