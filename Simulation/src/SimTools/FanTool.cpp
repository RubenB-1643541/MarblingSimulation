#include "FanTool.h"

//namespace FluidLib {

	FanTool::FanTool(FluidLib::Grid<IVelocity>* velgrid)
	{
		FluidLib::FanSurface* fan = new FluidLib::FanSurface();
		fan->SetDensity(30);
		_surface = fan;
		_action = new FluidLib::Action<IVelocity>(IVelocity(100, -100), velgrid, FluidLib::ACTION_OPERATION::ADD);
	}

	void FanTool::Draw()
	{
		_surface->Draw();
	}

	void FanTool::OnUpdate()
	{
		FluidLib::Action<IVelocity>* action = static_cast<FluidLib::Action<IVelocity>*>(_action);
		float xval = 100 * sin(_surface->GetRotation());
		float yval = -100 * cos(_surface->GetRotation());
		action->SetValue({ (int)xval, (int) yval });
		*action->GetScale() = 10;
		//FluidLib::FanSurface* surf = static_cast<FluidLib::FanSurface*>(_surface);
		//surf->SetLen(surf->GetLen() + 1);
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

	bool FanTool::OnMoveEvent(FluidLib::ToolMoveEvent& event)
	{
		_surface->OnMove(event.GetX(), event.GetY());
		_action->SetPos({ (int)event.GetX(), (int)event.GetY() });
		return true;
	}

	bool FanTool::OnScrollEvent(FluidLib::ToolScrollEvent& event)
	{
		_surface->OnScroll(event.GetX(), event.GetY());
		return true;
	}

//}