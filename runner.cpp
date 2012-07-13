#include "runner.h"

#include "guyframework/environment.h"
#include "guyframework/log.h"

#include <boost/bind.hpp>
#include <sstream>

#define GAME_NAME "Runner"
#define GAME_VERSION "1337"

Runner::Runner()
	: tilemap(24, 16, true), backmap(16, 256, false), player(tilemap)
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

void Runner::init()
{
	Guy::Environment &env = Guy::Environment::instance();
	//env.setFramesPerSecond(60);
	env.getScreenManager().setMode(Guy::Screen::Mode(1024, 600), false);

	Guy::Input &in = env.getInputManager();
	in.addFocusListener(this);
	in.getKeyboard().addListener(this);

	ActionsRunner *actionsRunner = new ActionsRunner();
	in.getKeyboard().addListener(actionsRunner);

	std::vector<Actions*> &actions = Actions::instance();
	actions.push_back(actionsRunner);

	//Keybinds k(NUMPLAYERS, Action_Size);
	//k[0][Action_Jump].setDefault(SDLK_UP);
	//k[0][Action_Dash].setDefault(SDLK_RIGHT);
}

void Runner::load()
{
	if (!frames.loadFont("data/font/nibby.ttf"))
	{
		Guy::printLog("ERROR: Loading frames font\n");
	}

	frames.setColor(Guy::rgba(1,1,1,1));

	camera.init();
	player.load();
	backmap.setColor(Guy::rgba(0.5f));

	middleText.loadFont("data/font/nibby.ttf", 32);
	middleText.font()->setAlignment(Guy::Font::CENTER);
	middleText.setColor(Guy::rgba(1,1,1,1));
	middleText.clampedPos() = math::vec2f(0.5f, 0.5f);

	scoreText.font() =  middleText.font();
	scoreText.setColor(Guy::rgba(1,1,1,1));
	scoreText.clampedPos() = math::vec2f(0.5f, 0.925f);

	player.hasJumped = boost::bind(&Runner::playerHasJumped, this);
	player.hasAirJumped = boost::bind(&Runner::playerHasAirJumped, this);
	player.hasDashed = boost::bind(&Runner::playerHasDashed, this);

	music.load_mem("data/sound/gamemusic.ogg");
	music.set_source();
	music.set_loop(true);
	music.play();

	linear.setTime(0.25f);
	startTutorial();
}

void Runner::unload()
{

}

void Runner::update(float deltaTime)
{
	frames.update(deltaTime);

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

			if(resetTransition)
			{
				if (linear.reached())
				{
					reset();
				}
			}
			else
			{
				if (anyKeyDown)
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

	//restart input
	anyKeyDown = false;
	Actions::endOfFrameAll();
}

void Runner::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().getScreenManager();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

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

	if (linear.getPos() > 0) screen.fillWithColor(Guy::rgba(0, 0, 0, linear.getPos()));

	frames.draw();
}

void Runner::onGainInputFocus()
{
	Guy::Environment::instance().resume();
}

void Runner::onLoseInputFocus()
{
	Guy::Environment::instance().pause();
}

void Runner::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{

}

void Runner::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	anyKeyDown = true;
}

void Runner::startTutorial()
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

void Runner::reset()
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

void Runner::playerHasJumped()
{
	if (this->tutorial_playing) this->tutorial_jumped = true;
	else score += -5;
}

void Runner::playerHasAirJumped()
{
	if (this->tutorial_jumped) this->tutorial_airjumped = true;
	else score += -10;
}

void Runner::playerHasDashed()
{
	if (this->tutorial_airjumped) this->tutorial_dashed = true;
	else score += -15;
}


