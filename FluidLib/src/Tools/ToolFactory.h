#pragma once

#include "ToolManager.h"

namespace FluidLib {

	enum class SURFACE_TYPE {SURFACE_CIRCLE, SURFACE_SQUARE, SURFACE_RECT, SURFACE_CUSTOM };
	enum class MOVEMENT_TYPE { MOVEMENT_MOUSE, MOVEMENT_SIN, MOVEMENT_COS, MOVEMENT_CUSTOM };

	struct ToolDefinition {
		std::string name;
		SURFACE_TYPE surface;
		MOVEMENT_TYPE movement;
		//Action
	};


	class ToolFactory
	{
	public:
		static void AddTool(ToolManager* _tools, ToolDefinition& def);
		static void AddTools(ToolManager* _tools, std::vector<ToolDefinition> defs);

	private:


	};

}