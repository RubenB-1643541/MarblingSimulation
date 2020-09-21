#pragma once

#include <iostream>
#include "Grids/ComputeShaderController.h"

namespace FluidLib {

	class Simulation
	{
	public:
		Simulation();
		~Simulation();

		void Init();
		void Update();
		void Draw();

		inline virtual void OnInit() {}
		inline virtual void OnUpdate() {}
		inline virtual void OnDraw() {}
		
		//virtual void OnEvent();
		//
		inline ComputeShaderController& GetShader() { return _computeshader; }
		//inline GridManager& GetGrids() { return _grids; }
	private:
		//Grid Manager
		//Tool Manager
		ComputeShaderController _computeshader;

	};

}


