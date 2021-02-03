#include "Exporter.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Simulation.h"
#include "../SimulationCore/GridStructures.h"

bool Exporter::Export()
{
	//Select File
	std::string file = FileDialog::Export();
	if(file == "")
		return false;
	int width = FluidLib::Simulation::Get()->GetSizeX();
	int height = FluidLib::Simulation::Get()->GetSizeY();
	uint8_t* pixels = new uint8_t[width * height * CHANNEL_NUM];
	FluidLib::ColorGrid<IInk> * grid = static_cast<FluidLib::ColorGrid<IInk> *>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
	std::vector<glm::vec4> colors = grid->GetColors();
	IInk* inkvals = grid->GetBufferPointer();
	int index = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			glm::vec4 col = colors[inkvals[i * width + j].id];
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
			pixels[index++] = int(255.99 * col.r * scale);
			pixels[index++] = int(255.99 * col.g * scale);
			pixels[index++] = int(255.99 * col.b * scale);
		}
	}
	stbi_write_png(file.c_str(), width, height, CHANNEL_NUM, pixels, width * CHANNEL_NUM);
	grid->ReleaseBufferPointer();
	delete[] pixels;
	return true;
}
