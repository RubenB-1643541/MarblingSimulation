#include "Simulation.h"

namespace FluidLib {

	Simulation::Simulation()
	{

	}

	Simulation::~Simulation()
	{
	}

	void Simulation::Init()
	{
		OnInit();
	}

	void Simulation::Update()
	{
		OnUpdate();
		if (_computeshader.IsShaderSet()) {
			//Bind Buffers
			_computeshader.Use();
			_computeshader.BindUniforms();
			_computeshader.Dispatch();
			//Useprogram
			//Set Uniforms
		}
		else {
			std::cerr << "No Compute Shader Set" << std::endl;
		}
	}

	void Simulation::Draw()
	{
		OnDraw();
	}

}
