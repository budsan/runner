#pragma once
#include "guyframework/game.h"

#include "guyframework/graphics/tools/texthud.h"
#include "guyframework/graphics/tools/frameshud.h"
#include "guyframework/graphics/camera2d.h"
#include "guyframework/tools/transition.h"
#include "guyframework/input/input.h"
#include "guyframework/input/keyboardlistener.h"

#include "actionsrunner.h"
#include "logic/runnertilemap.h"
#include "logic/player.h"

class Runner : public Guy::Game, public Guy::FocusListener, public Guy::KeyboardListener
{
public:
	Runner();
    virtual ~Runner();

protected:
	void init();

	void   load();
	void unload();

	void update(double deltaTime);
	void draw();

	virtual const char *name();
	virtual const char *version();

	void onGainInputFocus();
	void onLoseInputFocus();

	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);

	void startTutorial();
	void reset();

	void playerHasJumped();
	void playerHasAirJumped();
	void playerHasDashed();

	bool tutorial_playing;
	bool tutorial_jumped;
	bool tutorial_airjumped;
	bool tutorial_dashed;

	bool anyKeyDown;

	Guy::TextHUD middleText;
	Guy::TextHUD scoreText;
	double score;
	int showScore;

	Guy::Camera2D camera;
	RunnerTilemap tilemap;
	RunnerTilemap backmap;
	Player player;

	emyl::stream music;

	double countdown;
	bool resetTransition;
	Guy::TransitionLinear linear;

	Guy::FramesHUD frames;
};

