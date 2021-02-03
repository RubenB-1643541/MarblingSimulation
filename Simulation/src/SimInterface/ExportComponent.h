#pragma once

#include "Interface/InterfaceComponent.h"
#include "imgui/imgui.h"
#include "Simulation.h"
#include "../FileIO/Exporter.h"

class ExportComponent : public RenderEngine::InterfaceComponent
{
public:
	ExportComponent();
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
	inline void Open() { _opened = true; _oldfps = _settings->fps;}
	inline void Close() { _opened = false; }
private:
	int _oldfps = 0;
	bool _opened = false;
	int _width = 1000;
	int _height = 500;
	char _file[128] = "";
	Exporter _ex;
	FluidLib::Settings* _settings;

	bool _wattrans = false;
	bool _inktrans = false;


};

