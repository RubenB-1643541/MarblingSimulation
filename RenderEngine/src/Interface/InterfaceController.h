#pragma once
#include "InterfaceComponent.h"
#include "Core/Events/Events.h"
#include "imgui/imgui.h"
#include "Consts/Keys.h"
#include "ImGuiRenderer.h"


namespace RenderEngine {



	class InterfaceController
	{
	public:
		InterfaceController();
		~InterfaceController();

		void Init();
		void Update();
		void Draw();

		void OnEvent(Event& event);

		void AddComponent(std::string str, InterfaceComponent* component);
		void RemoveComponent(const std::string& str);
		InterfaceComponent* GetComponent(const std::string& str);

	private:
		bool OnMousePress(MousePressEvent& e);
		bool OnMouseRelease(MouseReleaseEvent& e);
		bool OnMouseMove(MouseMoveEvent& e);
		bool OnMouseScroll(MouseScrollEvent& e);
		bool OnKeyPress(KeyPressEvent& e);
		bool OnKeyRelease(KeyReleaseEvent& e);
		bool OnKeyType(KeyTypeEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static InterfaceController* _instance;
		std::map<std::string, InterfaceComponent*> _components;

		float _time = 0.0f;

	};

}
