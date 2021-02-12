#pragma once

#include "Interface/InterfaceComponent.h"
#include "../SimUtils/SaveStateHandler.h"

class SaveStateComponent : public RenderEngine::InterfaceComponent
{
public:
	SaveStateComponent();
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;

private:
	void DrawStates();

	
	bool _auto = false;
	
};

