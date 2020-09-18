#pragma once

#include "Event.h"
#include <sstream>

namespace FluidLib {

	/*
	Simulation Events: Start, Stop, Pause, ChangeFrameRate
	*/

	class SimulationStartEvent : public Event
	{
	public:
		SimulationStartEvent();

		EVENT_CATEGORY(SimulationEvent)
		EVENT_TYPE(SimulationStartEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "Simulation Start";
			return ss.str();
		}
	private:


	};

	class SimulationStopEvent : public Event
	{
	public:
		SimulationStopEvent();

		EVENT_CATEGORY(SimulationEvent)
			EVENT_TYPE(SimulationStopEvent)

			inline std::string GetString() const override {
			std::stringstream ss;
			ss << "Simulation Stop";
			return ss.str();
		}
	private:


	};

	class SimulationPauseEvent : public Event
	{
	public:
		SimulationPauseEvent();

		EVENT_CATEGORY(SimulationEvent)
		EVENT_TYPE(SimulationStartEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "Simulation Pause";
			return ss.str();
		}
	private:


	};

	class FrameRateChangeEvent : public Event
	{
	public:
		FrameRateChangeEvent(float framerate);

		EVENT_CATEGORY(SimulationEvent)
		EVENT_TYPE(FrameRateChangeEvent)

		inline std::string GetString() const override {
			std::stringstream ss;
			ss << "Frame rate change: " << _framerate;
			return ss.str();
		}
	private:
		float _framerate = 0;

	};

}