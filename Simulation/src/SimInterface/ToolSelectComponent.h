#pragma once

#include "Interface/InterfaceComponent.h"
#include "Tools/ToolManager.h"
#include "Tools/Basic Tools/BasicTool.h"
#include "Tools/Basic Tools/SelectTool.h"
#include <string>
#include "GL/glew.h"


enum class TOOL_PART {SURFACE, MOVEMENT, ACTION, TOOL, SELECT_ACTION};

struct Button {
	std::string icon;
	GLuint id = -1;
	std::string name;
	TOOL_PART part;
	Button(const std::string& icon, const std::string& name, TOOL_PART part) : icon(icon), name(name), part(part) {}
};

class ToolSelectComponent : public RenderEngine::InterfaceComponent
{
public:
	ToolSelectComponent(FluidLib::ToolManager* tools, const std::string& basic = "Basic");
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
	void AddButton(Button button, bool selected = false);
	void UpdateSelection(const std::string& surface, const std::string& movement, const std::string& action);
	const std::string& GetSelectedSurface();
	const std::string& GetSelectedMovement();
	const std::string& GetSelectedAction();
private:
	void DrawBasic();
	void DrawSelect();
	void DrawSelection();
	FluidLib::ToolManager* _tools = nullptr;
	FluidLib::BasicTool* _basic = nullptr;
	FluidLib::ToolBase* _active = nullptr;
	std::vector<Button> _surfaces;
	std::vector<Button> _movements;
	std::vector<Button> _actions;
	std::vector<Button> _toolsbuttons;
	std::vector<Button> _selectactions;
	std::string _basicname;
	ImVec2 _iconSize = ImVec2(30, 30);
	
	int _selectedsurface = -1;
	int _selectedmovement = -1;
	std::vector<std::string> _selectedactions;
	int _selectedtool = -1;
};


