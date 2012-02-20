#include "runner.h"

int main(int argc, char *argv[])
{
	Game* game = new Runner();
        if (game->Init()) game->Run();
	delete game;

	return 0;
}
