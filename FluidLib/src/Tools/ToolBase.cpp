#include "ToolBase.h"

namespace FluidLib {
    void ToolBase::Draw()
    {
        _movement->DrawPath();
        _surface->Draw();
        OnDraw();
    }
    void ToolBase::Update()
    {
        if (_using) {
            OnBeginUse();
            OnUse();
            OnEndUse();
        }
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
        //OnEndUse();
        return true;
    }
    bool ToolBase::OnMoveEvent(ToolMoveEvent& event)
    {
        FPoint p = _movement->Get(event.GetX(), event.GetY());
        _surface->OnMove(p.GetX(), p.GetY());
        _action->SetPos({ (int)p.GetX(), (int)p.GetY() });
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