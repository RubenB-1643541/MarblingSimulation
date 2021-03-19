#pragma once

#include <gl/glew.h>

namespace FluidLib {

	struct TextureData {
		int width = -1;
		int height = -1;
		int channels = -1;
		unsigned char* data;
	};


	class Texture
	{
	public:
		Texture(TextureData& data);

		void Bind();
	private:
		GLuint _id = -1;
	};

}