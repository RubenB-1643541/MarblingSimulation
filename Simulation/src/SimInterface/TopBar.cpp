#include "TopBar.h"
#include "../FileIO/SimulationSaveLoad.h"

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
			Exporter ex;
			ex.Export();
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
