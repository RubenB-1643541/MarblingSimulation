#pragma once

#include "ToolBase.h"
#include "../Events/Events.h"
#include <vector>

namespace FluidLib {

	class ToolManager
	{
	public:
		ToolManager();
		~ToolManager();

		void AddTool(ToolBase* tool);
		//Load Tool Layout
		//Update
		void Draw();
		//ToolEvent Handler
		void OnEvent(Event& event);
		inline ToolBase* GetActive() { return _activeTool; }
		//Iterators
	private:
		std::vector<ToolBase*> _tools;
		ToolBase* _activeTool = nullptr;

	};

	class ToolLoader 
	{
	public:
		static bool Load(ToolManager& manger, std::string file);
	};

}