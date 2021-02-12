#include "SaveStateComponent.h"

SaveStateComponent::SaveStateComponent()
{
}

void SaveStateComponent::OnInit()
{
}

void SaveStateComponent::OnUpdate()
{
}

void SaveStateComponent::OnDraw()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Save states")) {
		if (ImGui::Button("Add save state")) {
			INFO("Add Save State");
			SaveStateHandler::CreateSaveState();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Auto", &FluidLib::Simulation::Get()->GetSettings()->autosavestate);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Create automatic savestate after every tool use");
		DrawStates();
		ImGui::TreePop();
	}
}

void SaveStateComponent::EndDraw()
{
}

void SaveStateComponent::DrawStates()
{
	ImGui::BeginChild("Save state list",ImVec2(0, 300), true);
	std::vector<State> states = SaveStateHandler::GetStates();
	int i = 1;
	for (State& s : states) {
		std::string state = "Save state " + std::to_string(s.n);
		if (ImGui::Selectable(state.c_str(), i == SaveStateHandler::GetCurrentStateNumber())) {
			SaveStateHandler::LoadState(s);
		}
		++i;
	}
	ImGui::EndChild();
}
