#pragma once

#include "../ToolBase.h"

namespace FluidLib {

	/*Actions:
	* Copy
	* Freeze -> Change Flag -> Change Grid Value
	* 
	*/

	class SelectTool : public ToolBase
	{
	public:
		void Draw() override;

		void OnActivate() override;
		void OnDeactivate() override;

		bool OnUseEvent(ToolUseEvent& event) override;
		bool OnMoveEvent(ToolMoveEvent& event) override;
		bool OnScrollEvent(ToolScrollEvent& event) override;
	private:

	};

}