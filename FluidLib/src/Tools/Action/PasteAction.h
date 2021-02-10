#pragma once

#include "../Action.h"
#include "Util/Clipboard.h"

namespace FluidLib {

	template <class T>
	class PasteAction : public Action<T>
	{
	public:
		PasteAction(Grid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE);

		virtual void Start() override;
		virtual void Stop() override;
		//virtual void Execute(IPoint& p) override;
		//virtual void Execute(std::vector<IPoint>& points) override;
		void SetPos(float* x, float* y) { _x = x; _y = y; }

		void Set(IPoint& p) override;
		void Add(IPoint& p) override;
		void Sub(IPoint& p) override;
		void Mul(IPoint& p) override;
		void Div(IPoint& p) override;
		void Move(IPoint& p) override;
		void Mul(float f, IPoint& p) override;
		void Div(float f, IPoint& p) override;
	private:
		float* _x;
		float* _y;

	};
	template<class T>
	inline PasteAction<T>::PasteAction(Grid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE)
	{
		SetGrid(grid);
		SetOperation(op);
	}
	template<class T>
	inline void PasteAction<T>::Start()
	{
		_gridvals = _grid->GetBufferPointer();
	}
	template<class T>
	inline void PasteAction<T>::Stop()
	{
		_grid->ReleaseBufferPointer();
		_gridvals = nullptr;
	}
	template<class T>
	inline void PasteAction<T>::Set(IPoint& p)
	{
		IPoint temp = IPoint(*_x, *_y) + p;
		if (IN_GRID(temp)) {
			T* data = static_cast<T*>(Clipboard::GetData());
			_gridvals[POINT_TO_1D(temp)] = data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
		}
	}
	template<class T>
	inline void PasteAction<T>::Add(IPoint& p)
	{
		IPoint temp = IPoint(*_x, *_y) + p;
		if (IN_GRID(temp)) {
			T* data = static_cast<T*>(Clipboard::GetData());
			_gridvals[POINT_TO_1D(temp)] += data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
		}
	}
	template<class T>
	inline void PasteAction<T>::Sub(IPoint& p)
	{
		IPoint temp = IPoint(*_x, *_y) + p;
		if (IN_GRID(temp)) {
			T* data = static_cast<T*>(Clipboard::GetData());
			_gridvals[POINT_TO_1D(temp)] -= data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
		}
	}
	template<class T>
	inline void PasteAction<T>::Mul(IPoint& p)
	{
		IPoint temp = IPoint(*_x, *_y) + p;
		if (IN_GRID(temp)) {
			T* data = static_cast<T*>(Clipboard::GetData());
			_gridvals[POINT_TO_1D(temp)] *= data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
		}
	}
	template<class T>
	inline void PasteAction<T>::Div(IPoint& p)
	{
		IPoint temp = IPoint(*_x, *_y) + p;
		if (IN_GRID(temp)) {
			T* data = static_cast<T*>(Clipboard::GetData());
			_gridvals[POINT_TO_1D(temp)] /= data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
		}
	}
	template<class T>
	inline void PasteAction<T>::Move(IPoint& p)
	{
		//IPoint temp = IPoint(*_x, *_y) + p;
		//T* data = static_cast<T*>(Clipboard::GetData());
		//_gridvals[POINT_TO_1D(temp)] += data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
	}
	template<class T>
	inline void PasteAction<T>::Mul(float f, IPoint& p)
	{
		//IPoint temp = IPoint(*_x, *_y) + p;
		//T* data = static_cast<T*>(Clipboard::GetData());
		//_gridvals[POINT_TO_1D(temp)] += data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
	}	
	template<class T>
	inline void PasteAction<T>::Div(float f, IPoint& p)
	{
		//IPoint temp = IPoint(*_x, *_y) + p;
		//T* data = static_cast<T*>(Clipboard::GetData());
		//_gridvals[POINT_TO_1D(temp)] += data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
	}
	//template<class T>
	//inline void PasteAction<T>::Execute(IPoint& p)
	//{
	//	IPoint temp = IPoint(*_x, *_y) + p;
	//	T* data = static_cast<T*>(Clipboard::GetData());
	//	_gridvals[POINT_TO_1D(temp)] += data[p.GetY() * Clipboard::GetDataStruct()->width + p.GetX()];
	//}
	//template<class T>
	//inline void PasteAction<T>::Execute(std::vector<IPoint>& points)
	//{
	//}
}