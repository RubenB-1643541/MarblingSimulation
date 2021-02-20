#pragma once

#include "Interface/InterfaceComponent.h"
#include "imgui/imgui.h"

#include "Simulation.h"
#include "Tools/Basic Tools/SelectTool.h"

class ToolPopupComponent : public RenderEngine::InterfaceComponent
{
public:
	ToolPopupComponent(FluidLib::ToolManager* tools);
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:
	void DrawSelectPopup();
	bool _open = false;
	FluidLib::ToolManager* _tools = nullptr;
	FluidLib::ToolBase* _active = nullptr;
};

