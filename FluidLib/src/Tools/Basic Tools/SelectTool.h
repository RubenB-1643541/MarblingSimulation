#pragma once

#include "../ToolBase.h"
#include "../Surface/Rectangle.h"
#include <map>
#include "../Action.h"
#include "../Action/CopyAction.h"

namespace FluidLib {

	/*
	* 
	*/
	enum class MOVEMENT_MODE {BEGIN, SIZE, NONE};

	class SelectTool : public ToolBase
	{
	public:
		SelectTool();
		void Draw() override;
		void OnUpdate() override;

		void OnActivate() override;
		void OnDeactivate() override;

		bool OnUseEvent(ToolUseEvent& event) override;
		bool OnEndUseEvent(ToolEndUseEvent& event) override;
		bool OnMoveEvent(ToolMoveEvent& event) override;
		bool OnScrollEvent(ToolScrollEvent& event) override;
		inline void SetProjection(glm::mat4 proj) { _proj = proj; _surface->SetProjection(proj); }

		void SetCopyAction(ActionBase* copyaction);
		void SetPasteAction(ActionBase* pasteaction);
		void SetCutAction(ActionBase* cutaction);

		void AddAction(const std::string& actionname, ActionBase* action);
		bool ExecuteAction(const std::string& action);

		bool Copy();
		bool Paste();
		bool Cut();

		const std::vector<std::string>& GetActions() { return _actionnames; }

	private:
		void SwitchMode();
		Rectangle* _rect = nullptr;
		MOVEMENT_MODE _move;
		std::map<std::string, ActionBase*> _actions;
		std::vector<std::string> _actionnames;
	};

}