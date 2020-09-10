#pragma once

#include "WindowBase.h"

namespace RenderEngine {

	struct ApplicationProperties {
		WindowBase* window = nullptr;
		std::string name = "";
	};

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

		inline void SetCursor(Cursor cursor) { _props.window->SetCursor(cursor); }
		inline void SetCursor(std::string cursor) { _props.window->SetCursor(cursor); }
		inline void AddCursor(Cursor cursor, std::string name) { _props.window->CreateCursor(cursor, name); }
		inline void SetDefaultCursor() { _props.window->SetDefaultCursor(); }
		inline void HideCursor(bool state) { _props.window->HideCursor(state); }

		inline const ApplicationProperties& GetProperties() { return _props; }

	protected:
		virtual void Run();
		bool _stop = false;

	private:
		static Application* _instance;
		ApplicationProperties _props;
	};

}