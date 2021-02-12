#pragma once

#include "../Action.h"
#include "glm/gtx/transform.hpp"
#include "Grids/ColorGrid.h"

namespace FluidLib {


	template <class T>
	class InkAction : public Action<T>
	{
	public:
		inline InkAction();
		inline InkAction(T value, ACTION_OPERATION op = ACTION_OPERATION::NONE);
		inline InkAction(ColorGrid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE);
		inline InkAction(T value, ColorGrid<T>* grid, ACTION_OPERATION op = ACTION_OPERATION::NONE);

		virtual void Start() override;
		virtual void Stop() override;
		//virtual void Execute(IPoint& p) override;
		//virtual void Execute(std::vector<IPoint>& points) override;
		glm::vec3* GetColorPtr() { return &_col; }
		std::vector<glm::vec4>& GetColors();
		void SelectColor(int id);
		void AddCurrentColorToPalette();
		void Refresh();
		void SetColor(int id, glm::vec3 col);
	private:
		glm::vec3 _col = {1.0f, 1.0f, 1.0f};
		glm::vec3 _lastcol = { -1.0f, -1.0f, -1.0f };

	};
	template<class T>
	inline InkAction<T>::InkAction()
	{
	}
	template<class T>
	inline InkAction<T>::InkAction(T value, ACTION_OPERATION op)
	{
		SetValue(value);
		SetOperation(op);
	}
	template<class T>
	inline InkAction<T>::InkAction(ColorGrid<T>* grid, ACTION_OPERATION op)
	{
		SetGrid(grid); 
		SetOperation(op);
		if (grid->GetColors().size() > 1) {
			_col = grid->GetColor(1);
			_lastcol = _col;
		}
	}
	template<class T>
	inline InkAction<T>::InkAction(T value, ColorGrid<T>* grid, ACTION_OPERATION op)
	{
		SetValue(value); 
		SetGrid(grid); 
		SetOperation(op);
		if (grid->GetColors().size() > 1) {
			_col = grid->GetColor(1);
			_lastcol = _col;
		}
	}
	template<class T>
	inline void InkAction<T>::Start()
	{
		if (_col != _lastcol) {
			ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
			colgrid->AddColor(_col);
			_lastcol = _col;
			GetValue().id = colgrid->GetId();
		}
		_gridvals = _grid->GetBufferPointer();
	}
	template<class T>
	inline void InkAction<T>::Stop()
	{
		_grid->ReleaseBufferPointer();
		_gridvals = nullptr;
	}
	template<class T>
	inline std::vector<glm::vec4>& InkAction<T>::GetColors()
	{
		ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
		return colgrid->GetColors();
	}
	template<class T>
	inline void InkAction<T>::SelectColor(int id)
	{
		ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
		_col = colgrid->GetColor(id);
		_lastcol = _col;
		GetValue().id = id;
	}
	template<class T>
	inline void InkAction<T>::AddCurrentColorToPalette()
	{
		if (_col != _lastcol) {
			ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
			colgrid->AddColor(_col);
			_lastcol = _col;
			GetValue().id = colgrid->GetId();
		}
	}
	template<class T>
	inline void InkAction<T>::Refresh()
	{
		ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
		colgrid->RefreshColors();
	}
	template<class T>
	inline void InkAction<T>::SetColor(int id, glm::vec3 col)
	{
		ColorGrid<T>* colgrid = static_cast<ColorGrid<T>*>(_grid);
		colgrid->SetColor(id, col);
		if (GetValue().id == id) {
			_col = col;
			_lastcol = _col;
		}
	}
	//template<class T>
	//inline void InkAction<T>::Execute(IPoint& p)
	//{
	//}
	//template<class T>
	//inline void InkAction<T>::Execute(std::vector<IPoint>& points)
	//{
	//}
}