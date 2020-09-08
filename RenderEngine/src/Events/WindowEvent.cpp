#include "pch.h"
#include "WindowEvent.h"

namespace RenderEngine {

	WindowResizeEvent::WindowResizeEvent(int width, int height) : _width(width), _height(height)
	{
		_type = EventType::WindowResizeEvent;
		_cat = EventCategory::WindowEvent;
	}

	WindowCloseEvent::WindowCloseEvent()
	{
		_type = EventType::WindowCloseEvent;
		_cat = EventCategory::WindowEvent;
	}

}
