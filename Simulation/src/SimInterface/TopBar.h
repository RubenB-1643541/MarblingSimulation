#pragma once

#include "Simulation.h"
#include "Interface/InterfaceComponent.h"
#include "../FileIO/FileDialog.h"
#include "../FileIO/Exporter.h"
#include "ExportComponent.h"

class TopBar : public RenderEngine::InterfaceComponent
{
public:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void EndDraw() override;
	inline void SetExportComponent(ExportComponent* exp) { _export = exp; }
private:
	FluidLib::Settings* _settings;
	ExportComponent* _export = nullptr;
};

