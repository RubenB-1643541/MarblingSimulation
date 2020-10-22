#include "SettingsComponent.h"

void SettingsComponent::OnInit()
{
	_settings = FluidLib::Simulation::Get()->GetSettings();
	_lastfps = _settings->fps;
}

void SettingsComponent::OnUpdate()
{
}

void SettingsComponent::OnDraw()
{

		if (ImGui::TreeNode("Simulation Settings")) {

			if (_settings != nullptr) {
				if (ImGui::Button("Pause")) {
					_lastfps = _settings->fps;
					_settings->fps = 0;
				}
				if (ImGui::Button("Play")) {
					_settings->fps = _lastfps;
				}

				ImGui::SliderInt("Updates/sec", &_settings->fps, 0.0, 60.0);
				ImGui::SliderFloat("Ink Spreading", &_settings->spreading, 0.0001, 0.5f);
			}
			else {
				ImGui::Text("No Settings");
			}

			ImGui::TreePop();
		}
	
}

void SettingsComponent::EndDraw()
{
}
