#include "ToolBase.h"

namespace FluidLib {
    bool ToolBase::OnEvent(Event& toolevent)
    {
        EventDisplatcher dispatcher(toolevent);
        dispatcher.Dispatch<ToolSelectEvent>(BIND_EVENT(OnSelectEvent));
        dispatcher.Dispatch<ToolUseEvent>(BIND_EVENT(OnUseEvent));
        dispatcher.Dispatch<ToolMoveEvent>(BIND_EVENT(OnMoveEvent));
        dispatcher.Dispatch<ToolScrollEvent>(BIND_EVENT(OnScrollEvent));
        return true;
    }
}