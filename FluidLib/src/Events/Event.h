#pragma once

#include <string>
#include <functional>

#define BIT(x) (1 << x)

namespace FluidLib {

	/*
	Tool events: Use, SetActive, UseSecond, Move
	Grid events, select, modified, 
	Simulation Events: Start, Stop, Pause, ChangeFrameRate
	*/

	enum class EventType {
		None, 
		ToolSelectEvent, ToolUseEvent, ToolUseSecEvent, ToolMoveEvent, ToolScrollEvent,
		GridSelectEvent, GridModifiedEvent,
		SimulationStartEvent, SimulationStopEvent, SimulationPauseEvent, FrameRateChangeEvent
	};

	enum EventCategory {
		None = 0,
		ToolEvent = BIT(0),
		GridEvent = BIT(1),
		SimulationEvent = BIT(2)
	};

#define EVENT_TYPE(type)	static EventType GetStaticType() {return EventType::##type; }\
							virtual EventType GetEventType() const override {return GetStaticType(); }\
							virtual const char* GetName() const override {return #type; }

#define EVENT_CATEGORY(category) virtual int GetEventCategory() const override {return category; }

	class Event {
	public:
		friend class EventDisplatcher;
		virtual EventType GetEventType() const = 0;
		virtual int GetEventCategory() const = 0;
		inline virtual const char* GetName() const = 0;

		inline virtual std::string GetString() const { return GetName(); }

		void SetHandled() { _handled = true; }
		bool IsHandled() { return _handled; }

		inline bool IsInCategory(EventCategory cat) {
			return GetEventCategory() & cat;
		}

	protected:
		EventType _type = EventType::None;
		EventCategory _cat = EventCategory::None;
	private:
		bool _handled = false;
	};

	class EventDisplatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDisplatcher(Event& e) : _event(e) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (_event.GetEventType() == T::GetStaticType()) {
				_event._handled = func(*(T*)&_event);
				return true;
			}
			return false;
		}

	private:
		Event& _event;
	};

}