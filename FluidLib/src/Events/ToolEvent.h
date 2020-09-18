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
	Indicates if it is the first a secondary action
	*/
	class ToolUseEvent : public Event
	{
	public:
		ToolUseEvent(int action, int actionNumber = 1);
		inline int GetAction() { return _action; }
		inline int GetActionNumber() { return _actionNumber; }

		EVENT_CATEGORY(ToolEvent)
		EVENT_TYPE(ToolUseEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "ToolUseEvent: " << _action << " number: "<< _actionNumber;
			return ss.str();
		}
	private:
		int _action;
		int _actionNumber;
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