#pragma once

#include "ToolBase.h"
#include "../Events/Events.h"
#include <vector>
#include <map>

namespace FluidLib {

	class ToolManager
	{
	public:
		ToolManager();
		~ToolManager();

		void AddTool(const std::string& name, ToolBase* tool);
		//Load Tool Layout
		void Update();
		void Draw();
		//ToolEvent Handler
		void OnEvent(Event& event);
		void SetActive(ToolBase* tool);
		void SetActive(const std::string& tool);
		inline ToolBase* GetActive() { return _activeTool; }
		inline ToolBase* Get(const std::string& name) { return _tools[name]; }
		inline void SetSimSize(float width, float height) { SetProjection(glm::ortho(0.0f, width, height, 0.0f)); }
		inline void SetProjection(glm::mat4 proj) { _projection = proj; if (_activeTool != nullptr) _activeTool->SetProjection(proj); }
		//Iterators
	private:
		std::map<std::string, ToolBase*> _tools;
		//std::vector<ToolBase*> _tools;
		ToolBase* _activeTool = nullptr;
		glm::mat4 _projection = glm::mat4(1.0f);

	};

	class ToolLoader 
	{
	public:
		static bool Load(ToolManager& manger, std::string file);
	};

}