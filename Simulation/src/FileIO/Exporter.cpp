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
			float sum = inkvals[i * width + j].ink;
			int n = 1;
			for (int x = -5; x <= 5; ++x) {
				for (int y = -5; y <= 5; ++y) {
					if ((j + x) >= 0 && (i + y) >= 0 && (j + x) < width && (i + y) < height) {
						if (inkvals[(i + y) * width + j + x].id != 0) {
							sum += inkvals[(i + y) * width + j + x].ink;
							col += colors[inkvals[(i + y) * width + j + x].id];
						}
						n += 1;
					}
				}
			}
			freq = sum / n;
			col = glm::vec4(col.r / n, col.g / n, col.b / n, 1);
			float scale = 0;
			if (freq > 1000)
				scale = 1;
			else
				scale = freq / 1000;
			//else if (freq > 0)
			//	scale = 0.3;
			//else if (freq == 0)
			//	scale = 0.1;

			if (colid == 0) {
				scale = 1;
				col = colors[colid];
			}
			//mix =  v1 * (1 - a) + v2 * a
			pixels[index++] = int(255.99 * (watercolor.r * (1-(scale * intensity)) + col.r * scale * intensity));
			pixels[index++] = int(255.99 * (watercolor.g * (1-(scale * intensity)) + col.g * scale * intensity));
			pixels[index++] = int(255.99 * (watercolor.b * (1-(scale * intensity)) + col.b * scale * intensity));
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

void Exporter::SetFile(const std::string& file)
{
	_file = file;
}
