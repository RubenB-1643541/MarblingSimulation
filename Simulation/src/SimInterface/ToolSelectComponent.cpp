#include "ToolSelectComponent.h"
#include "../SimUtils/Icon.h"
#include "Simulation.h"

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
	_active = _tools->GetActive();
}

void ToolSelectComponent::OnDraw()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Tool Selection")) {
		DrawSelection();
		if (_active != nullptr && (_active->GetName() == "Basic" || _active->GetName() == "Dripping")) {
			DrawBasic();
		}
		if (_active != nullptr && _active->GetName() == "Select") {
			DrawSelect();
		}
		ImGui::TreePop();
		ImGui::Separator();
	}
}

void ToolSelectComponent::EndDraw()
{
}

void ToolSelectComponent::AddButton(Button button, bool selected)
{
	Icon icon = Load(button.icon);
	button.id = icon.id;
	if (button.part == TOOL_PART::SURFACE) {
		_surfaces.push_back(button);
		if (selected)
			_selectedsurface = _surfaces.size() - 1;
	}
	else if (button.part == TOOL_PART::MOVEMENT) {
		_movements.push_back(button);
		if (selected)
			_selectedmovement = _movements.size() - 1;
	}
	else if (button.part == TOOL_PART::ACTION) {
		_actions.push_back(button);
		if (selected) {
			_selectedactions.push_back("0");
		}
		else {
			_selectedactions.push_back("");
		}
	}
	else if (button.part == TOOL_PART::TOOL) {
		_toolsbuttons.push_back(button);
		if (selected)
			_selectedtool = _toolsbuttons.size() - 1;
	}
	else if (button.part == TOOL_PART::SELECT_ACTION) {
		_selectactions.push_back(button);
	}
}

void ToolSelectComponent::DrawSelection()
{
	int i = 0;
	for (const Button& button : _toolsbuttons) {
		if (i != 0)
			ImGui::SameLine();
		if (i == _selectedtool)
			ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(0.5,0.5,0.5,1));
		else {
			if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1))) {
				_tools->SetActive(button.name);
				_selectedtool = i;
			}
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(button.name.c_str());
		++i;
	}
}

void ToolSelectComponent::DrawBasic()
{
	//Surfaces
	ImGui::BeginChild("ToolSelectChild", ImVec2(250, 250));
	ImGui::Columns(3);
	ImGui::Text("Surfaces");
	int i = 0;
	for (const Button& button : _surfaces) {
		if (i == _selectedsurface)
			ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(0.5, 0.5, 0.5, 1));
		else {
			if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1))) {
				FluidLib::BasicTool* basic = static_cast<FluidLib::BasicTool*>(_active);
				basic->SetActiveSurface(button.name);
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
			ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(0.5, 0.5, 0.5, 1));
		else {

			if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1))) {
				FluidLib::BasicTool* basic = static_cast<FluidLib::BasicTool*>(_active);
				basic->SetActiveMovement(button.name);
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
	for(int i = 0; i < _actions.size(); ++i) {
		const Button& button = _actions[i];
		if (_selectedactions[i] != "") {
			ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(0.5, 0.5, 0.5, 1));
			for (int j = 0; j <= N_MOUSEBUTTONS; ++j) {
				if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[j]) {
					INFO("Button {0} Click {1}", j, button.name);
					FluidLib::BasicTool* basic = static_cast<FluidLib::BasicTool*>(_active);
					for (int k = 0; k < _selectedactions.size(); ++k) {
						if (std::to_string(j) == _selectedactions[k]) {
							_selectedactions[k] = _selectedactions[i];
							basic->SetActiveAction(_actions[k].name, std::stoi(_selectedactions[i]));
						}
					}
					_selectedactions[i] = std::to_string(j);
					basic->SetActiveAction(button.name, j);
					
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(button.name.c_str());
			ImGui::SameLine();
			if(_selectedactions[i] == "0")
				ImGui::Text("L");
			else if (_selectedactions[i] == "1")
				ImGui::Text("R");
			else if (_selectedactions[i] == "2")
				ImGui::Text("M");
			else
				ImGui::Text(_selectedactions[i].c_str());
		}
		else {
			ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
			for (int j = 0; j <= N_MOUSEBUTTONS; ++j) {
				if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[j]) {
					INFO("Button {0} Click {1}", j, button.name);
					for (int k = 0; k < _selectedactions.size(); ++k)
						if (std::to_string(j) == _selectedactions[k])
							_selectedactions[k] = "";
					_selectedactions[i] = std::to_string(j);
					FluidLib::BasicTool* basic = static_cast<FluidLib::BasicTool*>(_active);
					basic->SetActiveAction(button.name, j);
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(button.name.c_str());
		}
	}
	ImGui::EndChild();
}

void ToolSelectComponent::DrawSelect()
{
	ImGui::Text("Actions");
	for (const Button& button : _selectactions) {
		if (ImGui::ImageButton((void*)button.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 3, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1))) {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(_active);
			select->ExecuteAction(button.name);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(button.name.c_str());
	}

}


