#include "PresetComponent.h"
#include "Simulation.h"

PresetComponent::PresetComponent(FluidLib::ToolManager* tools, const std::string& basic)
{
	_tools = tools;
	_basicname = basic;

	_window_flags |= ImGuiWindowFlags_NoTitleBar;
	_window_flags |= ImGuiWindowFlags_NoCollapse;
	_window_flags |= ImGuiWindowFlags_NoMove;
	_window_flags |= ImGuiWindowFlags_NoResize;
	_window_flags |= ImGuiWindowFlags_Popup;
}

void PresetComponent::OnInit()
{
	Load();
}

void PresetComponent::OnUpdate()
{
	if (_basic == nullptr)
		_basic = static_cast<FluidLib::BasicTool*>(_tools->Get(_basicname));
	_active = _tools->GetActive();
}

void PresetComponent::OnDraw()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (_active != nullptr && _active->GetName() == "Basic") {
		if (ImGui::TreeNode("Presets")) {
			if (ImGui::Button("Add Preset")) {
				FluidLib::Simulation::Get()->GetTools()->SetEnabled(false);
				_create = true;
			}
			ImGui::BeginChild("PresetList", ImVec2(0, 300), true);
			for (auto preset : _presets) {
				if (ImGui::Selectable(preset.first.c_str(), false)) {
					_basic->SetActiveSurface(preset.second.surface);
					_basic->SetActiveMovement(preset.second.movement);
					_basic->SetActiveAction(preset.second.action);
				}
				if (ImGui::IsItemHovered()) {
					std::string tip = preset.second.surface;
					tip.append("-");
					tip.append(preset.second.movement);
					tip.append("-");
					tip.append(preset.second.action);
					ImGui::SetTooltip(tip.c_str());
				}
				static bool remove = false;
				std::string context = preset.first + "ContextMenu";
				if (ImGui::BeginPopupContextItem(context.c_str()))
				{
					if (ImGui::Button("Remove")) {
						FluidLib::Simulation::Get()->GetTools()->SetEnabled(false);
						ImGui::OpenPopup("RemovePreset");
						remove = true;
					}
					if (ImGui::BeginPopupModal("RemovePreset", &remove, _window_flags)) {
						ImGui::Text("Remove Preset?");
						ImGui::Text(preset.second.name.c_str());
						ImGui::Text(preset.second.surface.c_str());
						ImGui::Text(preset.second.movement.c_str());
						ImGui::Text(preset.second.action.c_str());
						if (ImGui::Button("No")) {
							remove = false;
							FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
						}
						ImGui::SameLine(0, 200.0f);
						if (ImGui::Button("Yes")) {
							remove = false;
							RemovePreset(preset.second);
							FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
							ImGui::EndPopup();
							ImGui::EndPopup();
							break;
						}
						ImGui::EndPopup();
					}
					ImGui::EndPopup();
				}
			}


			ImGui::EndChild();

			ImGui::Separator();
			ImGui::TreePop();
		}
		if (_create) {
			ImGui::OpenPopup("CreatePreset");
		}
		if (ImGui::BeginPopupModal("CreatePreset", &_create, _window_flags)) {
			ImGui::InputText("Name", _namebuffer, namebuffersize);
			if (ImGui::BeginCombo("Surface", _selectedsurface)) {
				for (int i = 0; i < _basic->GetSurfaceKeys().size(); ++i) {
					bool selected = (_selectedsurface == _basic->GetSurfaceKeys()[i].c_str());
					if (ImGui::Selectable(_basic->GetSurfaceKeys()[i].c_str(), selected)) {
						_selectedsurface = _basic->GetSurfaceKeys()[i].c_str();
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Movement", _selectedmovement)) {
				for (int i = 0; i < _basic->GetMovementKeys().size(); ++i) {
					bool selected = (_selectedmovement == _basic->GetMovementKeys()[i].c_str());
					if (ImGui::Selectable(_basic->GetMovementKeys()[i].c_str(), selected)) {
						_selectedmovement = _basic->GetMovementKeys()[i].c_str();
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Action", _selectedaction)) {
				for (int i = 0; i < _basic->GetActionKeys().size(); ++i) {
					bool selected = (_selectedaction == _basic->GetActionKeys()[i].c_str());
					if (ImGui::Selectable(_basic->GetActionKeys()[i].c_str(), selected)) {
						_selectedaction = _basic->GetActionKeys()[i].c_str();
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Cancel")) {
				_create = false;
				FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
			}
			ImGui::SameLine(0, 200.0f);
			if (ImGui::Button("Add")) {
				CreatePreset();
				_create = false;
				FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
				_namebuffer[0] = '\0';
				_selectedsurface = NULL;
				_selectedmovement = NULL;
				_selectedaction = NULL;
			}
			ImGui::EndPopup();
		}
	}
	
}

void PresetComponent::EndDraw()
{
}

void PresetComponent::AddPreset(Preset& preset)
{
	auto result = _presets.insert(std::make_pair(preset.name, preset));
	if (result.second) {
		WriteToFile(preset);
	}
}

void PresetComponent::RemovePreset(Preset& preset)
{
	int res = _presets.erase(preset.name);
	if (res > 0) {
		RemoveFromFile(preset);
	}
}

void PresetComponent::CreatePreset()
{
	Preset preset;
	preset.name = _namebuffer;
	preset.surface = _selectedsurface;
	preset.movement = _selectedmovement;
	preset.action = _selectedaction;
	AddPreset(preset);
}

void PresetComponent::Load()
{
	std::ifstream stream;
	stream.open(presetsfile, std::ios::in);
	while (!stream.eof()) {
		Preset preset;
		stream >> preset.name >> preset.surface >> preset.movement >> preset.action;
		if (preset.name != "") {
			_presets.insert(std::make_pair(preset.name, preset));
		}
	}
	stream.close();
}

void PresetComponent::WriteToFile(Preset& preset)
{ 
	std::ofstream stream;
	stream.open(presetsfile, std::ios::out | std::ios::app);
	stream << preset.name << " " << preset.surface << " " << preset.movement << " " << preset.action << std::endl;
	stream.close();
}

void PresetComponent::RemoveFromFile(Preset& preset)
{
	std::string line;
	std::string name;
	std::string surface;
	std::string movement;
	std::string action;
	std::ifstream in;
	in.open(presetsfile);
	std::ofstream out;
	out.open("preset.temp");
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		ss >> name >> surface >> movement >> action;
		if (name == preset.name && surface == preset.surface && movement == preset.movement && action == preset.action) {}
		else {
			out << line << std::endl;
		}
	}
	in.close();
	out.close();
	remove(presetsfile.c_str());
	rename("preset.temp", presetsfile.c_str());
}
