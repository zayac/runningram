#include "Game_manager.h"

#include "Battlefield.h"
//--------------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	Game_manager gm (argc, argv);
	if (!gm.Ok()) return -1;
	gm.Main_loop();

    if (!gm.Cleanup()) return -1;

    return 0;
}
//--------------------------------------------------------------------------------------------------