#include "actionsrunner.h"


ActionsRunner::ActionsRunner() : Actions(ActionCount)
{

}

void ActionsRunner::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_Up:
		m_actions[Jump].actionUp();
		break;
	case Guy::Keyboard::Key_Right:
		m_actions[Dash].actionUp();
		break;
	default:
		break;
	}
}

void ActionsRunner::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_Up:
		m_actions[Jump].actionDown();
		break;
	case Guy::Keyboard::Key_Right:
		m_actions[Dash].actionDown();
		break;
	default:
		break;
	}
}
