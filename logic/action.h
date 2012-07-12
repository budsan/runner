#pragma once

struct Action
{
	enum Type { Action_Jump, Action_Dash, Action_Count };

	Action() : isDown(false), isUp(false), isPressed(false) {}

	bool isDown;
	bool isUp;
	bool isPressed;

	void endOfFrame()
	{
		isDown = false;
		isUp = false;
	}
};

class ActionManager
{
public:
	bool isDown(Action::Type type) const
	{
		return m_actions[(unsigned int)type].isDown;
	}

	bool isUp(Action::Type type) const
	{
		return m_actions[(unsigned int)type].isUp;
	}

	bool isPressed(Action::Type type) const
	{
		return m_actions[(unsigned int)type].isPressed;
	}

	void endOfFrame()
	{
		for (unsigned int i = 0; i < Action::Action_Count; ++i)
		{
			m_actions[i].endOfFrame();
		}
	}

protected:

	Action	m_actions[Action::Action_Count];
};

