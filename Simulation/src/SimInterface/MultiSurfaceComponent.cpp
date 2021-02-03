#include "MultiSurfaceComponent.h"

MultiSurfaceComponent::MultiSurfaceComponent(FluidLib::ToolManager* tools, const std::string& basic)
{
	_tools = tools;
	_basicstr = basic;
}

void MultiSurfaceComponent::OnInit()
{
}

void MultiSurfaceComponent::OnUpdate()
{
	if (_basic == nullptr) {
		_basic = static_cast<FluidLib::BasicTool*>(_tools->Get(_basicstr));
		if (_basic != nullptr) {
			_selectedstr = _basic->GetMultisurface()->GetName();
			for (int i = 0; i < _basic->GetMultisurfaceKeys().size(); ++i) {
				if (_selectedstr == _basic->GetMultisurfaceKeys()[i])
					_selected = i;
			}
		}

	}
	_active = _tools->GetActive();
}

void MultiSurfaceComponent::OnDraw()
{
	if (_active != nullptr &&_active->GetName() == _basicstr) {
		static bool first = true;
		if (first) {
			ImGui::SetNextTreeNodeOpen(true);
			first = false;
		}
		if (ImGui::TreeNode("Multisurfaces")) {
			if(_basic != nullptr){
				std::vector<std::string> surfaces = _basic->GetMultisurfaceKeys();
				if (ImGui::BeginCombo("Multisurfaces", _selectedstr.c_str())) {
					for (int i = 0; i < surfaces.size(); ++i) {
						if (ImGui::Selectable(surfaces[i].c_str(), i == _selected)) {
							_selected = i;
							_selectedstr = surfaces[i];
							_basic->SetActiveMultisurface(_selectedstr);
						}
					}
					ImGui::EndCombo();
				}
				if (_selectedstr == "CrossSurface")
					DrawCrossSurface(static_cast<FluidLib::CrossSurface*>(_basic->GetMultisurface()));
				else if (_selectedstr == "Comb")
					DrawCombSurface(static_cast<FluidLib::Comb*>(_basic->GetMultisurface()));
			}
			ImGui::TreePop();
			ImGui::Separator();
		}
	}
}

void MultiSurfaceComponent::EndDraw()
{
}

void MultiSurfaceComponent::DrawCrossSurface(FluidLib::CrossSurface* s)
{
	ImGui::SliderFloat("Length", s->GetLenPtr(), 0.0f, 200.0f);
	ImGui::Checkbox("X", s->GetXPtr());
}

void MultiSurfaceComponent::DrawCombSurface(FluidLib::Comb* s)
{
	ImGui::SliderFloat("Length", s->GetLenPtr(), 0.0f, 500.0f);
	ImGui::SliderInt("Repetition", s->GetRepPtr(), 0, 50);
	ImGui::Checkbox("Vertical", s->GetVertPtr());
}
