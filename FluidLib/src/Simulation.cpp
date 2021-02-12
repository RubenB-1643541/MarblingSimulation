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
		_tools.Update();
		OnUpdate();
		if (!_paused && (std::clock() - _prev)/ (double) CLOCKS_PER_SEC >= 1.0f / _settings.fps) {
			//std::cout << "update: " << (std::clock() - _prev) / (double) CLOCKS_PER_SEC << " fps: " << _settings.fps << std::endl;
			_prev = std::clock();
			
			BeforeUpdate();
			_grids.BindGrids();
			if (_computeshader.IsShaderSet()) {
				_computeshader.Use();
				_computeshader.BindUniforms();
				_computeshader.Dispatch();
				AfterUpdate();
			}
			else {
				std::cerr << "No Compute Shader Set" << std::endl;
			}
			
		}
	}

	void Simulation::Draw()
	{
		
		glViewport(_screenwidth / 2 - _sizex / 2, _screenheight / 2 - _sizey / 2, _sizex, _sizey);
		//glViewport(_screenwidth / 2 - _sizex / 2, _screenheight / 2 - _sizey / 2, _sizex, _sizey);
		_tools.Draw();
		_renderer.Draw();
		OnDraw();
		glViewport(0, 0, _screenwidth, _screenheight);
		
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

	void Simulation::OnKeyPress(int key)
	{
		_keys.keys[key] = true;
		if (key == 340) //Shift
			_keys.shift = true;
		if (key == 341) //Ctrl
			_keys.ctr = true;
	}

	void Simulation::OnKeyRelease(int key)
	{
		_keys.keys[key] = false;
		if (key == 340) //Shift
			_keys.shift = false;
		if (key == 341) //Ctrl
			_keys.ctr = false;
	}

	void Simulation::OnMousePress(int button)
	{
		if(button < N_MOUSEBUTTONS)
			_keys.mouse[button] = true;
	}

	void Simulation::OnMouseRelease(int button)
	{
		if (button < N_MOUSEBUTTONS)
			_keys.mouse[button] = false;
	}

	void Simulation::SetSize(int sizex, int sizey)
	{
		_sizex = sizex; _sizey = sizey; 
		_renderer.SetSimSize(sizex, sizey);
		_tools.SetSimSize(sizex, sizey);
		_computeshader.SetGroupSizes((sizex * sizey)/ WORK_GROUP_SIZE, 1, 1);
		
	}

	void Simulation::SetScreenSize(float width, float height)
	{
		_screenwidth = width;
		_screenheight = height;
	}

}
