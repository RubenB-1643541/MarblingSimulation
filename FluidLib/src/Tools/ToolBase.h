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

		virtual void OnBeginUse();
		virtual void OnEndUse();
		virtual void OnUse();

		inline virtual void SetAction(ActionBase* action) { _action = action; }
		inline virtual void SetMovement(Movement* movement) { _movement = movement; _movement->SetProjection(_proj); }
		inline virtual void SetSurface(Surface* surface) { _surface = surface; _surface->SetProjection(_proj); }
		inline virtual void SetProjection(glm::mat4 proj) { _proj = proj; _surface->SetProjection(proj); _movement->SetProjection(proj); }
		inline ActionBase* GetAction() { return _action; }
		inline Movement* GetMovement() { return _movement; }
		inline Surface* GetSurface() { return _surface; }
		inline const std::string& GetName() { return _name; }
		inline void SetName(const std::string& name) { _name = name; }

		inline bool IsUsing() { return _using; }
	protected:
		ActionBase* _action = nullptr;
		Movement* _movement = nullptr;
		Surface* _surface = nullptr;
		glm::mat4 _proj;
		bool _using = false;
		bool _moveEdit = false;
		bool _surfaceEdit = false;
		float _x = 0;
		float _y = 0;
	private:
		std::string _name = "Base";


	};

}

