#pragma once

#include "Core/WindowBase.h"

class SimulationWindow : public RenderEngine::WindowBase
{
public:
	SimulationWindow();

	void OnDraw() override;
	void OnCreate() override;
private:



};

