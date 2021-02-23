#pragma once

#include "../Action.h"
#include "Util/Clipboard.h"

namespace FluidLib {

	template <class T>
	class CopyAction : public Action<T>
	{
	public:
		CopyAction(Grid<T>* grid);

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Execute(IPoint& p) override;
		virtual void Execute(std::vector<IPoint>& points) override;
		void SetDim(float * width, float * height) { _width = width; _height = height; }
	private:
		float * _width;
		float * _height;

	};

	template<class T>
	inline CopyAction<T>::CopyAction(Grid<T>* grid)
	{
		SetBasicCheckFunction();
		SetGrid(grid);
	}

	template<class T>
	inline void CopyAction<T>::Start()
	{
		_gridvals = _grid->GetBufferPointer();
		Clipboard::Reset(*_width, *_height, sizeof(T));
		//Setup copy data
	}

	template<class T>
	inline void CopyAction<T>::Stop()
	{
		_grid->ReleaseBufferPointer();
		_gridvals = nullptr;
		//Copy data to clipboard
	}

	template<class T>
	inline void CopyAction<T>::Execute(IPoint& p)
	{
		IPoint temp = _pos + p;
		T* data = static_cast<T*>(Clipboard::GetData());
		if (IN_GRID(temp))
			data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX() ] = _gridvals[POINT_TO_1D(temp)];
	}

	template<class T>
	inline void CopyAction<T>::Execute(std::vector<IPoint>& points)
	{
	}

}