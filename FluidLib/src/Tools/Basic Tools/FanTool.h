#pragma once

#include "../ToolBase.h"

namespace FluidLib {

	class FanTool : public ToolBase
	{
	public:
		FanTool();

		void Draw() override;
		void OnUpdate() override;

		void OnActivate() override;
		void OnDeactivate() override;

		//bool OnUseEvent(ToolUseEvent& event) override;
		//bool OnEndUseEvent(ToolEndUseEvent& event) override;
		bool OnMoveEvent(ToolMoveEvent& event) override;
		bool OnScrollEvent(ToolScrollEvent& event) override;
		inline void SetProjection(glm::mat4 proj) { _proj = proj; _surface->SetProjection(proj); }

	private:

	};

}