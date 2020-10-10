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

		static void MouseRelease(int button) {
			Simulation::Get()->OnEvent(ToolEndUseEvent(button));
		}

		static void MouseMove(float x, float y) {
			float newx = x - (Simulation::Get()->GetScreenWidth() / 2 - Simulation::Get()->GetSizeX() / 2);
			float newy = y - (Simulation::Get()->GetScreenHeight() / 2 - Simulation::Get()->GetSizeY() / 2);
			Simulation::Get()->OnEvent(ToolMoveEvent(newx, newy));
		}

		static void MouseScroll(float x, float y) {
			Simulation::Get()->OnEvent(ToolScrollEvent(x, y));
		}

		static void SetTool(int index) {
			Simulation::Get()->OnEvent(ToolSelectEvent(index));
		}
	};

}