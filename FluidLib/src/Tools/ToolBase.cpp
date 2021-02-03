#include "ToolBase.h"
#include "Simulation.h"
namespace FluidLib {
    void ToolBase::Draw()
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
            _surface->Draw();
        }
        OnDraw();
    }
    void ToolBase::Update()
    {
        if (Simulation::Get()->GetKeys()->shift || FluidLib::Simulation::Get()->GetSettings()->movementedit) {
            _moveEdit = true;
            _surfaceEdit = false;
            _using = false;
        }
        else if(Simulation::Get()->GetKeys()->ctr || FluidLib::Simulation::Get()->GetSettings()->surfaceedit){
            if (!_surfaceEdit)
                _surface->StartEdit();
            _moveEdit = false;
            _surfaceEdit = true;
            _using = false;
        }
        else {
            if (!_moveEdit)
                _movement->StartEdit();
            _moveEdit = false;
            _surfaceEdit = false;
            FPoint p = _movement->Get(_x, _y);
            _surface->OnMove(p.GetX(), p.GetY());
            _action->SetPos({ (int)p.GetX(), (int)p.GetY() });
        }
        if (_using) {
            OnBeginUse();
            OnUse();
            OnEndUse();
        }
        OnUpdate();
    }
    bool ToolBase::OnEvent(Event& toolevent)
    {
        EventDisplatcher dispatcher(toolevent);
       // dispatcher.Dispatch<ToolSelectEvent>(BIND_EVENT(OnSelectEvent));
        dispatcher.Dispatch<ToolUseEvent>(BIND_EVENT(OnUseEvent));
        dispatcher.Dispatch<ToolMoveEvent>(BIND_EVENT(OnMoveEvent));
        dispatcher.Dispatch<ToolScrollEvent>(BIND_EVENT(OnScrollEvent));
        dispatcher.Dispatch<ToolEndUseEvent>(BIND_EVENT(OnEndUseEvent));
        return true;
    }
    bool ToolBase::OnUseEvent(ToolUseEvent& event)
    {
        if (_moveEdit) {
            _movement->OnEditClick(_x, _y);
            return true;
        }
        else if (_surfaceEdit) {
            _surface->OnEditClick(_x, _y);
        }
        if (!_using) {
            //OnBeginUse();
            _using = true;
        }
        //OnUse();
        //OnEndUse();
        return true;
    }
    bool ToolBase::OnEndUseEvent(ToolEndUseEvent& event)
    {
        _using = false;
        if (_moveEdit) {
            _movement->OnEditRelease(_x, _y);
            return true;
        }
        else if (_surfaceEdit) {
            _surface->OnEditRelease(_x, _y);
        }
        //OnEndUse();
        return true;
    }
    bool ToolBase::OnMoveEvent(ToolMoveEvent& event)
    {
        _x = event.GetX();
        _y = event.GetY();
        if (_moveEdit) {
            _movement->OnEditMove(_x, _y);

            FPoint p = _movement->Get(_x, _y);
            _surface->OnMove(p.GetX(), p.GetY());
            _action->SetPos({ (int)p.GetX(), (int)p.GetY() });
        }
        else if (_surfaceEdit) {
            _surface->OnEditMove(_x, _y);
        }
        //else {
        //    FPoint p = _movement->Get(_x, _y);
        //    _surface->OnMove(p.GetX(), p.GetY());
        //    _action->SetPos({ (int)p.GetX(), (int)p.GetY() });
        //}
        return true;
    }
    bool ToolBase::OnScrollEvent(ToolScrollEvent& event)
    {
        _surface->OnScroll(event.GetX(), event.GetY());
        _movement->OnScroll(event.GetX(), event.GetY());
        return true;
    }
    void ToolBase::OnBeginUse()
    {
        _action->Start();
    }
    void ToolBase::OnEndUse()
    {
        _action->Stop();
    }
    void ToolBase::OnUse()
    {
        std::vector<IPoint> points = _surface->GetSurfacePoints();
        
        for(IPoint& p : points)
            _action->Execute(p); 
    }
}