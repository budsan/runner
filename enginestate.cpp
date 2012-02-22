#include "enginestate.h"

#include <fstream>

#include "gameframework/graphics/graphics.h"
#include "gameframework/graphics/screen.h"

EngineState:: EngineState() : emitter(nullptr) {}
EngineState::~EngineState() {}

void EngineState::Update(float GameTime)
{
	if (player.Loaded()) {
		player.Update(GameTime);
	}

	if (emitter != nullptr) {
		emitter->Update(GameTime);
	}
}

void EngineState::Load()
{
	camera.Init();
	camera.setZoom(2);
	player.Load();

	if (emitter == nullptr) {
		emitter = new ParticleEmitter();
		if (!emitter->Load("data/scripts/runner_dust.emp")) {
			delete emitter;
			emitter = nullptr;
		}
	}


}

void EngineState::Unload()
{

}

void EngineState::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Screen &screen = Screen::Instance();
	screen.FillWithColor(rgba(0.5, 0.5 ,0.5, 1));

	camera.setPos(math::vec2f(0,0));
	camera.updateOpenGLMatrices();

	if (emitter != nullptr) emitter->Draw();
	if (player.Loaded()) player.Draw();
}

