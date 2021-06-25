#pragma once

#include "Grid.h"

namespace FluidLib {

	template <class T>
	class DoubleGrid : public Grid<T>
	{
	public:
		/*
			Parameters:
			- GLuint id: ShaderStorageBuffer Id
			- int size: Number of elements in the buffer, not the size of the buffer
			*/
		DoubleGrid(GLuint id, GLuint id2, int size, bool render = false);
		DoubleGrid(GLuint id, GLuint id2, int size, int attr1, int attr2, bool render = false);

		virtual T* GetBufferPointer() override;
		virtual T* GetBufferPointer(GLint bufMask) override;
		virtual void ReleaseBufferPointer();

		inline virtual void SetBufferId(GLuint id) override { _data.id = id; }
		inline virtual void SetBuffer2Id(GLuint id) { _data2.id = id; }
		inline virtual int GetBufferId() override { return _data.id; }
		inline virtual int GetBuffer2Id() { return _data2.id; }

		/*
		Sets the number of elements in the buffer
		*/
		inline virtual void SetSize(int size) override { _data.size = size; _data2.size = size; }
		/*
		Gives the number of elements in the buffer
		*/
		inline virtual int GetSize() override { return _data.size; }

		inline virtual void SetElementSize(int size) override { _data.elementsize = size; _data2.elementsize = size; }
		inline virtual int GetElementSize() override { return _data.elementsize; }

		inline virtual void SetElementType(GLenum type) override { _data.type = type; _data2.type = type; }
		inline virtual GLenum GetElementType() override { return _data.type; }

		inline virtual void SetAttrPointer(int attr) override { _data.attrpointer = attr; }
		inline virtual void SetAttr2Pointer(int attr) { _data2.attrpointer = attr; }
		inline virtual int GetAttrPointer() override { return _data.attrpointer; }
		inline virtual int GetAttr2Pointer() { return _data2.attrpointer; }

		virtual void UseGrid() override;
		virtual void DrawGrid() override;

		void SwitchBuffer();
		void CopyFirstToSecond();
		void CopySecondToFirst();

	protected:
		BufferData _data2;
		bool _second = false; //Wich buffer to use.

	};

	template<class T>
	inline DoubleGrid<T>::DoubleGrid(GLuint id, GLuint id2, int size, bool render)
	{
		SetBufferId(id);
		SetSize(size);
		SetBuffer2Id(id2);
		SetRender(render);
	}

	template<class T>
	inline DoubleGrid<T>::DoubleGrid(GLuint id, GLuint id2, int size, int attr1, int attr2, bool render)
	{
		SetBufferId(id);
		SetSize(size);
		SetAttrPointer(attr1);
		SetBuffer2Id(id2);
		SetAttr2Pointer(attr2);
		SetRender(render);
	}

	template<class T>
	inline T* DoubleGrid<T>::GetBufferPointer()
	{
		if (_second) {
			lock.locked = true;
			lock.id = _data2.id;
			lock.attrpointer = _data2.attrpointer;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data2.id);
			GLint bufMask = GL_MAP_WRITE_BIT;
			return static_cast<T*>(glMapNamedBufferRange(_data2.id, 0, _data2.size * sizeof(T), bufMask));
		}
		else {
			lock.locked = true;
			lock.id = _data.id;
			lock.attrpointer = _data.attrpointer;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data.id);
			GLint bufMask = GL_MAP_WRITE_BIT;
			return static_cast<T*>(glMapNamedBufferRange(_data.id, 0, _data.size * sizeof(T), bufMask));
		}
	}

	template<class T>
	inline T* DoubleGrid<T>::GetBufferPointer(GLint bufMask)
	{
		if (_second) {
			if (lock.locked)
				ReleaseBufferLock();
			lock.locked = true;
			lock.id = _data2.id;
			lock.attrpointer = _data2.attrpointer;
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data2.id);
			return static_cast<T*>(glMapNamedBufferRange(_data2.id, 0, _data2.size * sizeof(T), bufMask));
		}
		else {
			if (lock.locked)
				ReleaseBufferLock();
			lock.locked = true;
			lock.id = _data.id;
			lock.attrpointer = _data.attrpointer;
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _data.id);
			return static_cast<T*>(glMapNamedBufferRange(_data.id, 0, _data.size * sizeof(T), bufMask));
		}
	}

	template<class T>
	inline void DoubleGrid<T>::ReleaseBufferPointer()
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
		lock.locked = false;
	}

	template<class T>
	inline void DoubleGrid<T>::UseGrid()
	{
		if (_second) {
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data2.attrpointer, _data.id);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data.attrpointer, _data2.id);
		}
		else {
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data.attrpointer, _data.id);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data2.attrpointer, _data2.id);
		}
		SwitchBuffer();
	}

	template<class T>
	inline void DoubleGrid<T>::DrawGrid()
	{
		if (GetRender()) {
			if (_second) {
				glBindBuffer(GL_ARRAY_BUFFER, _data2.id);
				glVertexAttribIPointer(0, _data2.elementsize, _data2.type, 0, (void*)0);
				//glVertexAttributePointer(_data.elementsize, _data.type, 0, (void*)0);
				glEnableVertexAttribArray(_data2.attrpointer);
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, _data.id);
				glVertexAttribIPointer(0, _data.elementsize, _data.type, 0, (void*)0);
				//glVertexAttributePointer(_data.elementsize, _data.type, 0, (void*)0);
				glEnableVertexAttribArray(_data.attrpointer);
			}
		}
	}

	template<class T>
	inline void DoubleGrid<T>::SwitchBuffer()
	{
		_second = !_second;
	}

	template<class T>
	inline void DoubleGrid<T>::CopyFirstToSecond()
	{
		if(_second)
			glCopyNamedBufferSubData(_data2.id, _data.id, NULL, NULL, _data.size * sizeof(T));
		else
			glCopyNamedBufferSubData(_data.id, _data2.id, NULL, NULL, _data.size * sizeof(T));
	}

	template<class T>
	inline void DoubleGrid<T>::CopySecondToFirst()
	{
		if (_second)
			glCopyNamedBufferSubData(_data.id, _data2.id, NULL, NULL, _data.size * sizeof(T));
		else
			glCopyNamedBufferSubData(_data2.id, _data.id, NULL, NULL, _data.size * sizeof(T));
	}

}