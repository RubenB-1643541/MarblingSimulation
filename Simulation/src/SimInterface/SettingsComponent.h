#pragma once

#include "Interface/InterfaceComponent.h"
#include "Simulation.h"
#include "../SimUtils/Icon.h"

class SettingsComponent : public RenderEngine::InterfaceComponent
{
public:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:
	FluidLib::Settings* _settings;
	int _lastfps = 0;
	ImVec2 _iconSize = ImVec2(30, 30);
	Icon _playicon;
	Icon _pauseicon;
};

