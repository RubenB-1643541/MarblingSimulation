#include "FanTool.h"

#include "../Surface/FanSurface.h"

namespace FluidLib {

	FanTool::FanTool()
	{
		_surface = new FanSurface();
	}

	void FanTool::Draw()
	{
		_surface->Draw();
	}

	void FanTool::OnUpdate()
	{
	}

	void FanTool::OnActivate()
	{
	}

	void FanTool::OnDeactivate()
	{
	}

	//bool FanTool::OnUseEvent(ToolUseEvent& event)
	//{
	//	return false;
	//}
	//
	//bool FanTool::OnEndUseEvent(ToolEndUseEvent& event)
	//{
	//	return false;
	//}

	bool FanTool::OnMoveEvent(ToolMoveEvent& event)
	{
		_surface->OnMove(event.GetX(), event.GetY());
		_action->SetPos({ (int)event.GetX(), (int)event.GetY() });
		return true;
	}

	bool FanTool::OnScrollEvent(ToolScrollEvent& event)
	{
		_surface->OnScroll(event.GetX(), event.GetY());
		return true;
	}

}