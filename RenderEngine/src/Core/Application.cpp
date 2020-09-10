#include "pch.h"
#include "Application.h"

namespace RenderEngine {

	Application* Application::_instance = nullptr;

	Application::Application()
	{
		_instance = this;
	}

	Application::Application(std::string& name) : Application()
	{
		_props.name = name;
	}

	Application::Application(std::string&& name) : Application()
	{
		_props.name = std::move(name);
	}

	Application::Application(WindowBase* window) : Application("")
	{
		_props.window = window;
		_props.window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::Application(std::string& name, WindowBase* window) : Application(name)
	{
		_props.window = window;
		_props.window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::Application(std::string&& name, WindowBase* window) : Application(name)
	{
		_props.window = window;
		_props.window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		delete _props.window;
	}

	void Application::Start()
	{
		//TODO Assert Window not null
		OnStart();
		if (_props.window != nullptr)
			_props.window->Start();

		Run();

		OnStop();
		if (_props.window != nullptr)
			_props.window->Destroy();
	}

	void Application::Stop()
	{
		_stop = true;
	}

	inline void Application::Run()
	{
		double lastTime = glfwGetTime();
		int nbFrames = 0;
		while (!_stop) {
			if (_props.window != nullptr) {
				_props.window->Update();
				_props.window->StartDraw();
			}
			OnUpdate();
			if (_props.window != nullptr)
				_props.window->EndDraw();
		}
	}
}