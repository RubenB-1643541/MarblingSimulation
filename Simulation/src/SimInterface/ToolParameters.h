#pragma once

#include "Interface/InterfaceComponent.h"
#include "Tools/ToolBase.h"
#include "Tools/ToolManager.h"
#include "Tools/Surface/Square.h"
#include "Tools/Surface/Circle.h"
#include "Tools/Movement/Line.h"
#include "Tools/Movement/Sine.h"
#include "Simulation.h"

class ToolParameters : public RenderEngine::InterfaceComponent
{
public:
	ToolParameters(FluidLib::ToolManager* tools);
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;

private:
	FluidLib::ToolBase* _active = nullptr;
	FluidLib::ToolManager* _tools = nullptr;
	bool _changed = false;

	void SurfaceParams();
	void MovementParams();
	void ActionParams();

};

