#pragma once

#include "Event.h"
#include <sstream>

namespace FluidLib {
	
	/*
	Tool events: Use, SetActive, Move, Scroll
	*/

	class ToolSelectEvent : public Event
	{
	public:
		ToolSelectEvent(int tool, bool first = false);
		inline int GetToolIndex() { return _toolIndex; }

		EVENT_CATEGORY(ToolEvent)
		EVENT_TYPE(ToolSelectEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolSelected: " << _toolIndex;
			return ss.str();
		}
	private:
		int _toolIndex = -1;
		bool _first = false;

	};

	/*
	Indicates What action is used
	*/
	class ToolUseEvent : public Event
	{
	public:
		ToolUseEvent(int action);
		inline int GetAction() { return _action; }

		EVENT_CATEGORY(ToolEvent)
		EVENT_TYPE(ToolUseEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolUseEvent: " << _action;
			return ss.str();
		}
	private:
		int _action;
	};

	/*
	Indicates action stopped being used
	*/
	class ToolEndUseEvent : public Event
	{
	public:
		ToolEndUseEvent(int action);
		inline int GetAction() { return _action; }

		EVENT_CATEGORY(ToolEvent)
			EVENT_TYPE(ToolEndUseEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolUseEvent: " << _action;
			return ss.str();
		}
	private:
		int _action;
	};

	class ToolMoveEvent : public Event
	{
	public:
		ToolMoveEvent(float x, float y);
		inline int GetX() { return _x; }
		inline int GetY() { return _y; }

		EVENT_CATEGORY(ToolEvent)
		EVENT_TYPE(ToolMoveEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolMoveEvent: " << _x << " " << _y;
			return ss.str();
		}
	private:
		float _x, _y = 0;
	};

	class ToolScrollEvent : public Event
	{
	public:
		ToolScrollEvent(float x, float y);
		inline int GetX() { return _x; }
		inline int GetY() { return _y; }

		EVENT_CATEGORY(ToolEvent)
		EVENT_TYPE(ToolScrollEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolMoveEvent: " << _x << " " << _y;
			return ss.str();
		}
	private:
		float _x, _y = 0;
	};

}