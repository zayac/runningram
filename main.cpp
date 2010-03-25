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