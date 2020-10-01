#pragma once

#include <iostream>
#include <ctime>
#include "Grids/ComputeShaderController.h"
#include "Grids/GridManager.h"
#include "SimulationSettings.h"
#include "Grids/Renderer/GridRenderer.h"

namespace FluidLib {

	class Simulation
	{
	public:
		Simulation();
		Simulation(int sizex, int sizey);
		~Simulation();

		void Init();
		void Update();
		void Draw();
		void Stop();

		inline virtual void OnInit() {}
		inline virtual void OnUpdate() {}
		inline virtual void OnDraw() {}
		inline virtual void OnStop() {}
		
		//virtual void OnEvent();
		//
		inline ComputeShaderController* GetShader() { return &_computeshader; }
		inline GridManager* GetGrids() { return &_grids; }
		inline GridRenderer* GetRenderer() { return &_renderer; }
		inline int GetSize() { return _sizex * _sizey; }
		inline int GetSizeX() { return _sizex; }
		inline int GetSizeY() { return _sizey; }
		inline void SetSize(int sizex, int sizey) { _sizex = sizex; _sizey = sizey; _renderer.SetSize(GetSize()); }
		inline void SetSizeX(int sizex) { _sizex = sizex; _renderer.SetSize(GetSize()); }
		inline void SetSizeY(int sizey) { _sizey = sizey; _renderer.SetSize(GetSize()); }
		
		inline Settings* GetSettings() { return &_settings; }

		inline void Pause() { _paused = true; }
		inline void UnPause() { _paused = false; }
	protected:
		Settings _settings;

	private:
		int _sizex = -1;
		int _sizey = -1;

		GridManager _grids;
		//Tool Manager
		ComputeShaderController _computeshader;

		GridRenderer _renderer;

		std::clock_t _prev;
		bool _paused = false;

		
	};

}


