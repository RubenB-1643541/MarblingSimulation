#pragma once

#include "Simulation.h"
#include "Events/Events.h"

namespace FluidLib {

	struct InGrid {
		bool in = false;
	};

	static InGrid ingrid;

	class SimulationController
	{
	public:
		static void MouseClick(int button) {
			if(ingrid.in)
				Simulation::Get()->OnEvent(ToolUseEvent(button));
		}

		static void MouseRelease(int button) {
			if(ingrid.in)
				Simulation::Get()->OnEvent(ToolEndUseEvent(button));
		}

		static void MouseMove(float x, float y) {
			ingrid.in = true;
			float newx = x - (Simulation::Get()->GetScreenWidth() / 2 - Simulation::Get()->GetSizeX() / 2);
			float newy = y - (Simulation::Get()->GetScreenHeight() / 2 - Simulation::Get()->GetSizeY() / 2);
			if (newx > 0 && newx < Simulation::Get()->GetSizeX() && newy > 0 && newy < Simulation::Get()->GetSizeY())
				Simulation::Get()->OnEvent(ToolMoveEvent(newx, newy));
			else
				ingrid.in = false;
		}

		static void MouseScroll(float x, float y) {
			Simulation::Get()->OnEvent(ToolScrollEvent(x, y));
		}

		static void SetTool(int index) {
			Simulation::Get()->OnEvent(ToolSelectEvent(index));
		}

		static void KeyPress(int key) {
			Simulation::Get()->OnKeyPress(key);
		}

		static void KeyRelease(int key) {
			Simulation::Get()->OnKeyRelease(key);
		}
	private:
		
	};

}