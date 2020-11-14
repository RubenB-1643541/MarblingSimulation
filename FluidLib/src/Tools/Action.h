#pragma once

#include "../Grids/Grid.h"
#include "Util/Point.h"
#include <vector>

namespace FluidLib {

	/* Actions

	Action	->	Change Value in Grid
			->	Add Value in Grid
			->	Subtract Value in Grid
			->	Divide Value in Grid
			->	Mult Value in Grid
	*/

	enum class ACTION_OPERATION {NONE, SET, ADD, SUB, MUL, DIV, MOVE};

	class ActionBase
	{
	public:

		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Execute(IPoint& p) = 0;
		virtual void Execute(std::vector<IPoint>& points) = 0;

		virtual void Set(IPoint& p) = 0;
		virtual void Add(IPoint& p) = 0;
		virtual void Sub(IPoint& p) = 0;
		virtual void Mul(IPoint& p) = 0;
		virtual void Div(IPoint& p) = 0;
		virtual void Move(IPoint& p) = 0;
		virtual void Mul(float f, IPoint& p) = 0;
		virtual void Div(float f, IPoint& p) = 0;
		virtual void SetPos(IPoint p) { _oldpos = _pos; _pos = p; }
		virtual float* GetScale() = 0;
		virtual const char* GetType() = 0;
	protected:
		IPoint _pos = { 0,0 };
		IPoint _oldpos = {-1,-1};
	private:

	};

	template <class T>
	class Action : public ActionBase
	{
	public:
		inline Action(T value, ACTION_OPERATION op = ACTION_OPERATION::NONE) { SetValue(value); SetOperation(op); }
		inline Action(Grid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE) { SetGrid(grid); SetOperation(op); }
		inline Action(T value, Grid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE) { SetValue(value); SetGrid(grid); SetOperation(op); }
		
		inline void SetOperation(ACTION_OPERATION op) { _operation = op; }
		inline void SetValue(T value) { _value = value; }
		inline T& GetValue() { return _value; }
		const char* GetType() { return typeid(T).name(); }
		inline void SetGrid(Grid<T>* grid) { _grid = grid; }

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Execute(IPoint& p) override;
		virtual void Execute(std::vector<IPoint>& points) override;

		void Set(IPoint& p) override;
		void Add(IPoint& p) override;
		void Sub(IPoint& p) override;
		void Mul(IPoint& p) override;
		void Div(IPoint& p) override;
		void Move(IPoint& p) override;
		void Mul(float f, IPoint& p) override;
		void Div(float f, IPoint& p) override;
		float* GetScale() override;
	private:
		T _value;
		Grid<T>* _grid;
		float _scale = 1.0f;
		T* _gridvals = nullptr;
		ACTION_OPERATION _operation = ACTION_OPERATION::NONE;
		
	};

	template<class T>
	inline void Action<T>::Start()
	{
		_gridvals = _grid->GetBufferPointer();
	}

	template<class T>
	inline void Action<T>::Stop()
	{
		_grid->ReleaseBufferPointer();
		_gridvals = nullptr;
	}

	template<class T>
	inline void Action<T>::Execute(IPoint& p)
	{
		switch (_operation)
		{
		case ACTION_OPERATION::SET:
			Set(p);
			break;
		case ACTION_OPERATION::ADD: 
			Add(p);
			break;
		case ACTION_OPERATION::SUB:
			Sub(p);
			break;
		case ACTION_OPERATION::MUL:
			Mul(p);
			break;
		case ACTION_OPERATION::DIV:
			Div(p);
			break;
		case ACTION_OPERATION::MOVE:
			Move(p);
			break;
		default:
			break;
		}
	}

	template<class T>
	inline void Action<T>::Execute(std::vector<IPoint>& points)
	{
	}

	template<class T>
	inline void Action<T>::Set(IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] = _value * _scale;
	}

	template<class T>
	inline void Action<T>::Add(IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] += _value * _scale;
	}

	template<class T>
	inline void Action<T>::Sub(IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] -= _value * _scale;
	}

	template<class T>
	inline void Action<T>::Mul(IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] *= _value * _scale;
	}

	template<class T>
	inline void Action<T>::Div(IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] /= _value * _scale;
	}

	template<class T>
	inline void Action<T>::Move(IPoint& p)
	{
		IPoint temp = _oldpos + p;
		IPoint move = _pos - _oldpos;
		_gridvals[POINT_TO_1D(temp)] += move * 20 *_scale;
	}

	template<class T>
	inline void Action<T>::Mul(float f, IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] *= f * _scale;
	}

	template<class T>
	inline void Action<T>::Div(float f, IPoint& p)
	{
		IPoint temp = _pos + p;
		_gridvals[POINT_TO_1D(temp)] /= f * _scale;
	}

	template<class T>
	inline float* Action<T>::GetScale()
	{
		return &_scale;
	}

}