#include "DrippingTool.h"


namespace FluidLib {

	DrippingTool::DrippingTool()
	{
		SetName("Dripping");
		_circle = new Circle();
		SetSurface(_circle);
		_rect = new Rectangle();
	}

	void DrippingTool::Draw()
	{
		if (_surfaceEdit) {
			_rect->Draw();
			_rect->EditDraw();
		}
		else {
			_rect->SetWidth(_width);
			_rect->SetHeight(_height);
			_rect->OnMove(_xpos, _ypos);
		}
	}

	void DrippingTool::OnUpdate()
	{
	}

	void DrippingTool::OnActivate()
	{
	}

	void DrippingTool::OnDeactivate()
	{
	}

	void DrippingTool::OnUse()
	{
		if(_arc)
			SetCircleRandomArc();
		else
			SetCircleRandom();
		std::vector<IPoint> points = _surface->GetSurfacePoints();

		for (IPoint& p : points)
			_action->Execute(p);
	}

	bool DrippingTool::OnUseEvent(ToolUseEvent& event)
	{
		if (_surfaceEdit) {
			_using = false;
			_rect->OnEditClick(_xpos, _ypos);
		}
		else
			_using = true;
		return true;
	}

	bool DrippingTool::OnEndUseEvent(ToolEndUseEvent& event)
	{
		if (_surfaceEdit) {
			_rect->OnEditRelease(_xpos, _ypos);
		}
		_using = false;
		return false;
	}

	bool DrippingTool::OnMoveEvent(ToolMoveEvent& event)
	{
		_xpos = event.GetX();
		_ypos = event.GetY();
		if (_surfaceEdit) {
			_rect->OnEditMove(_xpos, _ypos);
			_width = _rect->GetWidth();
			_height = _rect->GetHeight();
		}
		return false;
	}

	bool DrippingTool::OnScrollEvent(ToolScrollEvent& event)
	{
		return false;
	}

	//https://forum.processing.org/one/topic/how-to-create-a-paint-splatter-effect.html
	void DrippingTool::SetCircleRandom()
	{
		float ang = _minr + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX /(_maxr - _minr)));
		//float ang = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / M_2_PI));
		float splatX = _xpos - _width/2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / _width));
		float splatY = _ypos - _height/2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / _height));
		_circle->OnMove(splatX, splatY);
		_action->SetPos({ (int)splatX, (int)splatY });
		_circle->SetR(ang);
	}

	void DrippingTool::SetCircleRandomArc()
	{
		float ang = _minr + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_maxr - _minr)));
		//float ang = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / M_2_PI));
		float ranlen = _minlen + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_maxlen - _minlen)));
		float ranang = _minangle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_maxangle - _minangle)));
		float splatX = _xpos - cos(ranang + M_PI/2) * ranlen;
		float splatY = _ypos - sin(ranang + M_PI / 2) * ranlen;
		_circle->OnMove(splatX, splatY);
		_action->SetPos({ (int)splatX, (int)splatY });
		_circle->SetR(ang);
	}

	

}