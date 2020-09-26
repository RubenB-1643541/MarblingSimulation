#pragma once
#include "Consts/Keys.h"
#include "Consts/Macros.h"
#include <functional>
#include <string>

namespace RenderEngine {

	enum class EventType {
		None, KeyPressEvent, KeyReleaseEvent, KeyTypeEvent,
		MousePressEvent, MouseReleaseEvent, MouseMoveEvent, MouseScrollEvent,
		WindowResizeEvent, WindowCloseEvent
	};
	enum EventCategory {
		None = 0,
		KeyEvent = BIT(0),
		MouseEvent = BIT(1),
		WindowEvent = BIT(2)
	};

#define EVENT_TYPE(type)	static EventType GetStaticType() {return EventType::##type; }\
							virtual EventType GetEventType() const override {return GetStaticType(); }\
							virtual const char* GetName() const override {return #type; }

#define EVENT_CATEGORY(category) virtual int GetEventCategory() const override {return category; }

	class Event
	{
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