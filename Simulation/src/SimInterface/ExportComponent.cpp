#include "ExportComponent.h"

ExportComponent::ExportComponent()
{
	//_open = true;
}

void ExportComponent::OnInit()
{
	_settings = FluidLib::Simulation::Get()->GetSettings();
	_window_flags |= ImGuiWindowFlags_NoTitleBar;
	_window_flags |= ImGuiWindowFlags_NoCollapse;
	_window_flags |= ImGuiWindowFlags_NoMove;
	_window_flags |= ImGuiWindowFlags_NoResize;
	_window_flags |= ImGuiWindowFlags_Popup;
}

void ExportComponent::OnUpdate()
{
}

void ExportComponent::OnDraw()
{
	if(_opened) {
		ImGui::OpenPopup("Export");
		FluidLib::Simulation::Get()->GetTools()->SetEnabled(false);
		_settings->fps = 0;
		//ImGui::SetNextWindowSize(ImVec2(400, 150));
	}
	if (ImGui::BeginPopupModal("Export", &_opened, _window_flags)) {
		ImGui::Text("Export to png");
		//Export Options
		
		ImGui::Checkbox("Water Transparant", &_wattrans);
		if (_wattrans) {
			ImGui::SameLine();
			ImGui::Checkbox("Ink Transparant", &_inktrans);
		}
		ImGui::SliderFloat("Color Intesity", &_settings->intesity, 0.0f, 1.0f);
		ImGui::Separator();
		ImGui::InputText("File", _file, 128);
		if (ImGui::Button("Select File")) {
			strcpy(_file, FileDialog::Export().c_str());
		}
		ImGui::Spacing();
		if (ImGui::Button("Cancel")) {
			_opened = false;
			FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
			_settings->fps = _oldfps;
		}
		ImGui::SameLine(0, 300.0f);
		if (ImGui::Button("Export")) {
			_ex.SetWaterTrans(_wattrans);
			_ex.SetInkTrans(_inktrans);
			_ex.SetFile(_file);
			_ex.Export(false);
			_opened = false;
			FluidLib::Simulation::Get()->GetTools()->SetEnabled(true);
			_settings->fps = _oldfps;
		}
		ImGui::EndPopup();
	}
}

void ExportComponent::EndDraw()
{
}
