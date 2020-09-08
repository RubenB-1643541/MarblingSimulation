#pragma once

#include "Event.h"

namespace RenderEngine {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height);

		inline int GetWidth() { return _width; }
		inline int GetHeight() { return _height; }

		EVENT_CATEGORY(WindowEvent)
			EVENT_TYPE(WindowResizeEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: width " << _width << " height " << _height;
			return ss.str();
		}
	private:
		int _width;
		int _height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent();

		EVENT_CATEGORY(WindowEvent)
			EVENT_TYPE(WindowCloseEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}
	private:


	};

}
