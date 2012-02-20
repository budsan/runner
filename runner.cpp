#include "runner.h"

#include <iostream>

#include "gameframework/log.h"
#include "gameframework/settings.h"

#include "enginestate.h"
#include "keys.h"

#define GAME_NAME "Climbers"
#define GAME_VERSION "0.1"

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

void Runner::Configure()
{
	setFramesPerSecond(0);
	setStableGameTime(false);
	ChangeState(new EngineState());

	Keybinds k(NUMPLAYERS, K_SIZE);
	k[0][K_JUMP ].setDefault(SDLK_UP);

	mySettings->setKeybinds(k);
	mySettings->get("ScreenWidth" )->set(800);
	mySettings->get("ScreenHeight")->set(600);
	mySettings->get("Fullscreen"  )->set(false);
}

void Runner::Load()
{
	if (!frames.LoadFont("data/font/nibby.ttf"))
	{
		LOG << "ERROR: Loading frames font" << std::endl;
	}

	frames.setColor(rgba(1,1,0.2,1));
}

void Runner::Unload()
{

}

void Runner::Update(float GameTime)
{
	frames.Update(GameTime);
}

void Runner::Draw()
{
	frames.Draw();
}


