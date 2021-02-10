#pragma once

#include "Interface/InterfaceComponent.h"
#include "Tools/ToolBase.h"
#include "Tools/Basic Tools/SelectTool.h"
#include "Tools/Basic Tools/DrippingTool.h"
#include "Tools/ToolManager.h"
#include "Tools/Surface/Surfaces.h"
#include "Tools/Action/Actions.h"
#include "Tools/Movement/Movements.h"
#include "Simulation.h"
#include "../SimulationCore/GridStructures.h"

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
	void InkActionParams(FluidLib::InkAction<IInk>* inkaction);
	void SelectToolActions();

};

