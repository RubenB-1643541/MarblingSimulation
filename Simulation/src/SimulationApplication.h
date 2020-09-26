#pragma once

#include "Core/Application.h"
#include "SimulationWindow.h"
#include "MarblingSimulation.h"
#include "Rendering/ShaderStorageBuffer.h"
#include "Rendering/ComputeShaderProgram.h"

class SimulationApplication : public RenderEngine::Application
{
public:
	SimulationApplication();
	~SimulationApplication();

	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate();

private:
	MarblingSimulation _sim;
	std::map<std::string, RenderEngine::ShaderStorageBuffer*> _buffers;
	RenderEngine::ComputeShaderProgram _computeshader;



};

