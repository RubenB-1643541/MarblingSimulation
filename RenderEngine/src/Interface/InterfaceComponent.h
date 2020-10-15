#pragma once

#include <vector>
#include "Core/Log.h"
#include "imgui/imgui.h"

namespace RenderEngine {

	class InterfaceComponent
	{
	public:
		
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual inline void OnInit() {}
		virtual inline void OnUpdate() {}
		virtual inline void OnDraw() {}
		virtual inline void EndDraw() {}
		virtual void AddComponent(InterfaceComponent* component);
		virtual void RemoveComponent(int index);

	protected:
		std::vector<InterfaceComponent*> _subComponents;
		ImGuiWindowFlags _window_flags = 0;


	};

}