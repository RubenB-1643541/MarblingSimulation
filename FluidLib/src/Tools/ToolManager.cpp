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
	void ToolManager::Update()
	{
		if (_activeTool != nullptr)
			_activeTool->Update();
	}
	void ToolManager::Draw()
	{
		if(_activeTool != nullptr)
			_activeTool->Draw();
	}
	void ToolManager::OnEvent(Event& event)
	{
		if (event.GetEventCategory() == EventCategory::ToolEvent && _activeTool != nullptr) {
			_activeTool->OnEvent(event);
		}
	}
	void ToolManager::SetActive(ToolBase* tool)
	{
		if (_activeTool != nullptr)
			_activeTool->OnDeactivate();
		_activeTool = tool;
		_activeTool->SetProjection(_projection);
		_activeTool->OnActivate();
	}
	bool ToolLoader::Load(ToolManager& manger, std::string file)
	{
		return false;
	}
}