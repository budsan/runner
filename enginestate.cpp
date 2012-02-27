#include "enginestate.h"

#include <time.h>

#include "gameframework/graphics/graphics.h"
#include "gameframework/graphics/screen.h"

EngineState:: EngineState() : tilemap(16, 14), backmap(16, 128), player(tilemap) {}
EngineState::~EngineState() {}

void EngineState::update(float deltaTime)
{
	if (player.loaded())
	{
		if (player.failed()) reset();
		player.update(deltaTime);
	}
}

void EngineState::load()
{
	camera.resizeScreen(400);
	player.load();
	backmap.setColor(rgba(0.5f));
	reset();
}

void EngineState::unload()
{

}

void EngineState::draw()
{
	glDisable(GL_DEPTH_TEST);

	Screen &screen = Screen::instance();
	screen.fillWithColor(rgba(0.75f, 0.75f, 0.75f, 1));

	//BACKTILEMAP
	camera.setZoom(0.25f);
	camera.setPos(math::vec2f(player.pos().x-2048, 512)/4);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getModelviewMatrix().v);

	{auto frustrum = camera.getBounding();
	backmap.draw(frustrum);}


	//FRONT TILEMAP
	camera.setZoom(1);
	camera.setPos(math::vec2f(player.pos().x+64, 128));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getModelviewMatrix().v);

	{auto frustrum = camera.getBounding();
	tilemap.draw(frustrum);}

	if (player.loaded()) player.draw();
}

void EngineState::reset()
{
	player.reset();
	tilemap.init(time(0));
	backmap.init(time(0)*2);
}

