#include "Importer.h"

bool Importer::Import(bool askfile)
{
	//Select File
	if (askfile)
		_file = FileDialog::Import();
	if (_file == "")
		return false;
	if (_im.data != nullptr)
		FreeImage(_im);
	_im = LoadPng(_file);
	return true;
}

void Importer::SetFile(const std::string& file)
{
	_file = file;
}

Image Importer::GetImportedImage()
{
	return _im;
}

void Importer::ImportInGrid(FluidLib::ColorGrid<IInk>* grid)
{
	IInk* inkvals = grid->GetBufferPointer();
	std::vector<glm::vec4> colors = grid->GetColors();
	int index = 0;
	int colid = 0;
	int maxcolid = 0;
	for (int i = 0; i < _im.height; ++i) {
		for (int j = 0; j < _im.width; ++j) {
			glm::vec4 col;
			col.r = float(_im.data[index++])/255.99;
			col.g = float(_im.data[index++])/255.99;
			col.b = float(_im.data[index++])/255.99;
			_im.data[index++];
			std::vector<glm::vec4>::iterator it = std::find(colors.begin(), colors.end(), col);
			if (it == colors.end()) {
				colors.push_back(col);
				grid->AddColor(col);
				++maxcolid;
				colid = maxcolid;
			}
			else {
				colid = std::distance(colors.begin(), it);
			}
			inkvals[COOR_2D_TO_1D(j,i)].id = colid;
			if(colid != 0)
				inkvals[COOR_2D_TO_1D(j, i)].ink = 1000;
			else
				inkvals[COOR_2D_TO_1D(j, i)].ink = 0;
		}
	}
	grid->SetId(grid->GetColors().size() - 1);
	grid->ReleaseBufferPointer();
}
