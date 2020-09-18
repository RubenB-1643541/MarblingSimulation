#pragma once

namespace FluidLib {

	class Simulation
	{
	public:
		Simulation();
		~Simulation();
		
		void Update();
		void Draw();

		inline virtual void OnUpdate() {}
		inline virtual void OnDraw() {}
		
		//virtual void OnEvent();
		//

	private:
		//Grid Manager
		//Tool Manager

	};

}


