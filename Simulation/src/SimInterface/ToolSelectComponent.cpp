#include "ToolSelectComponent.h"
#include "../SimUtils/Icon.h"

ToolSelectComponent::ToolSelectComponent(FluidLib::ToolManager* tools, const std::string& basic)
{
	_tools = tools;
	_basicname = basic;
}

void ToolSelectComponent::OnInit()
{
	//_basic = static_cast<FluidLib::BasicTool*>(_tools->Get(_basicname));
}

void ToolSelectComponent::OnUpdate()
{
	if(_basic == nullptr)
		_basic = static_cast<FluidLib::BasicTool*>(_tools->Get(_basicname));
}

void ToolSelectComponent::OnDraw()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Tool Selection")) {
		//Surfaces
		ImGui::BeginChild("ToolSelectChild", ImVec2(250, 250));
		ImGui::Columns(3);
		ImGui::Text("Surfaces");
		int i = 0;
		for (const Button& button : _surfaces) {
			if (i == _selectedsurface)
				ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(1, 1, 1, 1));
			else {
				if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1))) {
					_basic->SetActiveSurface(button.name);
					_selectedsurface = i;
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(button.name.c_str());
			++i;
		}
	
		//Movements
		ImGui::NextColumn();
		ImGui::Text("Movements");
		i = 0;
		for (const Button& button : _movements) {
			if (i == _selectedmovement)
				ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(1, 1, 1, 1));
			else {
				
				if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1))) {
					_basic->SetActiveMovement(button.name);
					_selectedmovement = i;
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(button.name.c_str());
			++i;
		}

		//Actions
		ImGui::NextColumn();
		ImGui::Text("Actions");
		i = 0;
		for (const Button& button : _actions) {
			if (i == _selectedaction)
				ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(1, 1, 1, 1));
			else {
				if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1))) {
					_basic->SetActiveAction(button.name);
					_selectedaction = i;
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(button.name.c_str());
			++i;
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::TreePop();
	}
}

void ToolSelectComponent::EndDraw()
{
}

void ToolSelectComponent::AddButton(Button button)
{
	Icon icon = Load(button.icon);
	button.id = icon.id;
	if (button.part == TOOL_PART::SURFACE)
		_surfaces.push_back(button);
	else if (button.part == TOOL_PART::MOVEMENT)
		_movements.push_back(button);
	else if (button.part == TOOL_PART::ACTION)
		_actions.push_back(button);
}
