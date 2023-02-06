#pragma once
#include "Event.h"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height) 
		:Width(width), Height(height)
	{
	}

	int Width;
	int Height;

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryWindow)
	
};