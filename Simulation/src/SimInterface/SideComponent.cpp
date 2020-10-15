#include "SideComponent.h"
#include "Core/Application.h"

SideComponent::SideComponent(char* name, ImVec2 pos, ImVec2 size) : _name(name), _pos(pos), _size(size)
{
}

void SideComponent::OnInit()
{
	_window_flags |= ImGuiWindowFlags_NoTitleBar;
	_window_flags |= ImGuiWindowFlags_NoCollapse;
	_window_flags |= ImGuiWindowFlags_NoMove;
	_window_flags |= ImGuiWindowFlags_NoResize;
}

void SideComponent::OnUpdate()
{
}

void SideComponent::OnDraw()
{
	ImGui::SetNextWindowPos(_pos);
	ImGui::SetNextWindowSize(_size);
	static bool dummy = true;
	ImGui::Begin(_name, nullptr, _window_flags);
	//ImGui::Begin(_name);
	//ImGui::Text("Test");
}

void SideComponent::EndDraw()
{
	ImGui::End();
}
