#include "ToolParameters.h"

ToolParameters::ToolParameters(FluidLib::ToolManager* tools)
{
	_tools = tools;
}

void ToolParameters::OnInit()
{
	
}

void ToolParameters::OnUpdate()
{
	_active = _tools->GetActive();
}

void ToolParameters::OnDraw()
{
	//if (ImGui::CollapsingHeader("Tool Parameters")) {
	if (ImGui::TreeNode("Tool Parameters")) {
		
		if (_active != nullptr) {
			SurfaceParams();
			MovementParams();
			ActionParams();
		}
		else {
			ImGui::Text("No Active Tool");
		}

		ImGui::TreePop();
	}
	
	
	
	
}

void ToolParameters::EndDraw()
{
	
}

void ToolParameters::SurfaceParams()
{
	if (ImGui::TreeNode("Surface")) {
		FluidLib::Surface* surface = _active->GetSurface();
		FluidLib::Square* sq = static_cast<FluidLib::Square*>(surface);
		if (sq != nullptr) {
			ImGui::SliderFloat("Length", sq->GetLen(), 1.0f, 250.0f);
		}

		ImGui::TreePop();
	}
}

void ToolParameters::MovementParams()
{
	if (ImGui::TreeNode("Movement")) {
		FluidLib::Movement* movement = _active->GetMovement();
		FluidLib::Line* line = static_cast<FluidLib::Line*>(movement);
		if (line != nullptr) {
			ImGui::SliderFloat("Position X", line->GetX(), 0.0f, FluidLib::Simulation::Get()->GetSizeX());
			ImGui::SliderFloat("Position Y", line->GetY(), 0.0f, FluidLib::Simulation::Get()->GetSizeY());
			ImGui::SliderFloat("Length", line->GetLen(), 1.0f, FluidLib::Simulation::Get()->GetSizeX() - *line->GetX());
		}


		ImGui::TreePop();
	}
}

void ToolParameters::ActionParams()
{
	if (ImGui::TreeNode("Action")) {


		ImGui::TreePop();
	}
}
