#pragma once

#include "Grid.h"

namespace FluidLib {

	template <class T>
	class FlagGrid : public Grid<T>
	{
	public:
		FlagGrid();
		FlagGrid(GLuint id, int size, bool render = false);
		FlagGrid(GLuint id, int size, int attr, bool render = false);
		~FlagGrid();
		T* GetValues();

		void CopyFromGPU();
		void CopyToGPU();

		void SetAllValues(T val) override;
	private:
		T* _values;

	};



	template<class T>
	inline FlagGrid<T>::FlagGrid() : Grid()
	{
	}

	template<class T>
	inline FlagGrid<T>::FlagGrid(GLuint id, int size, bool render) : Grid(id, size, render)
	{
		_values = new T[size];
	}

	template<class T>
	inline FlagGrid<T>::FlagGrid(GLuint id, int size, int attr, bool render) : Grid(id, size, attr, render)
	{
		_values = new T[size];
	}

	template<class T>
	inline FlagGrid<T>::~FlagGrid()
	{
		delete [] _values;
	}

	template<class T>
	inline T* FlagGrid<T>::GetValues()
	{
		return _values;
	}

	template<class T>
	inline void FlagGrid<T>::CopyFromGPU()
	{
		T* data = GetBufferPointer();
		memcpy(_values, data, _data.size * sizeof(T));
		ReleaseBufferPointer();
	}

	template<class T>
	inline void FlagGrid<T>::CopyToGPU()
	{
		glNamedBufferSubData(_data.id, 0, _data.size * sizeof(T), _values);
	}

	template<class T>
	inline void FlagGrid<T>::SetAllValues(T val)
	{
		T* data = GetBufferPointer();
		T* data2 = GetValues();
		for (int i = 0; i < _data.size; ++i) {
			data[i] = val;
			data2[i] = val;
		}
		ReleaseBufferPointer();
	}

}