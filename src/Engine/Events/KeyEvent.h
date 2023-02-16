#pragma once
#include "Engine/Events/Event.h"

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput) 
protected:
	KeyEvent(int keycode)
		: m_KeyCode(keycode) {}

	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode)
		:KeyEvent(keycode) {}

	EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode)
		:KeyEvent(keycode) {}

	EVENT_CLASS_TYPE(KeyReleased)
};