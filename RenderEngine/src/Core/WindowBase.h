#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Events/Events.h"
#include "WindowCursor.h"
#include "Consts/Mouse.h"
#include "Core.h"
#include <map>

namespace RenderEngine {

	struct Properties {
		using EventCallBackFn = std::function<void(Event&)>;
		int width, height;
		EventCallBackFn callback; //Callback function
	};

	class WindowBase
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;
		const char* glsl_version = "#version 330";

		WindowBase(std::string name);
		~WindowBase();
		void Start();
		void Update();
		void StartDraw();
		void EndDraw();
		void Destroy();

		void SetTitle(std::string name);
		void SetBackgroundColor(float R, float G, float B, float A);
		//void SetIcon(std::string& icon, std::string& smallIcon);

		void SetSize(int width, int height);
		int GetWidth();
		int GetHeight();
		void Minimize();
		void UndoMinimize();
		void Maximize();
		void UndoMaximize();
		void Hide(bool b);
		void FullScreen();
		void ExitFullScreen();

		void UnableDepthTest();
		void DisableDepthTest();
		void SetDepthFunction(GLenum func);

		virtual void OnUpdate() {};
		virtual void OnDraw() {};
		virtual void OnCreate() {};
		virtual void OnResize(int width, int height) { _props.width = width; _props.height = height; };

		bool isClosing();

		inline virtual void SetEventCallBack(const EventCallBackFn& callback) { _props.callback = callback; };

		void SetCursor(std::string name);
		void SetCursor(Cursor cursor);
		void CreateCursor(Cursor cursor);
		void CreateCursor(Cursor cursor, std::string name);
		void SetDefaultCursor();
		void HideCursor(bool state);

	protected:
		bool b_closing;
		GLFWwindow* _window;
		Properties _props;

		std::map <std::string, WindowCursor> _cursors;
	private:
		void LoadDefaultCursors();
	};

}