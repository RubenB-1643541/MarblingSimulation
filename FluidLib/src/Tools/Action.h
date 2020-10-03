#pragma once

namespace FluidLib {

	struct ActionParameters {

	};


	class Action
	{
	public:
		Action();

		virtual void Activate(const ActionParameters&) = 0;
	private:


	};

}