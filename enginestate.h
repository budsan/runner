#pragma once

#include "gameframework/gamestate.h"
#include "gameframework/graphics/camera2d.h"
#include "gameframework/graphics/tools/texthud.h"

#include "logic/player.h"
#include "logic/runnertilemap.h"

class EngineState : public GameState
{
public:
	 EngineState();
	~EngineState();

	void update(float deltaTime);
	void draw();
	
	void load();
	void unload();

private:

	void startTutorial();
	void reset();

	void playerHasJumped();
	void playerHasAirJumped();
	void playerHasDashed();

	bool tutorial_playing;
	bool tutorial_jumped;
	bool tutorial_airjumped;
	bool tutorial_dashed;
	float tutorial_countdown;

	TextHUD middleText;
	TextHUD scoreText;
	float score;

	Camera2D camera;
	RunnerTilemap tilemap;
	RunnerTilemap backmap;
	Player player;
};
