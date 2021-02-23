#pragma once

#include "../Action.h"
#include "Grids/FlagGrid.h"

namespace FluidLib {

	template <class T>
	class FlagAction : public Action<T>
	{
	public:
		inline FlagAction();
		inline FlagAction(T value, ACTION_OPERATION op = ACTION_OPERATION::NONE);
		inline FlagAction(FlagGrid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE);
		inline FlagAction(T value, FlagGrid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE);

		void Set(IPoint& p) override;
		void Add(IPoint& p) override;
		void Sub(IPoint& p) override;
		void Mul(IPoint& p) override;
		void Div(IPoint& p) override;
		void Move(IPoint& p) override;
		void Mul(float f, IPoint& p) override;
		void Div(float f, IPoint& p) override;
	private:
		FlagGrid<T>* _flagGrid = nullptr;

	};

	template<class T>
	inline FlagAction<T>::FlagAction()
	{
		SetBasicCheckFunction();
	}
	template<class T>
	inline FlagAction<T>::FlagAction(T value, ACTION_OPERATION op)
	{
		SetBasicCheckFunction();
		SetValue(value);
		SetOperation(op);
	}
	template<class T>
	inline FlagAction<T>::FlagAction(FlagGrid<T>* grid, ACTION_OPERATION op)
	{
		SetBasicCheckFunction();
		SetGrid(grid);
		_flagGrid = grid;
	}
	template<class T>
	inline FlagAction<T>::FlagAction(T value, FlagGrid<T>* grid, ACTION_OPERATION op)
	{
		SetBasicCheckFunction();
		SetValue(value);
		SetGrid(grid);
		SetOperation(op);
		_flagGrid = grid;
	}
	template<class T>
	inline void FlagAction<T>::Set(IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] = _value * _scale;
		_flagGrid->GetValues()[i] = _value * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Add(IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] += _value * _scale;
		_flagGrid->GetValues()[i] += _value * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Sub(IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] -= _value * _scale;
		_flagGrid->GetValues()[i] -= _value * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Mul(IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] *= _value * _scale;
		_flagGrid->GetValues()[i] *= _value * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Div(IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] /= _value * _scale;
		_flagGrid->GetValues()[i] /= _value * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Move(IPoint& p)
	{
		IPoint move = _pos - _oldpos;
		int i = POINT_TO_1D(p);
		_gridvals[i] += move * 20 * _scale;
		_flagGrid->GetValues()[i] += move * 20 * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Mul(float f, IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] *= f * _scale;
		_flagGrid->GetValues()[i] *= f * _scale;
	}
	template<class T>
	inline void FlagAction<T>::Div(float f, IPoint& p)
	{
		int i = POINT_TO_1D(p);
		_gridvals[i] /= f * _scale;
		_flagGrid->GetValues()[i] /= f * _scale;
	}
}