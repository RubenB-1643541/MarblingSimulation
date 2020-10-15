#include "TopBar.h"

void TopBar::OnInit()
{

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
		if (ImGui::MenuItem("Save", "Ctrl+S")) { INFO("Save"); }
		ImGui::Separator();
		if (ImGui::MenuItem("Export")) {
			INFO("EXPORT");
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Window")) {
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools")) {
		ImGui::EndMenu();
	}

}

void TopBar::EndDraw()
{
	ImGui::EndMainMenuBar();
}
