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
	};

	class GridBase 
	{
	public:

		virtual void UseGrid() = 0;
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
		Grid(GLuint id, int size);

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
		inline int ElementSize() { return sizeof(T); }
		inline void SetAttrPointer(int attr) { _data.attrpointer = attr; }
		inline void GetAttrPointer() { return _data.attrpointer; }

		inline BufferData& GetBufferData() { return _data; }

		void UseGrid();
	private:
		BufferData _data;


	};

	template<class T>
	inline Grid<T>::Grid()
	{

	}

	template<class T>
	inline Grid<T>::Grid(GLuint id, int size) : Grid()
	{
		SetBufferId(id);
		SetSize(size);
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

}