#include "enginestate.h"

#include <fstream>

#include "gameframework/graphics/graphics.h"
#include "gameframework/graphics/screen.h"

EngineState:: EngineState() : tilemap(16), player(tilemap) {}
EngineState::~EngineState() {}

void EngineState::update(float deltaTime)
{
	if (player.loaded()) player.update(deltaTime);
}

void EngineState::load()
{
	camera.init();
	camera.setZoom(1.5);
	player.load();
}

void EngineState::unload()
{

}

void EngineState::draw()
{
	glDisable(GL_DEPTH_TEST);

	Screen &screen = Screen::instance();
	screen.fillWithColor(rgba(0.5, 0.5 ,0.5, 1));

	camera.setPos(math::vec2f(player.pos().x+64, 128));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getModelviewMatrix().v);

	auto frustrum = camera.getBounding();
	tilemap.draw(frustrum);

	if (player.loaded()) player.draw();
}

