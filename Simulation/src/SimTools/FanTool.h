#pragma once

#include "Tools/ToolBase.h"
#include "Events/SimulationEvent.h"
#include "Tools/Surface/Surfaces.h"
#include "../SimulationCore/GridStructures.h"
#include "Simulation.h"
#include "Tools/Action/Actions.h"


class FanTool : public FluidLib::ToolBase
{
public:
	FanTool(FluidLib::Grid<IVelocity>* velgrid);

	void Draw() override;
	void OnUpdate() override;

	void OnActivate() override;
	void OnDeactivate() override;

	//bool OnUseEvent(ToolUseEvent& event) override;
	//bool OnEndUseEvent(ToolEndUseEvent& event) override;
	bool OnMoveEvent(FluidLib::ToolMoveEvent& event) override;
	bool OnScrollEvent(FluidLib::ToolScrollEvent& event) override;

	inline void SetProjection(glm::mat4 proj) { _proj = proj; _surface->SetProjection(proj); }

private:
	float _dir = 0;

};

