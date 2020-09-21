#include "ToolManager.h"

namespace FluidLib {

	ToolManager::ToolManager()
	{
	}
	ToolManager::~ToolManager()
	{
		for (ToolBase* tool : _tools) {
			delete tool;
		}
	}
	void ToolManager::AddTool(ToolBase* tool)
	{
		_tools.push_back(tool);
	}
	void ToolManager::OnEvent(Event& event)
	{
		if (event.GetEventCategory() == EventCategory::ToolEvent && _activeTool != nullptr) {
			_activeTool->OnEvent(event);
		}
	}
	bool ToolLoader::Load(ToolManager& manger, std::string file)
	{
		return false;
	}
}