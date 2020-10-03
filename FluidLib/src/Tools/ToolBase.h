#pragma once

#include "../Events/Events.h"
#include "Action.h"

namespace FluidLib {
	
#define BIND_EVENT(x) std::bind(&ToolBase::x, this, std::placeholders::_1)

	class ToolBase
	{
	public:
		inline virtual void Draw() {};

		virtual bool OnEvent(Event& toolevent);
		virtual inline bool OnSelectEvent(ToolSelectEvent& event) {}
		virtual inline bool OnUseEvent(ToolUseEvent& event) {}
		virtual inline bool OnMoveEvent(ToolMoveEvent& event) {}
		virtual inline bool OnScrollEvent(ToolScrollEvent& event) {}


	protected:
		Action* action;

	private:


	};

}

