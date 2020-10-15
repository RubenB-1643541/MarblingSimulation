#pragma once

#include "Core/Application.h"
#include "SimulationWindow.h"
#include "MarblingSimulation.h"
#include "Rendering/ShaderStorageBuffer.h"
#include "Rendering/ComputeShaderProgram.h"
#include "Rendering/ShaderProgram.h"
#include "Events/Events.h"
#include "../SimInterface/InterfaceComponentsList.h"

class SimulationApplication : public RenderEngine::Application
{
public:
	SimulationApplication();
	~SimulationApplication();

	virtual void OnStart() override;
	virtual void OnStop() override;
	virtual void OnUpdate() override;

	void CreateInterface();

	virtual bool OnWindowResizeEvent(RenderEngine::WindowResizeEvent& e) override;
	virtual bool OnMouseMoveEvent(RenderEngine::MouseMoveEvent& e) override;
	virtual bool OnMousePressEvent(RenderEngine::MousePressEvent& e) override;
	virtual bool OnMouseReleaseEvent(RenderEngine::MouseReleaseEvent& e) override;
	virtual bool OnMouseScrollEvent(RenderEngine::MouseScrollEvent& e) override;

private:
	MarblingSimulation _sim;
	std::map<std::string, RenderEngine::ShaderStorageBuffer*> _buffers;
	RenderEngine::ComputeShaderProgram _computeshader;
	RenderEngine::ShaderProgram _shader;
	RenderEngine::Shader _shaderdata = { "res/shaders/computegrid_vertexshader.glsl", "res/shaders/computegrid_fragmentshader.glsl", "" };


};

