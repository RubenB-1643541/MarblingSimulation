#include "SelectTool.h"
#include "Simulation.h"

namespace FluidLib {
	SelectTool::SelectTool()
	{
		SetName("Select");
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
		if (!_moveEdit && !_surfaceEdit) {
			if (_move != MOVEMENT_MODE::SIZE)
				SwitchMode();
		}
		return true;
	}

	bool SelectTool::OnEndUseEvent(ToolEndUseEvent& event)
	{
		if (!_moveEdit && !_surfaceEdit) {
			if (_move == MOVEMENT_MODE::SIZE)
				SwitchMode();
		}
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

	void SelectTool::SetCopyAction(ActionBase* copyaction)
	{
		AddAction("Copy", copyaction);
		//_actions["Copy"] = copyaction;
		//copyaction->SetName("Copy");
	}

	void SelectTool::SetPasteAction(ActionBase* pasteaction)
	{
		AddAction("Paste", pasteaction);
		//_actions["Paste"] = pasteaction;
		//pasteaction->SetName("Paste");
	}

	void SelectTool::SetCutAction(ActionBase* cutaction)
	{
		AddAction("Cut", cutaction);
		//_actions["Cut"] = cutaction;
		//cutaction->SetName("Cut");
	}

	void SelectTool::AddAction(const std::string& actionname, ActionBase* action)
	{
		_actions[actionname] = action;
		action->SetName(actionname);
		_actionnames.push_back(actionname);
	}

	bool SelectTool::ExecuteAction(const std::string& action)
	{
		std::vector<IPoint> points = _surface->GetSurfacePoints();

		if (_actions.find(action) != _actions.end()) {
			ActionBase* ac = _actions.at(action);
			ac->SetPos(IPoint(_rect->GetX(), _rect->GetY()));
			ac->Start();
			for (IPoint& p : points)
				ac->Execute(p);
			ac->Stop();
			return true;
		}
		else
			return false;
	}

	bool SelectTool::Copy()
	{
		std::vector<IPoint> points = _surface->GetSurfacePoints();
		
		if (_actions.find("Copy") != _actions.end()) {
			ActionBase* copy = _actions.at("Copy");
			copy->SetPos(IPoint(_rect->GetX(), _rect->GetY()));
			copy->Start();
			for (IPoint& p : points)
				copy->Execute(p);
			copy->Stop();
			return true;
		}
		else
			return false;
		
	}

	bool SelectTool::Paste()
	{
		if (_actions.find("Paste") != _actions.end()) {
			ActionBase* paste = _actions.at("Paste");
			paste->Start();
			size_t width = Clipboard::GetDataStruct()->width;
			size_t height = Clipboard::GetDataStruct()->height;
			for (int i = 0; i < width; ++i) {
				for (int j = 0; j < height; ++j) {
					paste->Execute(IPoint(i, j));
				}
			}
			paste->Stop();
			return true;
		}
		else
			return false;
	}

	bool SelectTool::Cut()
	{
		std::vector<IPoint> points = _surface->GetSurfacePoints();

		if (_actions.find("Cut") != _actions.end()) {
			ActionBase* copy = _actions.at("Cut");
			copy->SetPos(IPoint(_rect->GetX(), _rect->GetY()));
			copy->Start();
			for (IPoint& p : points)
				copy->Execute(p);
			copy->Stop();
			return true;
		}
		else
			return false;
	}

	void SelectTool::SwitchMode()
	{
		if (_move == MOVEMENT_MODE::BEGIN) {
			_move = MOVEMENT_MODE::SIZE;

		}
		else if (_move == MOVEMENT_MODE::SIZE) {
			_move = MOVEMENT_MODE::NONE;
		}
		else {
			_move = MOVEMENT_MODE::BEGIN;
			_rect->SetWidth(0);
			_rect->SetHeight(0);
		}
	}

}