#pragma once

#include "FileDialog.h"
#include <string>
#include "Grids/ColorGrid.h"
#include "../SimUtils/Icon.h"
#include "../SimulationCore/GridStructures.h"
#include "Util/Point.h"
#include "Simulation.h"

#define CHANNEL_NUM 4

class Importer
{
public:
	bool Import(bool askfile = true);
	void SetFile(const std::string& file);
	Image GetImportedImage();
	void ImportInGrid(FluidLib::ColorGrid<IInk>* grid);
private:
	std::string _file;
	Image _im;

};

