#include "pch.h"
#include "Application.h"

#include "Timer.h"

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
		_props.window->Start();
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

	bool Application::OnEvent(Event& e)
	{
		EventDisplatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressEvent>(BIND_EVENT(OnKeyPressEvent));
		dispatcher.Dispatch<KeyReleaseEvent>(BIND_EVENT(OnKeyReleaseEvent));
		dispatcher.Dispatch<MouseMoveEvent>(BIND_EVENT(OnMouseMoveEvent));
		dispatcher.Dispatch<MousePressEvent>(BIND_EVENT(OnMousePressEvent));
		dispatcher.Dispatch<MouseReleaseEvent>(BIND_EVENT(OnMouseReleaseEvent));
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(OnMouseScrollEvent));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OnWindowResizeEvent));
		return true;
	}

	void Application::Run()
	{
		if (_props.window != nullptr) {
			while (!_stop) {
				_props.window->Update();
				_props.window->StartDraw();
				
				OnUpdate();
				_props.window->EndDraw();
			}
		}
	}
}