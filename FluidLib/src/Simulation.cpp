#include "Simulation.h"

namespace FluidLib {

	Simulation* Simulation::_instance = nullptr;

	Simulation::Simulation() : _renderer(&_grids)
	{
		_instance = this;
	}

	Simulation::Simulation(int sizex, int sizey) : Simulation()
	{
		SetSize(sizex, sizey);
	}

	Simulation::~Simulation()
	{
	}

	void Simulation::Init()
	{
		_prev = std::clock();
		OnInit();
	}

	void Simulation::Update()
	{
		if (!_paused && (std::clock() - _prev)/ (double) CLOCKS_PER_SEC >= 1.0f / _settings.fps) {
			//std::cout << "update: " << (std::clock() - _prev) / (double) CLOCKS_PER_SEC << " fps: " << 1.0f / _settings.fps << std::endl;
			_prev = std::clock();

			OnUpdate();
			if (_computeshader.IsShaderSet()) {
				_grids.BindGrids();
				_computeshader.Use();
				_computeshader.BindUniforms();
				_computeshader.Dispatch();
			}
			else {
				std::cerr << "No Compute Shader Set" << std::endl;
			}
		}
	}

	void Simulation::Draw()
	{
		OnDraw();
		_renderer.Draw();
		_tools.Draw();
	}
	
	void Simulation::Stop()
	{
		OnSimulationStop(SimulationStopEvent());
	}

	void Simulation::OnEvent(Event& event)
	{
		_tools.OnEvent(event);
		_grids.OnEvent(event);

	}

}
