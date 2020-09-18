#pragma once

#include "GL/glew.h"

namespace RenderEngine {

	class IndexBuffer
	{
	public:
		IndexBuffer();

		void Bind();
		void Unbind();
		void Destroy();

		void BufferData(std::vector<unsigned int> indices);
		void BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
		
		inline GLuint GetId() { return _id; }
	private:
		GLuint _id = -1;


	};

}