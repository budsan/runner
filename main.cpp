#include "runner.h"
#include "guyframework/environment.h"

int main(int argc, char *argv[])
{
	Guy::Game* game = new Runner();
	Guy::Environment &env = Guy::Environment::instance();

	if (env.init(game)) env.run();

	delete game;
	env.destroy();

	return 0;
}
