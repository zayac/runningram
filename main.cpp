#include "Game_manager.h"

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