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
	std::vector<glm::vec3> colors = grid->GetColors();
	IInk* inkvals = grid->GetBufferPointer();
	int index = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			//TODO Fix export
			glm::vec3 col = colors[inkvals[i * width + j].id];
			float freq = inkvals[i * width + j].ink;
			pixels[index++] = int(255.99 * col.r * freq / 1000);
			pixels[index++] = int(255.99 * col.g * freq / 1000);
			pixels[index++] = int(255.99 * col.b * freq / 1000);
		}
	}
	stbi_write_png(file.c_str(), width, height, CHANNEL_NUM, pixels, width * CHANNEL_NUM);
	grid->ReleaseBufferPointer();
	delete pixels;
	return true;
}
