#include "DrippingTool.h"

namespace FluidLib {

	DrippingTool::DrippingTool()
	{
		SetName("Dripping");
	}

	void DrippingTool::OnUse()
	{
		std::vector<IPoint> points = _surface->GetSurfacePoints();
		std::vector<FPoint> transpoints = { { 0.0f, 0.0f } };
		if (_multisurface != nullptr)
			transpoints = _multisurface->GetPoints();
		
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, std::distance(points.begin(), points.end()) - 1);
		int limit = points.size() * _perc;
		for (int i = 0; i < limit; ++i ) {
			auto start = points.begin();
			std::advance(start, dis(gen));
			_action->Execute(*start);
		}

		//for (IPoint& p : points) {
		//	for (FPoint& t : transpoints) {
		//		_action->Execute(p + t);
		//	}
		//}
	}

}