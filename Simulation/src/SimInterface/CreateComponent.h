#pragma once

#include "Interface/InterfaceComponent.h"
#include "imgui/imgui.h"
#include "Simulation.h"


class SimulationApplication;

class CreateComponent : public RenderEngine::InterfaceComponent
{
public:
	CreateComponent(SimulationApplication * app, bool open = false);
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:
	bool _open = false;
	int _width = 1000;
	int _height = 500;
	SimulationApplication* _app = nullptr;
	bool _startup = true;

};
