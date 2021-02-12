#include "TopBar.h"
#include "../FileIO/SimulationSaveLoad.h"
#include <Core/Application.h>
#include "../SimUtils/SaveStateHandler.h"

void TopBar::OnInit()
{
	_settings = FluidLib::Simulation::Get()->GetSettings();
}

void TopBar::OnUpdate()
{

}

void TopBar::OnDraw()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::MenuItem("Save", "Ctrl+S")) { 
			INFO("Save"); 
			SimSave saver;
			saver.SetGrids(FluidLib::Simulation::Get()->GetGrids());
			saver.Save();
		}
		if (ImGui::MenuItem("Save as", "Ctrl+S")) {
			INFO("Save as");
			SimSave saver;
			std::string file = FileDialog::Save();
			if (file != "") {
				saver.SetFile(file);
				saver.SetGrids(FluidLib::Simulation::Get()->GetGrids());
				saver.Save();
			}
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Export")) {
			INFO("EXPORT");
			if(_export != nullptr)
				_export->Open();
			//Exporter ex;
			//ex.Export();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Edit Surface", "Ctrl", &_settings->surfaceedit)) {
			_settings->movementedit = false;
		}
		if (ImGui::MenuItem("Edit Movement", "Shift", &_settings->movementedit)) {
			_settings->surfaceedit = false;
		}
		if (ImGui::MenuItem("Create Save State", "Ctrl+E"))
			SaveStateHandler::CreateSaveState();
		if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			SaveStateHandler::PrevState();
		if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z"))
			SaveStateHandler::NextState();

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Window")) {
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools")) {
		if (ImGui::MenuItem("Basic", "Shift+B"))
			FluidLib::Simulation::Get()->GetTools()->SetActive("Basic");
		if (ImGui::MenuItem("Select", "Shift+S")) 
			FluidLib::Simulation::Get()->GetTools()->SetActive("Select");
		
		ImGui::EndMenu();
	}

}

void TopBar::EndDraw()
{
	ImGui::EndMainMenuBar();
}
