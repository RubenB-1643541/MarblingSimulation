#pragma once

#include <iostream>
#include <ctime>
#include "Grids/ComputeShaderController.h"
#include "Grids/GridManager.h"
#include "SimulationSettings.h"
#include "Grids/Renderer/GridRenderer.h"
#include "Events/Events.h"
#include "Tools/ToolManager.h"

#define WORK_GROUP_SIZE 100
#define N_MOUSEBUTTONS 5

namespace FluidLib {

	struct KeyIo {
		bool keys[500] = {false};
		bool shift = false;
		bool ctr = false;
		bool mouse[N_MOUSEBUTTONS] = { false };
	};

	class Simulation
	{
	public:
		Simulation();
		Simulation(int sizex, int sizey);
		~Simulation();

		static Simulation* Get() { return _instance; }

		void Init();
		void Update();
		void Draw();
		void Stop();

		inline virtual void OnInit() {}
		inline virtual void OnUpdate() {}
		inline virtual void BeforeUpdate() {}
		inline virtual void AfterUpdate() {}
		inline virtual void OnDraw() {}
		
		void OnEvent(Event& event);
		virtual void OnSimulationStart(SimulationStartEvent& event) {}
		virtual void OnSimulationStop(SimulationStopEvent& event) {}
		virtual void OnSimulationPause(SimulationPauseEvent& event) {}
		virtual void OnFrameRateChange(FrameRateChangeEvent& event) {}
		void OnKeyPress(int key);
		void OnKeyRelease(int key);
		void OnMousePress(int button);
		void OnMouseRelease(int button);

		inline ComputeShaderController* GetShader() { return &_computeshader; }
		inline GridManager* GetGrids() { return &_grids; }
		inline GridRenderer* GetRenderer() { return &_renderer; }
		inline int GetSize() { return _sizex * _sizey; }
		inline int GetSizeX() { return _sizex; }
		inline int GetSizeY() { return _sizey; }
		void SetSize(int sizex, int sizey);
		inline int GetScreenSize() { return _screenwidth * _screenheight; }
		inline int GetScreenWidth() { return _screenwidth; }
		inline int GetScreenHeight() { return _screenheight; }
		
		//inline void SetSizeX(int sizex) { _sizex = sizex; _renderer.SetSize(GetSize()); }
		//inline void SetSizeY(int sizey) { _sizey = sizey; _renderer.SetSize(GetSize()); }
		
		inline Settings* GetSettings() { return &_settings; }

		inline void Pause() { _paused = true; OnSimulationPause(SimulationPauseEvent()); }
		inline void UnPause() { _paused = false; OnSimulationStart(SimulationStartEvent()); }

		void SetScreenSize(float width, float height);
		KeyIo* GetKeys() { return &_keys; }

		inline ToolManager* GetTools() { return &_tools; }
	protected:
		Settings _settings;
		GridManager _grids;
		ToolManager _tools;
		ComputeShaderController _computeshader;
		KeyIo _keys;
	private:
		static Simulation* _instance;

		int _sizex = -1;
		int _sizey = -1;
		float _screenwidth = 0;
		float _screenheight = 0;

		GridRenderer _renderer;

		std::clock_t _prev;
		bool _paused = false;
	};

}


