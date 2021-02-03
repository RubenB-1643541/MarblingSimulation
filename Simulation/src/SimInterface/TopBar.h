#pragma once

#include "Simulation.h"
#include "Interface/InterfaceComponent.h"
#include "../FileIO/FileDialog.h"
#include "../FileIO/Exporter.h"

class TopBar : public RenderEngine::InterfaceComponent
{
public:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:
	FluidLib::Settings* _settings;
};

