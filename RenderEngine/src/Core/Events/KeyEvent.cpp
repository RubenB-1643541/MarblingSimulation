#include "pch.h"
#include "KeyEvent.h"

namespace RenderEngine {

	KeyPressEvent::KeyPressEvent(int key) : _key(key)
	{
		_type = EventType::KeyPressEvent;
		_cat = EventCategory::KeyEvent;
	}

	KeyReleaseEvent::KeyReleaseEvent(int key) : _key(key)
	{
		_type = EventType::KeyReleaseEvent;
		_cat = EventCategory::KeyEvent;
	}

	KeyTypeEvent::KeyTypeEvent(int key) : _key(key)
	{
		_type = EventType::KeyTypeEvent;
		_cat = EventCategory::KeyEvent;
	}

}