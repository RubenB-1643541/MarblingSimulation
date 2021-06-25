#include "PresetComponent.h"
#include "Simulation.h"
#include <Tools/Surface/Circle.h>

PresetComponent::PresetComponent(FluidLib::ToolManager* tools, const std::string& basic, ToolSelectComponent* tool)
{
	_tools = tools;
	_basicname = basic;
	_toolsselect = tool;

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
		static bool first = true;
		if (first) {
			ImGui::SetNextTreeNodeOpen(true);
			first = false;
		}
		if (ImGui::TreeNode("Presets")) {
			if (ImGui::Button("Add Preset")) {
				FluidLib::Simulation::Get()->GetTools()->SetEnabled(false);
				_create = true;
				if (_toolsselect != nullptr) {
					//Returns std::string reference, so it should be fine
					_selectedsurface = _toolsselect->GetSelectedSurface().c_str();
					_selectedmovement = _toolsselect->GetSelectedMovement().c_str();
					_selectedaction = _toolsselect->GetSelectedAction().c_str();
				}
			}
			ImGui::BeginChild("PresetList", ImVec2(0, 300), true);
			for (auto preset : _presets) {
				if (ImGui::Selectable(preset.first.c_str(), false)) {
					_basic->SetActiveSurface(preset.second.surface);
					_basic->SetActiveMovement(preset.second.movement);
					_basic->SetActiveAction(preset.second.action);
					if (_toolsselect != nullptr) {
						_toolsselect->UpdateSelection(preset.second.surface, preset.second.movement, preset.second.action);
					}
					_basic->GetSurface()->LoadParam(preset.second.surfparams);
					_basic->GetMovement()->LoadParam(preset.second.moveparams);
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
	Load();
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
	std::remove_if(preset.name.begin(), preset.name.end(), isspace);
	preset.surface = _selectedsurface;
	preset.movement = _selectedmovement;
	preset.action = _selectedaction;
	AddPreset(preset);
}

void PresetComponent::Load()
{
	_presets.clear();
	std::ifstream stream;
	stream.open(presetsfile, std::ios::in);
	while (!stream.eof()) {
		Preset preset;
		stream >> preset.name >> preset.surface;
		if (preset.surface == "Circle") {
			FluidLib::CircleSurfParameters* p = new FluidLib::CircleSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		else if (preset.surface == "FanSurface") {
			FluidLib::FanSurfParameters * p = new FluidLib::FanSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		else if (preset.surface == "PointSurface") {
			//stream << static_cast<FluidLib::PointSurface*>(_basic->GetSurface("PointSurface"))->GetParam() << " ";
			preset.surfparams = nullptr;
		}
		else if (preset.surface == "Polygon") {
			FluidLib::PolygonSurfParameters* p = new FluidLib::PolygonSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		else if (preset.surface == "Rectangle") {
			FluidLib::RectangleSurfParameters * p = new FluidLib::RectangleSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		else if (preset.surface == "Square") {
			FluidLib::SquareSurfParameters * p = new FluidLib::SquareSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		else if (preset.surface == "Triangle") {
			FluidLib::TriangleSurfParameters * p = new FluidLib::TriangleSurfParameters();
			stream >> *p;
			preset.surfparams = p;
		}
		stream >> preset.movement;
		if (preset.movement == "BezierCurve") {
			FluidLib::BezierCurveMoveParameters * p = new FluidLib::BezierCurveMoveParameters();
			stream >> *p;
			preset.moveparams = p;
		}
		else if (preset.movement == "Circle") {
			FluidLib::CircleMoveParameters * p = new FluidLib::CircleMoveParameters();
			stream >> *p;
			preset.moveparams = p;
		}
		else if (preset.movement == "Line") {
			FluidLib::LineMoveParameters * p = new FluidLib::LineMoveParameters();
			stream >> *p;
			preset.moveparams = p;
		}
		else if (preset.movement == "Mouse") {
			//stream << static_cast<FluidLib::MouseMovement*>(_basic->GetMovement("Mouse"))->GetParam() << " ";
			preset.moveparams = nullptr;
		}
		else if (preset.movement == "Point") {
			FluidLib::PointMoveParameters * p = new FluidLib::PointMoveParameters();
			stream >> *p;
			preset.moveparams = p;
		}
		else if (preset.movement == "Sine") {
			FluidLib::SineMoveParameters * p = new FluidLib::SineMoveParameters();
			stream >> *p;
			preset.moveparams = p;
		}
		stream >> preset.action;
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
	
	stream << preset.name << " ";
	stream << preset.surface << " ";
	if (preset.surface == "Circle") {
		stream << static_cast<FluidLib::Circle*>(_basic->GetSurface("Circle"))->GetParam() << " ";
	}
	else if (preset.surface == "FanSurface") {
		stream << static_cast<FluidLib::FanSurface*>(_basic->GetSurface("FanSurface"))->GetParam() << " ";
	}
	else if (preset.surface == "PointSurface") {
		//stream << static_cast<FluidLib::PointSurface*>(_basic->GetSurface("PointSurface"))->GetParam() << " ";
	}
	else if (preset.surface == "Polygon") {
		stream << static_cast<FluidLib::Polygon*>(_basic->GetSurface("Polygon"))->GetParam() << " ";
	}
	else if (preset.surface == "Rectangle") {
		stream << static_cast<FluidLib::Rectangle*>(_basic->GetSurface("Rectangle"))->GetParam() << " ";
	}
	else if (preset.surface == "Square") {
		stream << static_cast<FluidLib::Square*>(_basic->GetSurface("Square"))->GetParam() << " ";
	}
	else if (preset.surface == "Triangle") {
		stream << static_cast<FluidLib::Triangle*>(_basic->GetSurface("Triangle"))->GetParam() << " ";
	}
	stream << preset.movement << " ";
	if (preset.movement == "BezierCurve") {
		stream << static_cast<FluidLib::BezierCurve*>(_basic->GetMovement("BezierCurve"))->GetParam() << " ";
	}
	else if (preset.movement == "Circle") {
		stream << static_cast<FluidLib::CircleMovement*>(_basic->GetMovement("Circle"))->GetParam() << " ";
	}
	else if (preset.movement == "Line") {
		stream << static_cast<FluidLib::Line*>(_basic->GetMovement("Line"))->GetParam() << " ";
	}
	else if (preset.movement == "Mouse") {
		//stream << static_cast<FluidLib::MouseMovement*>(_basic->GetMovement("Mouse"))->GetParam() << " ";
	}
	else if (preset.movement == "Point") {
		stream << static_cast<FluidLib::PointMovement*>(_basic->GetMovement("Point"))->GetParam() << " ";
	}
	else if (preset.movement == "Sine") {
		stream << static_cast<FluidLib::Sine*>(_basic->GetMovement("Sine"))->GetParam() << " ";
	}
	stream << preset.action << std::endl;
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
		if (name == preset.name) {}//&& surface == preset.surface && movement == preset.movement && action == preset.action) {}
		else {
			out << line << std::endl;
		}
	}
	in.close();
	out.close();
	remove(presetsfile.c_str());
	rename("preset.temp", presetsfile.c_str());
}
