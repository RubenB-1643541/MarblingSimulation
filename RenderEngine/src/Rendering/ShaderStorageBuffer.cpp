#include "pch.h"
#include "ShaderStorageBuffer.h"

namespace RenderEngine {

	ShaderStorageBuffer::ShaderStorageBuffer()
	{
		glGenBuffers(1, &_id);
	}

	void ShaderStorageBuffer::Destroy()
	{
		if (_id != -1)
			glDeleteBuffers(1, &_id);
	}

	void ShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);
	}

	void ShaderStorageBuffer::UnBind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	}

	void ShaderStorageBuffer::Bind(GLenum target)
	{
		glBindBuffer(target, _id);
	}

	void ShaderStorageBuffer::UnBind(GLenum target)
	{
		glBindBuffer(target, NULL);
	}

	void ShaderStorageBuffer::BindBufferBase(int index)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, _id);
	}

	void ShaderStorageBuffer::BufferData(int size)
	{
		_size = size;
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
	}

	void ShaderStorageBuffer::EnableAttributeArray(int attr)
	{
		glEnableVertexAttribArray(attr);
	}

	void ShaderStorageBuffer::VertexPointer(int size, GLenum type, GLsizei stride, const void* pointer)
	{
		glVertexPointer(size, type, stride, pointer);
	}

	void* ShaderStorageBuffer::MapBufferRange()
	{
		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		return glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _size, bufMask);
	}

	void* ShaderStorageBuffer::MapBufferRange(GLint bufMask)
	{
		return glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _size, bufMask);
	}

	void ShaderStorageBuffer::UnMapBuffer()
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void* ShaderStorageBuffer::MapBuffer()
	{
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	}

}