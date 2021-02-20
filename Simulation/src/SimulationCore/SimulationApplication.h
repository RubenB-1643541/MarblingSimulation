#pragma once

#include "Core/Application.h"
#include "SimulationWindow.h"
#include "MarblingSimulation.h"
#include "Rendering/ShaderStorageBuffer.h"
#include "Rendering/ComputeShaderProgram.h"
#include "Rendering/ShaderProgram.h"
#include "Events/Events.h"
#include "../SimInterface/InterfaceComponentsList.h"
#include "../SimUtils/ShortCutHandler.h"
#include "../FileIO/SimulationSaveLoad.h"
#include "Grids/ColorGrid.h"
#include "Grids/FlagGrid.h"
#include "../SimUtils/SaveStateHandler.h"

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
	inline virtual bool OnKeyPressEvent(RenderEngine::KeyPressEvent& e) override;
	inline virtual bool OnKeyReleaseEvent(RenderEngine::KeyReleaseEvent& e) override;
	void StartSimulation();
	bool LoadSimulation(const std::string& file);
	bool ImportImage(const std::string& file);
	inline void SetLoad(bool l) { _load = l; }
private:
	void CreateGrids();
	void CreateUniforms();
	void InitShortCuts();
	ShortCutHandler _shortcuts;
	bool _simrunning = false;
	MarblingSimulation _sim;
	std::map<std::string, RenderEngine::ShaderStorageBuffer*> _buffers;
	RenderEngine::ComputeShaderProgram _computeshader;
	RenderEngine::ShaderProgram _shader;
	RenderEngine::Shader _shaderdata = { "res/shaders/ink_vertexshader.glsl", "res/shaders/ink_fragmentshader.glsl", "" };
	std::string _computeShader = "res/computeshaders/StamAdvection_Float.glsl";
	//RenderEngine::Shader _shaderdata = { "res/shaders/computegrid_vertexshader.glsl", "res/shaders/computegrid_fragmentshader.glsl", "" };

	bool _load = false;
};

