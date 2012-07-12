#pragma once
#include "gameframework/game.h"

#include "gameframework/graphics/tools/texthud.h"
#include "gameframework/graphics/tools/frameshud.h"
#include "gameframework/graphics/camera2d.h"
#include "gameframework/tools/transition.h"
#include "gameframework/input/input.h"
#include "gameframework/input/keyboardlistener.h"

#include "logic/runnertilemap.h"
#include "logic/action.h"
#include "logic/player.h"

class Runner : public Game, public Input::FocusListener, public KeyboardListener, public ActionManager
{
public:
	Runner();

protected:
	void init();

	void   load();
	void unload();

	void update(float deltaTime);
	void draw();

	virtual const char *getName();
	virtual const char *getVersion();

	void onGainInputFocus();
	void onLoseInputFocus();

	void onKeyUp(wchar_t unicode, Keyboard::Key key, Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Keyboard::Key key, Keyboard::Mod mods);

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

	TextHUD middleText;
	TextHUD scoreText;
	float score;
	int showScore;

	Camera2D camera;
	RunnerTilemap tilemap;
	RunnerTilemap backmap;
	Player player;

	emyl::stream music;

	float countdown;
	bool resetTransition;
	TransitionLinear linear;

	FramesHUD frames;
};

