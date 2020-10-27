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
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Tool Parameters")) {
		
		if (_active != nullptr) {
			SurfaceParams();
			MovementParams();
			ActionParams();
		}
		else {
			ImGui::Text("No Active Tool"); 
			ImGui::SetNextTreeNodeOpen(true);
			ImGui::TreeNode("Surface");
			ImGui::TreePop();
			ImGui::SetNextTreeNodeOpen(true);
			ImGui::TreeNode("Movement");
			ImGui::TreePop();
			ImGui::SetNextTreeNodeOpen(true);
			ImGui::TreeNode("Action");
			ImGui::TreePop();
		}
		ImGui::Separator();
		ImGui::TreePop();
	}
	
}

void ToolParameters::EndDraw()
{
	
}

void ToolParameters::SurfaceParams()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Surface")) {
		FluidLib::Surface* surface = _active->GetSurface();
		if (surface->GetType() == "Square") {
			FluidLib::Square* sq = static_cast<FluidLib::Square*>(surface);
			if (sq != nullptr) {
				ImGui::SliderFloat("Length", sq->GetLen(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Rectangle") {
			FluidLib::Rectangle* sq = static_cast<FluidLib::Rectangle*>(surface);
			if (sq != nullptr) {
				ImGui::SliderFloat("Width", sq->GetWidth(), 1.0f, 250.0f);
				ImGui::SliderFloat("Height", sq->GetHeight(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Circle") {
			FluidLib::Circle* c = static_cast<FluidLib::Circle*>(surface);
			if (c != nullptr) {
				ImGui::SliderFloat("Radius", c->GetR(), 1.0f, 250.0f);
			}
		}

		ImGui::TreePop();
	}
}

void ToolParameters::MovementParams()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Movement")) {
		FluidLib::Movement* movement = _active->GetMovement();
		if (movement->GetType() == "Line") {
			FluidLib::Line* line = static_cast<FluidLib::Line*>(movement);
			if (line != nullptr) {
				ImGui::SliderFloat("Position X", line->GetX(), 0.0f, FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat("Position Y", line->GetY(), 0.0f, FluidLib::Simulation::Get()->GetSizeY());
				ImGui::SliderFloat("Length", line->GetLen(), 1.0f, FluidLib::Simulation::Get()->GetSizeX() - *line->GetX());
			}
		}
		else if (movement->GetType() == "Sine") {
			
			FluidLib::Sine* sine = static_cast<FluidLib::Sine*>(movement);
			if (sine != nullptr) {
				ImGui::SliderFloat("HTranslation", sine->GetHortrans(), 0.0f, 1000);
				ImGui::SliderFloat("VTranslation", sine->GetVerttrans(), 0.0f, FluidLib::Simulation::Get()->GetSizeY());
				ImGui::SliderFloat("Amplitude", sine->GetAmp(), 0.0f, 250);
				ImGui::SliderFloat("Period", sine->GetPeriod(), 1.0f, 300);
			}
		}
		
		else if (movement->GetType() == "BezierCurve") {

			FluidLib::BezierCurve *curve = static_cast<FluidLib::BezierCurve*>(movement);
			if (curve != nullptr) {
				//ImGui::Text("P1");
				//ImGui::SameLine();
				//ImGui::SliderFloat("X1", curve->GetPoints()[0].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SameLine();
				//ImGui::SliderFloat("Y1", curve->GetPoints()[0].GetYPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::Text("P2");
				//ImGui::SameLine();
				//ImGui::SliderFloat("X2", curve->GetPoints()[1].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SameLine();
				//ImGui::SliderFloat("Y2", curve->GetPoints()[1].GetYPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::Text("P3");
				//ImGui::SameLine();
				//ImGui::SliderFloat("X3", curve->GetPoints()[2].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SameLine();
				//ImGui::SliderFloat("Y3", curve->GetPoints()[2].GetYPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::Text("P4");
				//ImGui::SameLine();
				//ImGui::SliderFloat("X4", curve->GetPoints()[3].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SameLine();
				//ImGui::SliderFloat("Y4", curve->GetPoints()[3].GetYPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P1", curve->GetPoints()[0].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P2", curve->GetPoints()[1].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P3", curve->GetPoints()[2].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P4", curve->GetPoints()[3].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());

			}
		}


		ImGui::TreePop();
	}
}

void ToolParameters::ActionParams()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::TreeNode("Action")) {
		FluidLib::ActionBase* action = _active->GetAction();
		if (action != nullptr) {
			ImGui::SliderFloat("Scale", action->GetScale(), 0.1f, 10.0f);
		}
		ImGui::TreePop();
	}
}
