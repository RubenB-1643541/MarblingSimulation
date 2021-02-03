#pragma once

#include "Util/Point.h"
#include <vector>
#include <string>

namespace FluidLib {

	class MultiSurface
	{
	public:
		virtual std::vector<FPoint>& GetPoints();
		const std::string& GetName() { return _name; }
		void SetName(const std::string& name) { _name = name; }
	protected:
		std::vector<FPoint> _points;
		bool _changed = true;
		std::string _name = "MultiSurface";
	private:


	};

}