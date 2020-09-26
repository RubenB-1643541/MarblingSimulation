#pragma once

#include "Consts/Mouse.h"
#include "Event.h"
#include <sstream>

namespace RenderEngine {

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(double xpos, double ypos);
		inline double GetX() { return _xpos; }
		inline double GetY() { return _ypos; }

		EVENT_CATEGORY(MouseEvent)
		EVENT_TYPE(MouseMoveEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "MouseMoveEvent: x" << _xpos << " y" << _ypos;
			return ss.str();
		}
	private:
		double _xpos = 0;
		double _ypos = 0;
	};

	class MousePressEvent : public Event
	{
	public:
		MousePressEvent(int button);
		MousePressEvent(int button, double xpos, double ypos);

		EVENT_CATEGORY(MouseEvent)
			EVENT_TYPE(MousePressEvent)

			inline int GetButton() { return _button; }
		inline double GetX() { return _xpos; }
		inline double GetY() { return _ypos; }

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "MousePressEvent: " << _button;
			return ss.str();
		}
	private:
		int _button;
		double _xpos = 0;
		double _ypos = 0;
	};

	class MouseReleaseEvent : public Event
	{
	public:
		MouseReleaseEvent(int button);
		MouseReleaseEvent(int button, double xpos, double ypos);

		EVENT_CATEGORY(MouseEvent)
			EVENT_TYPE(MouseReleaseEvent)

			inline int GetButton() { return _button; }
		inline double GetX() { return _xpos; }
		inline double GetY() { return _ypos; }

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "MouseReleaseEvent: " << _button;
			return ss.str();
		}
	private:
		int _button;
		double _xpos = 0;
		double _ypos = 0;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(double xoffset, double yoffset);
		inline double GetX() { return _xoffset; }
		inline double GetY() { return _yoffset; }

		EVENT_CATEGORY(MouseEvent)
			EVENT_TYPE(MouseScrollEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "MouseScrollEvent: x" << _xoffset << " y" << _yoffset;
			return ss.str();
		}
	private:
		double _xoffset = 0;
		double _yoffset = 0;
	};

}