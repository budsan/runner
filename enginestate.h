#pragma once

#include "gameframework/gamestate.h"
#include "gameframework/graphics/spriteanim.h"
#include "gameframework/graphics/camera2d.h"

#include "gameframework/tools/particleemitter.h"

#include "player.h"

class EngineState : public GameState
{
public:
	 EngineState();
	~EngineState();

	void Update(float GameTime);
	void Draw();
	
	void Load();
	void Unload();

private:

	Camera2D camera;
	Player player;
	ParticleEmitter *emitter;
};

