#include "ToolEvent.h"

namespace FluidLib {

	ToolSelectEvent::ToolSelectEvent(int tool, bool first) : _toolIndex(tool), _first(first)
	{

	}

	ToolUseEvent::ToolUseEvent(int action, int actionNumber) : _action(action), _actionNumber(actionNumber)
	{

	}

	ToolMoveEvent::ToolMoveEvent(float x, float y) : _x(x), _y(y)
	{

	}

	ToolScrollEvent::ToolScrollEvent(float x, float y) : _x(x), _y(y)
	{
	}

}