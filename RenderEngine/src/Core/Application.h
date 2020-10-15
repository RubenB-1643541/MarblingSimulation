#pragma once

#include "WindowBase.h"
#include "Log.h"
#include "Interface/InterfaceController.h"

namespace RenderEngine {

	

	struct ApplicationProperties {
		WindowBase* window = nullptr;
		std::string name = "";
	};

#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	class Application
	{
	public:
		inline static Application* Get() { return _instance; }

		Application();
		Application(std::string& name);
		Application(std::string&& name);
		Application(WindowBase* window);
		Application(std::string& name, WindowBase* window);
		Application(std::string&& name, WindowBase* window);

		~Application();

		void Start();
		void Stop();

		virtual bool OnEvent(Event& e);
		inline virtual void OnStart() {};
		inline virtual void OnStop() {};
		inline virtual void OnUpdate() {};

		inline virtual bool OnKeyPressEvent(KeyPressEvent& e) { return false; }
		inline virtual bool OnKeyReleaseEvent(KeyReleaseEvent& e) { return false; }
		inline virtual bool OnMouseMoveEvent(MouseMoveEvent& e) { return false; }
		inline virtual bool OnMousePressEvent(MousePressEvent& e) { return false; }
		inline virtual bool OnMouseReleaseEvent(MouseReleaseEvent& e) { return false; }
		inline virtual bool OnMouseScrollEvent(MouseScrollEvent& e) { return false; }
		inline virtual bool OnWindowCloseEvent(WindowCloseEvent& e) { Stop(); return true; }
		inline virtual bool OnWindowResizeEvent(WindowResizeEvent& e) { _width = e.GetWidth(); _height = e.GetHeight(); return false; }

		inline void SetCursor(Cursor cursor) { _props.window->SetCursor(cursor); }
		inline void SetCursor(std::string cursor) { _props.window->SetCursor(cursor); }
		inline void AddCursor(Cursor cursor, std::string name) { _props.window->CreateCursor(cursor, name); }
		inline void SetDefaultCursor() { _props.window->SetDefaultCursor(); }
		inline void HideCursor(bool state) { _props.window->HideCursor(state); }

		inline const ApplicationProperties& GetProperties() { return _props; }
		inline float GetWidth() { return _props.window->GetWidth(); }
		inline float GetHeight() { return _props.window->GetHeight(); }

	protected:
		virtual void Run();
		bool _stop = false;
		float _width = 0.0f;
		float _height = 0.0f;
		ApplicationProperties _props;
		InterfaceController _interface;
	private:
		static Application* _instance;


	};

}