#pragma once

#include "Interface/InterfaceComponent.h"

class TopBar : public RenderEngine::InterfaceComponent
{
public:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:

};

