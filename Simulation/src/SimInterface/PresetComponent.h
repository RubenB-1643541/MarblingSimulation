#pragma once

#include "Interface/InterfaceComponent.h"
#include "Tools/ToolManager.h"
#include "Tools/Basic Tools/BasicTool.h"
#include "Tools/Surface/surfaces.h"
#include "Tools/Movement/movements.h"
#include <map>
#include <fstream>
#include "ToolSelectComponent.h"

struct Preset {
	std::string name;
	std::string surface;
	FluidLib::Parameters* surfparams;
	std::string movement;
	FluidLib::Parameters* moveparams;
	std::string action;
};

class PresetComponent : public RenderEngine::InterfaceComponent
{
public:
	PresetComponent(FluidLib::ToolManager* tools, const std::string& basic = "Basic", ToolSelectComponent* tool = nullptr);
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
	void AddPreset(Preset& preset);
	void RemovePreset(Preset& preset);
private:
	void CreatePreset();
	void Load();
	void WriteToFile(Preset& preset);
	void RemoveFromFile(Preset& preset);
	const static int namebuffersize = 128;
	FluidLib::ToolManager* _tools = nullptr;
	FluidLib::BasicTool* _basic = nullptr;
	FluidLib::ToolBase* _active = nullptr;
	std::string _basicname;
	std::map<std::string, Preset> _presets;
	bool _create = false;
	char _namebuffer[namebuffersize] = "";
	const char* _selectedsurface = NULL;
	const char* _selectedmovement = NULL;
	const char* _selectedaction = NULL;

	const std::string presetsfile = "presets.conf";
	ToolSelectComponent* _toolsselect = nullptr;
};


