#include "SimulationEvent.h"

namespace FluidLib {

	SimulationStartEvent::SimulationStartEvent()
	{
	}

	SimulationStopEvent::SimulationStopEvent()
	{
	}

	SimulationPauseEvent::SimulationPauseEvent()
	{
	}

	FrameRateChangeEvent::FrameRateChangeEvent(float framerate) : _framerate(framerate)
	{
	}

}