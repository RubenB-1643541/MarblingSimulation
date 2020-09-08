#pragma once

#include "Event.h"

namespace RenderEngine {

	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(int key);

		inline int GetKey() { return _key; };

		EVENT_CATEGORY(KeyEvent)
			EVENT_TYPE(KeyPressEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "KeyPressEvent: " << _key;
			return ss.str();
		}
	private:
		int _key;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(int key);

		inline int GetKey() { return _key; };

		EVENT_CATEGORY(KeyEvent)
			EVENT_TYPE(KeyReleaseEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << _key;
			return ss.str();
		}
	private:
		int _key;
	};

	class KeyTypeEvent : public Event
	{
	public:
		KeyTypeEvent(int key);

		inline int GetKey() { return _key; };

		EVENT_CATEGORY(KeyEvent)
			EVENT_TYPE(KeyTypeEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "KeyTypeEvent: " << _key;
			return ss.str();
		}
	private:
		int _key;
	};

}