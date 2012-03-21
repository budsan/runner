#include "enginestate.h"

#include <time.h>
#include <sstream>

#include "boost/bind.hpp"

#include "keys.h"
#include "input/input.h"

#include "audio/emyl.h"

#include "graphics/graphics.h"
#include "graphics/screen.h"

EngineState:: EngineState() : tilemap(24, 16, true), backmap(16, 256, false), player(tilemap) {}
EngineState::~EngineState() {}

void EngineState::load()
{
	camera.init();
	player.load();
	backmap.setColor(rgba(0.5f));

	middleText.loadFont("data/font/nibby.ttf", 32);
	middleText.font()->setAlignment(Font::CENTER);
	middleText.setColor(rgba(1,1,1,1));
	middleText.clampedPos() = math::vec2f(0.5f, 0.5f);

	scoreText.font() =  middleText.font();
	scoreText.setColor(rgba(1,1,1,1));
	scoreText.clampedPos() = math::vec2f(0.5f, 0.925f);

	player.hasJumped = boost::bind(&EngineState::playerHasJumped, this);
	player.hasAirJumped = boost::bind(&EngineState::playerHasAirJumped, this);
	player.hasDashed = boost::bind(&EngineState::playerHasDashed, this);

	music.load("data/sound/A_Airbrushed.ogg");
	music.set_source();
	music.set_loop(true);
	music.play();

	linear.setTime(0.25f);
	startTutorial();
}

void EngineState::unload()
{

}

void EngineState::update(float deltaTime)
{
	if (tutorial_playing)
	{
		if (!tutorial_jumped) {
			middleText.displayText() = "Press up for jump";
		}
		else if(!tutorial_airjumped) {
			middleText.displayText() = "Press up in middle air for air jump";
		}
		else if(!tutorial_dashed) {
			middleText.displayText() = "Press right in middle air for flying";
		}
		else if (countdown < 3.0) {
			middleText.displayText() = "Now you are ready!";
			countdown += deltaTime;
		}
		else {

			if(resetTransition)
			{
				if (linear.reached())
				{
					reset();
				}
			}
			else
			{
				resetTransition = true;
				linear.setPos(0);
				linear.goPos(1);
			}
		}
	}
	else
	{
		if (countdown >= -1)
		{
			static char * readyStrings[] = { "GO FOR IT!!! :D", "One!!!", "Two!!", "Three!" };
			int select = (int) floor(countdown);
			countdown -= deltaTime * 2;

			select = select >= 0 ? select : 0;
			middleText.displayText() = readyStrings[select];
			scoreText.displayText() = "";
		}
		else
		{
			int currShowScore = 0;
			if (!player.failed())
			{
				score += deltaTime * 100;
				currShowScore = ((int)floor(score/25))*25;
			}
			else
			{
				currShowScore = (int)score;
			}

			if (currShowScore > showScore) showScore = currShowScore;
			std::stringstream ss; ss << "Score: " << showScore;
			scoreText.displayText() = ss.str();
			middleText.displayText() = "";
		}
	}

	if (player.loaded())
	{
		if (player.failed())
		{
			middleText.displayText() = "Press any key to continue...";
			const InputState &state = Input::Instance().getInputState();

			if(resetTransition)
			{
				if (linear.reached())
				{
					reset();
				}
			}
			else
			{
				if (state.isAnyKeyDown())
				{
					resetTransition = true;
					linear.setPos(0);
					linear.goPos(1);
				}
			}
		}

		player.update(deltaTime);
	}

	linear.update(deltaTime);
}

void EngineState::draw()
{
	glDisable(GL_DEPTH_TEST);

	Screen &screen = Screen::instance();
	screen.fillWithColor(rgba(0.75f, 0.75f, 0.75f, 1));

	//BACKTILEMAP
	camera.setZoom(0.25f);
	camera.setPos(math::vec2f(player.pos().x+16384, 2048)*0.25f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getModelviewMatrix().v);

	{
		math::bbox2f frustrum = camera.getBounding();
		backmap.draw(frustrum);
	}


	//FRONT TILEMAP
	camera.setZoom(1);
	camera.setPos(math::vec2f(player.pos().x+224, 224));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getModelviewMatrix().v);

	{
		math::bbox2f frustrum = camera.getBounding();
		tilemap.draw(frustrum);
	}

	if (player.loaded()) player.draw();
	if (!tutorial_playing) scoreText.draw();
	middleText.draw();

	if (linear.getPos() > 0) screen.fillWithColor(rgba(0, 0, 0, linear.getPos()));
}

void EngineState::startTutorial()
{
	player.reset();
	tilemap.init(0);
	backmap.init(time(0));

	tutorial_playing = true;
	tutorial_jumped = false;
	tutorial_airjumped = false;
	tutorial_dashed = false;
	countdown = 0;

	resetTransition = false;
	linear.setPos(1);
	linear.goPos(0);
}

void EngineState::reset()
{
	player.reset();
	tilemap.init((int) time(0));
	backmap.init((int) time(0) * 2);

	showScore = 0;
	score = 0;
	tutorial_playing = false;
	tutorial_jumped = false;
	tutorial_airjumped = false;
	tutorial_dashed = false;
	countdown = 3.999;

	resetTransition = false;
	linear.setPos(1);
	linear.goPos(0);
}

void EngineState::playerHasJumped()
{
	if (this->tutorial_playing) this->tutorial_jumped = true;
	else score += -5;
}

void EngineState::playerHasAirJumped()
{
	if (this->tutorial_jumped) this->tutorial_airjumped = true;
	else score += -10;
}

void EngineState::playerHasDashed()
{
	if (this->tutorial_airjumped) this->tutorial_dashed = true;
	else score += -15;
}

