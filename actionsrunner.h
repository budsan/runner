#pragma once

#include "action.h"
#include "guyframework/input/keyboardlistener.h"

class ActionsRunner : public Actions, public Guy::KeyboardListener
{
public:
	ActionsRunner();

	enum Type { Jump, Dash, ActionCount };

	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
};


