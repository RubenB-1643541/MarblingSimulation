#pragma once

#include "GL/glew.h"

namespace RenderEngine {

	class CountBuffer
	{
	public:
		CountBuffer();
		~CountBuffer();
		void Bind(GLenum target);
		void UnBind(GLenum target);
		void BindBufferBase(int index);
		void BindBufferRange(GLuint index, GLintptr offset, GLsizeiptr size);

		void BufferData(int size);
		void BufferSubData(int size, void* data);

		void* MapBuffer();
		void UnMapBuffer();
	private:
		GLuint _id;
	};

}

