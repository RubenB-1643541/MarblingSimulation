#pragma once

#include "../Events/Events.h"
#include "Action.h"
#include "Movement.h"
#include "Surface.h"

namespace FluidLib {
	
#define BIND_EVENT(x) std::bind(&ToolBase::x, this, std::placeholders::_1)

	class ToolBase
	{
	public:
		virtual void Draw();
		virtual inline void OnDraw() {}
		virtual void Update();
		virtual inline void OnUpdate() {}

		virtual void OnActivate() {}
		virtual void OnDeactivate() {}

		virtual bool OnEvent(Event& toolevent);
		//virtual inline bool OnSelectEvent(ToolSelectEvent& event) {}
		virtual bool OnUseEvent(ToolUseEvent& event);
		virtual bool OnEndUseEvent(ToolEndUseEvent& event);
		virtual bool OnMoveEvent(ToolMoveEvent& event);
		virtual bool OnScrollEvent(ToolScrollEvent& event);

		void OnBeginUse();
		void OnEndUse();
		virtual void OnUse();

		inline void SetAction(ActionBase* action) { _action = action; }
		inline void SetMovement(Movement* movement) { _movement = movement; }
		inline void SetSurface(Surface* surface) { _surface = surface; }
		inline void SetProjection(glm::mat4 proj) { _surface->SetProjection(proj); _movement->SetProjection(proj); }
		inline ActionBase* GetAction() { return _action; }
		inline Movement* GetMovement() { return _movement; }
		inline Surface* GetSurface() { return _surface; }
	protected:
		ActionBase* _action;
		Movement* _movement;
		Surface* _surface;
	private:
		bool _using = false;

	};

}

