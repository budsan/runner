#include "runner.h"
#include "gameframework/environment.h"

int main(int argc, char *argv[])
{
	Game* game = new Runner();
	Environment &env = Environment::instance();

	if (env.init(game)) env.run();

	delete game;
	env.destroy();

	return 0;
}
