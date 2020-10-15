#pragma once

#include "Interface/InterfaceComponent.h"
#include "imgui/imgui.h"

class SideComponent : public RenderEngine::InterfaceComponent
{
public:
	SideComponent(char* name, ImVec2 pos, ImVec2 size);
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
private:
	ImVec2 _pos = { 0,0 };
	ImVec2 _size = { 0,0 };
	char* _name;


};

