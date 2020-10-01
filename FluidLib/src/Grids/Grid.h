#pragma once

#include "GL/glew.h"

namespace FluidLib {

	struct BufferLock {
		bool locked = false;
	};

	static BufferLock lock ;

	static void ReleaseBufferLock() {
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		lock.locked = false;
	}

	struct BufferData 
	{
		GLuint id = -1;
		/*
		Number of elements in the buffer
		*/
		int size = -1;
		int attrpointer = -1;
		int elementsize = 1;
		GLenum type = GL_INT;
	};

	class GridBase 
	{
	public:

		virtual void UseGrid() = 0;
		virtual void DrawGrid() = 0;
	private:


	};
	
	template <class T>
	class Grid : public GridBase
	{
	public:
		Grid();
		/*
		Parameters:
		- GLuint id: ShaderStorageBuffer Id
		- int size: Number of elements in the buffer, not the size of the buffer
		*/
		Grid(GLuint id, int size, bool render = false);
		Grid(GLuint id, int size, int attr, bool render = false);

		T* GetBufferPointer();
		T* GetBufferPointer(GLint bufMask);

		inline void SetBufferId(GLuint id) { _data.id = id; }
		inline int GetBufferId() { return _data.id; }

		/*
		Sets the number of elements in the buffer
		*/
		inline void SetSize(int size) { _data.size = size; }
		/*
		Gives the number of elements in the buffer
		*/
		inline int GetSize() { return _size; }
		
		inline void SetElementSize(int size) { _data.elementsize = size; }
		inline int GetElementSize() { return _data.elementsize; }

		inline void SetElementType(GLenum type) { _data.type = type; }
		inline GLenum GetElementType() { return _data.type; }

		inline int ElementSize() { return sizeof(T); }
		inline void SetAttrPointer(int attr) { _data.attrpointer = attr; }
		inline void GetAttrPointer() { return _data.attrpointer; }

		inline BufferData& GetBufferData() { return _data; }

		void UseGrid();
		void DrawGrid();

		inline void SetRender(bool state) { _render = state; }
		inline bool GetRender() { return _render; }
	private:
		BufferData _data;
		bool _render = false;


	};

	template<class T>
	inline Grid<T>::Grid()
	{

	}

	template<class T>
	inline Grid<T>::Grid(GLuint id, int size, bool render) : Grid()
	{
		SetBufferId(id);
		SetSize(size);
		SetRender(render);
	}

	template<class T>
	inline Grid<T>::Grid(GLuint id, int size, int attr, bool render) : Grid()
	{
		SetBufferId(id);
		SetSize(size);
		SetAttrPointer(attr);
		SetRender(render);
	}

	template<class T>
	inline T* Grid<T>::GetBufferPointer()
	{
		if (lock.locked)
			ReleaseBufferLock();
		lock.locked = true;
		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		return static_cast<T*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _size * sizeof(T), bufMask));
	}

	template<class T>
	inline T* Grid<T>::GetBufferPointer(GLint bufMask)
	{
		if (lock.locked)
			ReleaseBufferLock();
		lock.locked = true;
		return static_cast<T*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _size * sizeof(T), bufMask));
	}

	template<class T>
	inline void Grid<T>::UseGrid()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data.attrpointer, _data.id);
	}

	template<class T>
	inline void Grid<T>::DrawGrid()
	{
		if (_render) {
			glBindBuffer(GL_ARRAY_BUFFER, _data.id);
			glVertexPointer(_data.elementsize, _data.type, 0, (void*)0);
			glEnableVertexAttribArray(_data.attrpointer);
		}
	}

}