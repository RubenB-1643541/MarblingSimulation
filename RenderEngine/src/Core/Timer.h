#pragma once

#include <time.h>
#include <GLFW/glfw3.h>

namespace RenderEngine {

	class Timer
	{
	public:
		Timer();
		void Start();
		void Restart();
		void Stop();
		void Pause();
		void UnPause();
		//Returns passed time since previous update
		double Update();
		double Duration();
		

	private:
		double _prev;
		double _current;
		bool _running;

	};

}

