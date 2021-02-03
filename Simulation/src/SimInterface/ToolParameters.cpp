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
			ImGui::TreeNode("Surface (Ctrl)");
			ImGui::TreePop();
			ImGui::SetNextTreeNodeOpen(true);
			ImGui::TreeNode("Movement (Shift)");
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
	FluidLib::Surface* surface = _active->GetSurface();
	if (surface != nullptr && ImGui::TreeNode("Surface (Ctrl)")) {
		if (surface->GetType() == "Square") {
			FluidLib::Square* sq = static_cast<FluidLib::Square*>(surface);
			if (sq != nullptr) {
				ImGui::SliderFloat("Length", sq->GetLen(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Rectangle") {
			FluidLib::Rectangle* sq = static_cast<FluidLib::Rectangle*>(surface);
			if (sq != nullptr) {
				ImGui::SliderFloat("Width", sq->GetWidthPtr(), 1.0f, 250.0f);
				ImGui::SliderFloat("Height", sq->GetHeightPtr(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Triangle") {
			FluidLib::Triangle* tr = static_cast<FluidLib::Triangle*>(surface);
			if (tr != nullptr) {
				ImGui::SliderFloat("Width", tr->GetWidthPtr(), 1.0f, 250.0f);
				ImGui::SliderFloat("Height", tr->GetHeightPtr(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Circle") {
			FluidLib::Circle* c = static_cast<FluidLib::Circle*>(surface);
			if (c != nullptr) {
				ImGui::SliderFloat("Radius", c->GetR(), 1.0f, 250.0f);
			}
		}
		else if (surface->GetType() == "Polygon") {
			FluidLib::Polygon* p = static_cast<FluidLib::Polygon*>(surface);
			if (p != nullptr) {
				std::string s = "P";
				for (int i = 0; i < p->PointCount(); ++i) {
					std::string name = s + std::to_string(i);
					ImGui::SliderFloat2(name.c_str(), p->GetPoint(i).GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				}
				if (ImGui::Button("Add Point")) {
					p->AddPoint({ 0.0f,0.0f });
				}
			}
		}

		ImGui::TreePop();
	}
}

void ToolParameters::MovementParams()
{
	if (_first)
		ImGui::SetNextTreeNodeOpen(true);
	FluidLib::Movement* movement = _active->GetMovement();
	if (movement != nullptr && ImGui::TreeNode("Movement (Shift)")) {
		if (movement->GetType() == "Line") {
			FluidLib::Line* line = static_cast<FluidLib::Line*>(movement);
			if (line != nullptr) {
				ImGui::SliderFloat2("P1", line->GetP1Ptr()->GetXPtr(), 0, FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P2", line->GetP2Ptr()->GetXPtr(), 0, FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SliderFloat("Position X", line->GetX(), 0.0f, FluidLib::Simulation::Get()->GetSizeX());
				//ImGui::SliderFloat("Position Y", line->GetY(), 0.0f, FluidLib::Simulation::Get()->GetSizeY());
				//ImGui::SliderFloat("Length", line->GetLen(), 1.0f, FluidLib::Simulation::Get()->GetSizeX() - *line->GetX());
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
				ImGui::SliderFloat2("P1", curve->GetPoints()[0].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P2", curve->GetPoints()[1].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P3", curve->GetPoints()[2].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat2("P4", curve->GetPoints()[3].GetXPtr(), -FluidLib::Simulation::Get()->GetSizeX(), FluidLib::Simulation::Get()->GetSizeX());

			}
		}

		else if (movement->GetType() == "CircleMovement") {
			FluidLib::CircleMovement* c = static_cast<FluidLib::CircleMovement*>(movement);
			if (c != nullptr) {
				ImGui::SliderFloat2("Position", c->GetXPtr(), 0.0f, FluidLib::Simulation::Get()->GetSizeX());
				ImGui::SliderFloat("Radius", c->GetRPtr(), 1.0f, 250.0f);
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
			if (action->GetName() == "AddInk") {
				FluidLib::InkAction<IInk>* inkaction = static_cast<FluidLib::InkAction<IInk>*>(action);
				if (inkaction != nullptr) {
					InkActionParams(inkaction);
				}
			}
			if (_active->GetName() == "Select") {
				SelectToolActions();
			}
		}
		ImGui::TreePop();
	}
}

void ToolParameters::InkActionParams(FluidLib::InkAction<IInk>* inkaction)
{
	ImGui::ColorEdit3("Ink Color", &inkaction->GetColorPtr()->x);
	if (ImGui::BeginPopupContextItem("AddToPalette"))
	{
		if (ImGui::Button("Add Color To Palette")) {
			inkaction->AddCurrentColorToPalette();
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Add Color To Palette")) {
		inkaction->AddCurrentColorToPalette();
	}
	std::string colstr = "Color ";
	static bool first = true;
	if (first) {
		ImGui::SetNextTreeNodeOpen(true);
		first = false;
	}
	if (ImGui::TreeNode("Color Palette")) {
		//for (glm::vec3& col : inkaction->GetColors()) {
			std::vector<glm::vec4> colors = inkaction->GetColors();
		for(int i = 1; i < colors.size(); ++i) {
			std::string name = colstr + std::to_string(i);
			glm::vec3 col = colors[i];
			ImVec4 vec(col.x, col.y, col.z, 1);
			if (i != 1 && i % 5 != 0)
				ImGui::SameLine();
			if (ImGui::ColorButton(name.c_str(), vec)) {
				INFO("SELECTED {0}", i);
				inkaction->SelectColor(i);
			}
			std::string edit = "edit color ";
			std::string nameedit = edit + std::to_string(i);
			if (ImGui::BeginPopupContextItem(nameedit.c_str()))
			{
				//std::string editcolname = nameedit + "col";
				ImGui::ColorEdit3("Ink Color", &colors[i].x);
				inkaction->SetColor(i, colors[i]);
				//if (ImGui::Button("Change Color")) {
				//	ERROR("EDIT COLOR NOT implemented");
				//}
				inkaction->Refresh();
				ImGui::EndPopup();
			}
		}
		ImGui::TreePop();
	}
}

void ToolParameters::SelectToolActions()
{
	FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(_active);
	const std::vector<std::string>& acts = select->GetActions();
	for (int i = 0; i < acts.size(); ++i) {
		if (ImGui::Button(acts[i].c_str())) {
			if(i != 0)
				ImGui::SameLine();
			select->ExecuteAction(acts[i]);
		}
	}
	//if (ImGui::Button("Copy")) {
	//	select->Copy();
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Cut")) {
	//	select->Cut();
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Paste")) {
	//	select->Paste();
	//}

}
