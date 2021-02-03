#include "Exporter.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Simulation.h"
#include "../SimulationCore/GridStructures.h"

bool Exporter::Export(bool askfile)
{
	//Select File
	if(askfile)
		_file = FileDialog::Export();
	if(_file == "")
		return false;
	int width = FluidLib::Simulation::Get()->GetSizeX();
	int height = FluidLib::Simulation::Get()->GetSizeY();
	uint8_t* pixels = new uint8_t[width * height * CHANNEL_NUM];
	FluidLib::ColorGrid<IInk> * grid = static_cast<FluidLib::ColorGrid<IInk> *>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
	std::vector<glm::vec4> colors = grid->GetColors();
	IInk* inkvals = grid->GetBufferPointer();
	int index = 0;
	glm::vec4 watercolor = colors[0];
	float intensity = FluidLib::Simulation::Get()->GetSettings()->intesity;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int colid = inkvals[i * width + j].id;
			glm::vec4 col = colors[colid];
			float freq = inkvals[i * width + j].ink;
			float scale = 0;
			if (freq > 1000)
				scale = 1;
			else if (freq > 300)
				scale = freq / 1000;
			else if (freq > 0)
				scale = 0.3;
			else if (freq == 0)
				scale = 0.1;

			if (colid == 0)
				scale = 1;
			//mix =  v1 * (1 - a) + v2 * a
			pixels[index++] = int(255.99 * (watercolor.r * (1-intensity) + (col.r * scale) * intensity));
			pixels[index++] = int(255.99 * (watercolor.g * (1-intensity) + (col.g * scale) * intensity));
			pixels[index++] = int(255.99 * (watercolor.b * (1-intensity) + (col.b * scale) * intensity));
			if (_watertrans) {
				if (colid == 0)
					pixels[index++] = 0;
				else if (_inktrans)
					pixels[index++] = 255 * scale;
				else
					pixels[index++] = 255;
			}
			else
				pixels[index++] = 255;
		}
	}
	stbi_write_png(_file.c_str(), width, height, CHANNEL_NUM, pixels, width * CHANNEL_NUM);
	grid->ReleaseBufferPointer();
	delete[] pixels;
	return true;
}

void Exporter::SetFile(std::string file)
{
	_file = file;
}
