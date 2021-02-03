#pragma once

#include "Interface/InterfaceComponent.h"
#include "Tools/ToolManager.h"
#include "Tools/Basic Tools/BasicTool.h"
#include <string>
#include "Simulation.h"

class MultiSurfaceComponent : public RenderEngine::InterfaceComponent
{
public:
	MultiSurfaceComponent(FluidLib::ToolManager* tools, const std::string& basic = "Basic");
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;

private:
	void DrawCrossSurface(FluidLib::CrossSurface* s);
	void DrawCombSurface(FluidLib::Comb* s);
	FluidLib::ToolManager* _tools = nullptr;
	FluidLib::BasicTool* _basic = nullptr;
	FluidLib::ToolBase* _active = nullptr;
	std::string _basicstr = "";

	int _selected = 0;
	std::string _selectedstr = "";
};

