#pragma once

#include "Simulation.h"
#include "Events/Events.h"

namespace FluidLib {

	class SimulationController
	{
	public:
		static void MouseClick(int button) {
			Simulation::Get()->OnEvent(ToolUseEvent(button));
		}

		static void MouseMove(float x, float y) {
			Simulation::Get()->OnEvent(ToolMoveEvent(x, y));
		}

		static void MouseScroll(float x, float y) {
			Simulation::Get()->OnEvent(ToolScrollEvent(x, y));
		}

		static void SetTool(int index) {
			Simulation::Get()->OnEvent(ToolSelectEvent(index));
		}
	};

}