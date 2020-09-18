#include "pch.h"
#include "VertexBuffer.h"

namespace RenderEngine {

    VertexBuffer::VertexBuffer()
    {
        glCreateBuffers(1, &_id);
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    void VertexBuffer::Destroy()
    {
        glDeleteBuffers(1, &_id);
    }

    void VertexBuffer::BufferData(std::vector<float> vertices)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    }

    void VertexBuffer::BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
    {
        glBufferData(target, size, data, usage);
    }

    void VertexBuffer::EnableAttributeArray(int attr)
    {
        glEnableVertexAttribArray(attr);
    }

    void VertexBuffer::DisableAttributeArray(int attr)
    {
        glDisableVertexAttribArray(attr);
    }

    void VertexBuffer::AttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset)
    {
        glVertexAttribPointer(index, size, type, normalized, stride, offset);
    }

    void VertexBuffer::AttributePointer(GLuint index, GLint size, const void* offset)
    {
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, offset);
    }

    void VertexBuffer::AttributePointer(GLint size, const void* offset)
    {
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, offset);
    }

    void VertexBuffer::AttributePointer(GLint size)
    {
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void VertexBuffer::BufferSubData(std::vector<float> vertices)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), &vertices[0]);
    }

    void VertexBuffer::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
    {
        glBufferSubData(target, offset, size, data);
    }

    void VertexBuffer::NamedBufferSubData(std::vector<float> vertices)
    {
        glNamedBufferSubData(_id, 0, vertices.size(), &vertices[0]);
    }

    void VertexBuffer::NamedBufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
    {
        glNamedBufferSubData(_id, offset, size, data);
    }

    void* VertexBuffer::MapBuffer()
    {
        return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void VertexBuffer::UnMapBuffer()
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

}