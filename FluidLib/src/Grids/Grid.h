#pragma once

#include "GL/glew.h"
#include <fstream>

#define COOR_2D_TO_1D(x,y) Simulation::Get()->GetSizeX() * y + x
#define POINT_TO_1D(p) Simulation::Get()->GetSizeX() * p.GetY() + p.GetX()
#define IN_GRID(p) p.GetX()>=0 && p.GetY()>=0 && p.GetX() < Simulation::Get()->GetSizeX() &&  p.GetY() < Simulation::Get()->GetSizeY()

namespace FluidLib {

	struct BufferLock {
		bool locked = false;
		GLuint id;
		int attrpointer;
	};

	static BufferLock lock ;

	static void ReleaseBufferLock() {
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
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
		BufferData& GetBufferData() { return _data; }
		virtual void WriteToFile(std::ofstream& stream) = 0;
	protected:
		BufferData _data;
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
		void ReleaseBufferPointer();

		inline void SetBufferId(GLuint id) { _data.id = id; }
		inline int GetBufferId() { return _data.id; }

		/*
		Sets the number of elements in the buffer
		*/
		inline void SetSize(int size) { _data.size = size; }
		/*
		Gives the number of elements in the buffer
		*/
		inline int GetSize() { return _data.size; }
		
		inline void SetElementSize(int size) { _data.elementsize = size; }
		inline int GetElementSize() { return _data.elementsize; }

		inline void SetElementType(GLenum type) { _data.type = type; }
		inline GLenum GetElementType() { return _data.type; }

		inline int ElementSize() { return sizeof(T); }
		inline void SetAttrPointer(int attr) { _data.attrpointer = attr; }
		inline void GetAttrPointer() { return _data.attrpointer; }

		virtual void UseGrid();
		virtual void DrawGrid();

		inline void SetRender(bool state) { _render = state; }
		inline bool GetRender() { return _render; }

		virtual void WriteToFile(std::ofstream& stream) override;
	private:
		
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
		//if (lock.locked)
		//	ReleaseBufferLock();
		lock.locked = true;
		lock.id = _data.id;
		lock.attrpointer = _data.attrpointer;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data.id);
		GLint bufMask = GL_MAP_WRITE_BIT;
		return static_cast<T*>(glMapNamedBufferRange(_data.id, 0, _data.size * sizeof(T), bufMask));
	}

	template<class T>
	inline T* Grid<T>::GetBufferPointer(GLint bufMask)
	{
		if (lock.locked)
			ReleaseBufferLock();
		lock.locked = true;
		lock.id = _data.id;
		lock.attrpointer = _data.attrpointer;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data.id);
		return static_cast<T*>(glMapNamedBufferRange(_data.id, 0, _data.size * sizeof(T), bufMask));
	}

	template<class T>
	inline void Grid<T>::ReleaseBufferPointer()
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
		lock.locked = false;
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
			glVertexAttribIPointer(0, _data.elementsize, _data.type, 0, (void*)0);
			//glVertexAttributePointer(_data.elementsize, _data.type, 0, (void*)0);
			glEnableVertexAttribArray(_data.attrpointer);
		}
	}

	template<class T>
	inline void Grid<T>::WriteToFile(std::ofstream& stream)
	{
		T* data = GetBufferPointer();
		stream.write((char*)data, sizeof(T) * _data.size);
		//for (int i = 0; i < _data.size; ++i) {
		//	stream << data[i] << " ";
		//}
		ReleaseBufferLock();
	}

}