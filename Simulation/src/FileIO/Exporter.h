#pragma once

#include "FileDialog.h"
#include <string>
#include "Grids/ColorGrid.h"

#define CHANNEL_NUM 4

class Exporter
{
public:
	bool Export(bool askfile = true);
	void SetFile(std::string file);
	void SetWaterTrans(bool trans) { _watertrans = trans; }
	void SetInkTrans(bool trans) { _inktrans = trans; }
private:
	std::string _file;
	//Options
	bool _watertrans = false;
	bool _inktrans = false;
};

