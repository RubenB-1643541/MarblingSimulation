#include "SelectTool.h"
#include "Simulation.h"
namespace FluidLib {
	SelectTool::SelectTool()
	{
		_rect = new Rectangle();
		_surface = _rect;
		_move = MOVEMENT_MODE::BEGIN;
		_rect->SetCentred(false);
	}
	void SelectTool::Draw()
	{
		_surface->Draw();
	}

	void SelectTool::OnUpdate()
	{
		if (Simulation::Get()->GetKeys()->keys[73]) {// i
			_using = true;
			_action->SetPos(IPoint(_rect->GetX(), _rect->GetY()));
		}
		else
			_using = false;
	}

	void SelectTool::OnActivate()
	{
		_move = MOVEMENT_MODE::BEGIN;
	}

	void SelectTool::OnDeactivate()
	{
	}

	bool SelectTool::OnUseEvent(ToolUseEvent& event)
	{
		SwitchMode();
		return true;
	}

	bool SelectTool::OnMoveEvent(ToolMoveEvent& event)
	{
		if (_move == MOVEMENT_MODE::BEGIN) {
			_rect->SetX(event.GetX());
			_rect->SetY(event.GetY());
		}
		else if (_move == MOVEMENT_MODE::SIZE) {
			_rect->SetWidth(event.GetX() - _rect->GetX());
			_rect->SetHeight(event.GetY() - _rect->GetY());
		}
		else {

		}

		return false;
	}

	bool SelectTool::OnScrollEvent(ToolScrollEvent& event)
	{
		return false;
	}

	void SelectTool::SwitchMode()
	{
		if (_move == MOVEMENT_MODE::BEGIN)
			_move = MOVEMENT_MODE::SIZE;
		else if (_move == MOVEMENT_MODE::SIZE)
			_move = MOVEMENT_MODE::NONE;
		else
			_move = MOVEMENT_MODE::BEGIN;
	}

}