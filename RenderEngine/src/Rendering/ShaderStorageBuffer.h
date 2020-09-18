#pragma once

#include "GL/glew.h"

namespace RenderEngine {

	class ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer();
		void Destroy();

		void Bind();
		void UnBind();
		void Bind(GLenum target);
		void UnBind(GLenum target);
		void BindBufferBase(int index);
		void BufferData(int size);

		void EnableAttributeArray(int attr);
		void VertexPointer(int size, GLenum type, GLsizei stride, const void* pointer);

		void* MapBufferRange();
		void* MapBufferRange(GLint bufMask);
		void UnMapBuffer();
		void* MapBuffer();

		inline GLuint GetId() { return _id; }
	private:
		GLuint _id = -1;
		int _size = -1;


	};

}