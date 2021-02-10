#include "CreateComponent.h"
#include "../SimulationCore/SimulationApplication.h"
#include "../FileIO/FileDialog.h"
CreateComponent::CreateComponent(SimulationApplication* app, bool open) : _open(open), _app(app)
{
}

void CreateComponent::OnInit()
{
	_window_flags |= ImGuiWindowFlags_NoTitleBar;
	_window_flags |= ImGuiWindowFlags_NoCollapse;
	_window_flags |= ImGuiWindowFlags_NoMove;
	_window_flags |= ImGuiWindowFlags_NoResize;
	_window_flags |= ImGuiWindowFlags_Popup;
}

void CreateComponent::OnUpdate()
{
	static bool first = true;
	if (first) {
		_width = _app->GetWidth() / 2;
		_height = _app->GetHeight() / 2;
		INFO("Screen Width = {0} - Sim Width = {1}", _app->GetWidth(), _width);
		INFO("Screen Height = {0} - Sim Height = {1}", _app->GetHeight(), _height);
		first = false;
	}
}

void CreateComponent::OnDraw()
{
	if (_open) {
		ImGui::OpenPopup("Create");
		//ImGui::SetNextWindowSize(ImVec2(400, 150));
	}
	if (ImGui::BeginPopupModal("Create", &_open, _window_flags)) {
		ImGui::InputInt("Width", &_width);
		ImGui::InputInt("Height", &_height);
		ImGui::Spacing();
		if (ImGui::Button("Load")) {
			std::string file = FileDialog::Open();
			_app->LoadSimulation(file);
			_open = false;
		}
		ImGui::SameLine(0, 200.0f);
		if (ImGui::Button("Create")) {
			FluidLib::Simulation::Get()->SetSize(_width, _height);
			_app->StartSimulation();
			_open = false;

		}
		ImGui::EndPopup();
	}
}

void CreateComponent::EndDraw()
{
	
}
