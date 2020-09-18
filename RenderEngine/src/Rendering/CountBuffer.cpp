#include "pch.h"
#include "CountBuffer.h"

namespace RenderEngine {



	CountBuffer::CountBuffer()
	{
		glGenBuffers(1, &_id);
	}

	CountBuffer::~CountBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	void CountBuffer::Bind(GLenum target)
	{
		glBindBuffer(target, _id);
	}

	void CountBuffer::UnBind(GLenum target)
	{
		glBindBuffer(target, NULL);
	}

	void CountBuffer::BindBufferBase(int index)
	{
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, index, _id);
	}

	void CountBuffer::BindBufferRange(GLuint index, GLintptr offset, GLsizeiptr size)
	{
		glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, index, _id, offset, size);
	}

	void CountBuffer::BufferData(int size)
	{
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	}

	void CountBuffer::BufferSubData(int size, void* data)
	{
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, size, data);
	}

	void* CountBuffer::MapBuffer()
	{
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	}

	void CountBuffer::UnMapBuffer()
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

}