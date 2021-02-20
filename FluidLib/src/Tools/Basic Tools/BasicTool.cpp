#include "BasicTool.h"

namespace FluidLib {

	BasicTool::BasicTool()
	{
		SetName("Basic");
		_activeactions.resize(N_MOUSEBUTTONS);
	}

	void BasicTool::Draw()
	{
		if (_moveEdit) {
			_movement->DrawPath();
			_movement->OnEditDraw();
		}
		else if (_surfaceEdit) {
			_surface->Draw();
			_surface->EditDraw();
		}
		else {
			_movement->DrawPath();
			DrawSurface();
			
		}
		OnDraw();
	}

	void BasicTool::OnUpdate()
	{
		if (_action != nullptr) {
			for (const std::string& k : _activeactions) {
				if (k != "" && k != _action->GetName()) {
					GetAction(k)->SetPos(_action->GetPos());
				}
			}
		}
	}

	void BasicTool::AddSurface(const std::string& name, Surface* surface)
	{
		_surfaces.insert(std::make_pair(name, surface));
		_surfacekeys.push_back(name);
	}

	void BasicTool::AddMovement(const std::string& name, Movement* movement)
	{
		_movements.insert(std::make_pair(name, movement));
		_movementkeys.push_back(name);
	}

	void BasicTool::AddAction(const std::string& name, ActionBase* action)
	{
		_actions.insert(std::make_pair(name, action));
		_actionkeys.push_back(name);
		action->SetName(name);
	}

	void BasicTool::AddMultisurface(const std::string& name, MultiSurface* multisurface)
	{
		_multisurfaces.insert(std::make_pair(name, multisurface));
		_multisurfacekeys.push_back(name);
		multisurface->SetName(name);
	}

	void BasicTool::SetActiveSurface(const std::string& surfacename)
	{
		SetSurface(_surfaces[surfacename]);
	}

	void BasicTool::SetActiveMovement(const std::string& movementname)
	{
		SetMovement(_movements[movementname]);
	}

	void BasicTool::SetActiveAction(const std::string& actionname, bool firstactive)
	{
		SetAction(_actions[actionname]);
		if(firstactive)
			_activeactions[0] = actionname;
	}

	void BasicTool::SetActiveAction(const std::string& actionname, int button)
	{
		_activeactions[button] = actionname;
	}

	void BasicTool::SetActiveMultisurface(const std::string& multisurfacename)
	{
		_multisurface = _multisurfaces[multisurfacename];
	}

	void BasicTool::OnBeginUse()
	{
		for (int i = 0; i < N_MOUSEBUTTONS; ++i) {
			if (Simulation::Get()->GetKeys()->mouse[i]) {
				if (_activeactions[i] != "")
					SetActiveAction(_activeactions[i]);
				break;
			}
		}
		_action->Start();
	}

	void BasicTool::OnEndUse()
	{
		_action->Stop();
	}

	void BasicTool::OnUse()
	{
		std::vector<IPoint> points = _surface->GetSurfacePoints();
		std::vector<FPoint> transpoints = { { 0.0f, 0.0f } };
		if(_multisurface != nullptr)
			transpoints = _multisurface->GetPoints();
		
		if (_action != nullptr) {
			for (IPoint& p : points) {
				for (FPoint& t : transpoints) {
					_action->Execute(p + t);
				}
			}
		}
	}

	void BasicTool::DrawSurface()
	{
		if (_multisurface != nullptr) {
			std::vector<FPoint> transpoints = _multisurface->GetPoints();
			for (FPoint& p : transpoints) {
				_surface->SetTrans(p);
				_surface->Draw();
			}
			_surface->SetTrans({ 0.0f, 0.0f });
		}
		else
			_surface->Draw();
	}

}