#include "Icon.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Icon Load(const std::string& name) {
    int image_width, image_height;
    unsigned char* image_data = stbi_load(name.c_str(), &image_width, &image_height, NULL, 4);

    // Turn the RGBA pixel data into an OpenGL texture:
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    return { image_width, image_height, texture };
}

Image LoadPng(const std::string& image)
{
    Image im;
    im.data = stbi_load(image.c_str(), &im.width, &im.height, NULL, 4);
    return im;
}

GLFWimage * LoadGLFWimage(const std::string& png)
{
    GLFWimage icon[1];
    icon->pixels = stbi_load("res/icons/Icon.png", &icon->width, &icon->height, NULL, 4);
    return icon;
}

void FreeImage(Image i)
{
    stbi_image_free(i.data);
}

Image ImageFromClipboard(std::vector<glm::vec4>& colors, float intensity)
{
    FluidLib::ClipboardData* dat = FluidLib::Clipboard::GetDataStruct();
    Image im;
    im.width = dat->width;
    im.height = dat->height;
	im.data = new uint8_t[im.width* im.height* 4];
    IInk* inkvals = static_cast<IInk*>(FluidLib::Clipboard::GetData());
	int index = 0;
	for (int i = 0; i < im.height; ++i) {
		for (int j = 0; j < im.width; ++j) {
			int colid = inkvals[i * im.width + j].id;
			glm::vec4 col = colors[colid];
			float freq = inkvals[i * im.width + j].ink;
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
			im.data[index++] = int(255.99 *  (col.r * scale) * intensity);
			im.data[index++] = int(255.99 *  (col.g * scale) * intensity);
			im.data[index++] = int(255.99 *  (col.b * scale) * intensity);
			if(colid == 0)
				im.data[index++] = 0;
			else
				im.data[index++] = 255;
		}
	}
    return im;
}
