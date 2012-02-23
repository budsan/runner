#pragma once

#include "gameframework/gamestate.h"
#include "gameframework/graphics/camera2d.h"

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

	Camera2D camera;
	RunnerTilemap tilemap;
	Player player;
};
