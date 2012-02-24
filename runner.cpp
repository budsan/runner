#include "runner.h"

#include <iostream>

#include "gameframework/log.h"
#include "gameframework/settings.h"

#include "enginestate.h"
#include "keys.h"

#define GAME_NAME "Runner"
#define GAME_VERSION "1337"

Runner::Runner()
{

}

const char *Runner::getName()
{
    return GAME_NAME;
}

const char *Runner::getVersion()
{
    return GAME_VERSION;
}

void Runner::configure()
{
	setFramesPerSecond(0);
	setStableDeltaTime(false);
	changeState(new EngineState());

	Keybinds k(NUMPLAYERS, K_SIZE);
	k[0][K_JUMP].setDefault(SDLK_UP);
	k[0][K_DASH].setDefault(SDLK_RIGHT);

	m_settings->setKeybinds(k);
	m_settings->get("ScreenWidth" )->set(800);
	m_settings->get("ScreenHeight")->set(600);
	m_settings->get("Fullscreen"  )->set(false);
}

void Runner::load()
{
	if (!frames.loadFont("data/font/nibby.ttf"))
	{
		LOG << "ERROR: Loading frames font" << std::endl;
	}

	frames.setColor(rgba(1,1,1,1));
}

void Runner::unload()
{

}

void Runner::update(float deltaTime)
{
	frames.update(deltaTime);
}

void Runner::draw()
{
	frames.draw();
}


