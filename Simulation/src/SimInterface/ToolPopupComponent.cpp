#include "ToolPopupComponent.h"

ToolPopupComponent::ToolPopupComponent(FluidLib::ToolManager* tools) : _tools(tools)
{
}

void ToolPopupComponent::OnInit()
{
}

void ToolPopupComponent::OnUpdate()
{
    _active = _tools->GetActive();
}

void ToolPopupComponent::OnDraw()
{
    if (_active == nullptr || _active->GetName() != "Select")
        return;

    if (ImGui::GetIO().MouseClicked[1])
    {
        //ImGuiState& g = *GImGui;
        //if (g.OpenedPopupStack.size() > 0)
        //    g.OpenedPopupStack.pop_back();
        _tools->SetEnabled(false);
        ImGui::OpenPopup("ToolPopup");
        _open = true;
    }

    if (ImGui::BeginPopup("ToolPopup")) {
        if (_active->GetName() == "Select") {
            DrawSelectPopup();
        }
        ImGui::EndPopup();
    }
    else {
        if (_open) {
            _tools->SetEnabled(true);
            _open = false;
        }
    }
}

void ToolPopupComponent::EndDraw()
{
}

void ToolPopupComponent::DrawSelectPopup()
{
    if (ImGui::Selectable("Copy  (ctrl+c)", false)) {
        FluidLib::SelectTool* s = static_cast<FluidLib::SelectTool*>(_active);
        s->Copy();
    }
    if (ImGui::Selectable("Paste (ctrl+v)", false)) {
        FluidLib::SelectTool* s = static_cast<FluidLib::SelectTool*>(_active);
        s->Paste();
    }
    if (ImGui::Selectable("Cut   (ctrl+x)", false)) {
        FluidLib::SelectTool* s = static_cast<FluidLib::SelectTool*>(_active);
        s->Cut();
    }
}
