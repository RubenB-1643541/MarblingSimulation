#include "pch.h"
#include "MouseEvent.h"

namespace RenderEngine {

	MouseMoveEvent::MouseMoveEvent(double xpos, double ypos) : _xpos(xpos), _ypos(ypos)
	{
		_type = EventType::MouseMoveEvent;
		_cat = EventCategory::MouseEvent;
	}

	MousePressEvent::MousePressEvent(int button) : _button(button), _xpos(-1), _ypos(-1)
	{
		_type = EventType::MousePressEvent;
		_cat = EventCategory::MouseEvent;
	}

	MousePressEvent::MousePressEvent(int button, double xpos, double ypos) : _button(button), _xpos(xpos), _ypos(ypos)
	{
		_type = EventType::MousePressEvent;
		_cat = EventCategory::MouseEvent;
	}

	MouseReleaseEvent::MouseReleaseEvent(int button) : _button(button), _xpos(-1), _ypos(-1)
	{
		_type = EventType::MouseReleaseEvent;
		_cat = EventCategory::MouseEvent;
	}

	MouseReleaseEvent::MouseReleaseEvent(int button, double xpos, double ypos) : _button(button), _xpos(xpos), _ypos(ypos)
	{
		_type = EventType::MouseReleaseEvent;
		_cat = EventCategory::MouseEvent;
	}

	MouseScrollEvent::MouseScrollEvent(double xoffset, double yoffset) : _xoffset(xoffset), _yoffset(yoffset)
	{
		_type = EventType::MouseScrollEvent;
		_cat = EventCategory::MouseEvent;
	}

}