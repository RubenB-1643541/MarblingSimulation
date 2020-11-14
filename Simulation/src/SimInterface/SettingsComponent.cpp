#include "SettingsComponent.h"

void SettingsComponent::OnInit()
{
	_settings = FluidLib::Simulation::Get()->GetSettings();
	_lastfps = _settings->fps;
	_pauseicon = Load("res/icons/Pause.png");
	_playicon = Load("res/icons/Play.png");
}

void SettingsComponent::OnUpdate()
{
}

void SettingsComponent::OnDraw()
{
	if(_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Simulation Settings")) {

		if (_settings != nullptr) {
			
			if (ImGui::ImageButton((void*)_playicon.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 2, ImVec4(1, 1, 1, 1))) {
				_settings->fps = _lastfps;
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Play");
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)_pauseicon.id, _iconSize, ImVec2(0, 0), ImVec2(1, 1), 2, ImVec4(1, 1, 1, 1))) {
				_lastfps = _settings->fps;
				_settings->fps = 0;
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Pause");
			

			ImGui::SliderInt("Updates/sec", &_settings->fps, 0.0, 60.0);
			ImGui::SliderFloat("Ink Spreading", &_settings->spreading, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse", &_settings->diffuse, 0.0f, 1.0f);
		}
		else {
			ImGui::Text("No Settings");
		}
		ImGui::Separator();
		ImGui::TreePop();
		
	}
	
}

void SettingsComponent::EndDraw()
{
}
