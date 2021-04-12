#include "SettingsComponent.h"

void SettingsComponent::OnInit()
{
	_settings = FluidLib::Simulation::Get()->GetSettings();
	_lastfps = _settings->fps;
	_pauseicon = Load("res/icons/Pause.png");
	_playicon = Load("res/icons/Play.png");
	//_colorgrid = static_cast<FluidLib::ColorGrid<IInk>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
}

void SettingsComponent::OnUpdate()
{
	if(_colorgrid == nullptr)
		_colorgrid = static_cast<FluidLib::ColorGrid<IInk>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
	if(_flaggrid == nullptr)
		_flaggrid = static_cast<FluidLib::FlagGrid<Flags>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Flag"));
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
			

			ImGui::SliderInt("Updates/sec", &_settings->fps, 0.0, 120.0);
			ImGui::SliderFloat("Ink Spreading", &_settings->spreading, 0.0f, 0.1f);
			ImGui::SliderFloat("Diffuse", &_settings->diffuse, 0.0f, 0.999f);
			ImGui::Checkbox("Edit Frozen", &_settings->editfreeze);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Use actions on frozen parts");
			if (ImGui::Button("Freeze All")) {
				if (_flaggrid != nullptr)
					_flaggrid->SetAllValues({ 1,0,0,0 });
			}
			if (ImGui::Button("UnFreeze All")) {
				if (_flaggrid != nullptr)
					_flaggrid->SetAllValues({ 0,0,0,0 });
			}
		}
		else {
			ImGui::Text("No Settings");
		}
		
		ImGui::TreePop();
		
	}
	ImGui::Separator();
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Simulation Visual Settings")) {
		if (_settings != nullptr) {
			ImGui::SliderFloat("Color Intesity", &_settings->intesity, 0.0f, 1.0f);
			ImGui::SliderFloat("Freeze Intesity", &_settings->freezeintensity, 0.0f, 1.0f);
			if (_colorgrid != nullptr) {
				glm::vec3 col = _colorgrid->GetColor(0);
				ImVec4 imcol = ImVec4(col.x, col.y, col.z, 1);
				ImGui::Text("Water Color");
				ImGui::SameLine();
				ImGui::ColorButton("Water color", imcol);
				if (ImGui::BeginPopupContextItem("watercolorcontext"))
				{
					//std::string editcolname = nameedit + "col";
					ImGui::ColorEdit3("Water Color", &col.x);
					_colorgrid->SetColor(0, col);
					//if (ImGui::Button("Change Color")) {
					//	ERROR("EDIT COLOR NOT implemented");
					//}
					_colorgrid->RefreshColors();
					ImGui::EndPopup();
				}
				ImGui::Checkbox("RenderForce", (bool*)&_settings->renderforce);
				ImGui::SliderFloat("RenderForce", &_settings->renderforce, 0.0f, 1.0f);
			}
		}
		else {
			ImGui::Text("No Settings");
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	
}

void SettingsComponent::EndDraw()
{
}
