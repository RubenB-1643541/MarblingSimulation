#pragma once

#include "Core/Application.h"
#include "SimulationWindow.h"
#include "MarblingSimulation.h"
#include "Rendering/ShaderStorageBuffer.h"
#include "Rendering/ComputeShaderProgram.h"
#include "Rendering/ShaderProgram.h"
#include "Events/Events.h"

class SimulationApplication : public RenderEngine::Application
{
public:
	SimulationApplication();
	~SimulationApplication();

	virtual void OnStart() override;
	virtual void OnStop() override;
	virtual void OnUpdate() override;

	virtual bool OnWindowResizeEvent(RenderEngine::WindowResizeEvent& e);

private:
	MarblingSimulation _sim;
	std::map<std::string, RenderEngine::ShaderStorageBuffer*> _buffers;
	RenderEngine::ComputeShaderProgram _computeshader;
	RenderEngine::ShaderProgram _shader;
	RenderEngine::Shader _shaderdata = { "res/shaders/computegrid_vertexshader.glsl", "res/shaders/computegrid_fragmentshader.glsl", "" };


};

