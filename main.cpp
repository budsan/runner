#include "runner.h"

int main(int argc, char *argv[])
{
	Game* game = new Runner();
        if (game->init()) game->run();
	delete game;

	return 0;
}
