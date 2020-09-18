#include "pch.h"
#include "Timer.h"

namespace RenderEngine {
	Timer::Timer() : _prev(0), _current(0), _running(false)
	{
	}
	void Timer::Start()
	{
		_prev = glfwGetTime();
		_running = true;
	}
	
	void Timer::Restart()
	{
		_prev = glfwGetTime();
		_current = 0;
		_running = true;
	}
	
	void Timer::Stop()
	{
		_prev = 0;
		_running = false;
	}
	
	void Timer::Pause()
	{
		_running = false;
	}
	void Timer::UnPause()
	{
		Start();
	}
	double Timer::Update()
	{
		double passed = 0;
		if (_running) {
			double temp = _current;
			passed = glfwGetTime() - _prev;
			_current += passed;
			_prev = temp;
		}	
		return passed;
	}
	double Timer::Duration()
	{
		return _current;
	}
}