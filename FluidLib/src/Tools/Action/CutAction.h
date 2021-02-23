#pragma once

#include "../Action.h"
#include "Util/Clipboard.h"

namespace FluidLib {

	template <class T>
	class CutAction : public Action<T>
	{
	public:
		CutAction(Grid<T>* grid, T value);

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Execute(IPoint& p) override;
		virtual void Execute(std::vector<IPoint>& points) override;
		void SetDim(float* width, float* height) { _width = width; _height = height; }
	private:
		float* _width;
		float* _height;

	};
	template<class T>
	inline CutAction<T>::CutAction(Grid<T>* grid, T value)
	{
		SetBasicCheckFunction();
		SetGrid(grid);
		SetValue(value);
	}
	template<class T>
	inline void CutAction<T>::Start()
	{
		_gridvals = _grid->GetBufferPointer();
		Clipboard::Reset(*_width, *_height, sizeof(T));
	}
	template<class T>
	inline void CutAction<T>::Stop()
	{
		_grid->ReleaseBufferPointer();
		_gridvals = nullptr;
	}
	template<class T>
	inline void CutAction<T>::Execute(IPoint& p)
	{
		IPoint temp = _pos + p;
		T* data = static_cast<T*>(Clipboard::GetData());
		if (IN_GRID(temp)) {
			data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()] = _gridvals[POINT_TO_1D(temp)];
			_gridvals[POINT_TO_1D(temp)] = _value;
		}
	}
	template<class T>
	inline void CutAction<T>::Execute(std::vector<IPoint>& points)
	{
	}
}