#include "pch.h"
#include "InterfaceController.h"

#include "Core/Application.h"
#define BIND_EVENT(x) std::bind(&InterfaceController::x, this, std::placeholders::_1)

namespace RenderEngine {
	InterfaceController* InterfaceController::_instance = nullptr;


	InterfaceController::InterfaceController()
	{
		_instance = this;
	}

	InterfaceController::~InterfaceController()
	{
		for (auto c : _components)
			delete c.second;
		_components.clear();
	}

	void InterfaceController::Init()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = KEY_HOME;
		io.KeyMap[ImGuiKey_End] = KEY_END;
		io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = KEY_A;
		io.KeyMap[ImGuiKey_C] = KEY_C;
		io.KeyMap[ImGuiKey_V] = KEY_V;
		io.KeyMap[ImGuiKey_X] = KEY_X;
		io.KeyMap[ImGuiKey_Y] = KEY_Y;
		io.KeyMap[ImGuiKey_Z] = KEY_Z;

		//Clipboard text

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void InterfaceController::Update()
	{
		for (auto c : _components)
			c.second->Update();
	}

	void InterfaceController::Draw()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(Application::Get()->GetWidth(), Application::Get()->GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = _time > 0.0 ? (time - _time) : (1.0f / 60.0f);
		_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		for (auto c : _components)
			c.second->Draw();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//Init ImGUI Draw

	}

	void InterfaceController::OnEvent(Event& event)
	{
		EventDisplatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressEvent>(BIND_EVENT(OnKeyPress));
		dispatcher.Dispatch<KeyReleaseEvent>(BIND_EVENT(OnKeyRelease));
		dispatcher.Dispatch<KeyTypeEvent>(BIND_EVENT(OnKeyType));
		dispatcher.Dispatch<MouseMoveEvent>(BIND_EVENT(OnMouseMove));
		dispatcher.Dispatch<MousePressEvent>(BIND_EVENT(OnMousePress));
		dispatcher.Dispatch<MouseReleaseEvent>(BIND_EVENT(OnMouseRelease));
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT(OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OnWindowResize));
	}

	void InterfaceController::AddComponent(std::string str, InterfaceComponent* component)
	{
		_components.insert(std::make_pair(str, component));
	}

	void InterfaceController::RemoveComponent(const std::string& str)
	{
		_components.erase(str);
	}

	InterfaceComponent* InterfaceController::GetComponent(const std::string& str)
	{
		return _components.at(str);
	}

	bool InterfaceController::OnMousePress(MousePressEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButton()] = true;
		return false;
	}

	bool InterfaceController::OnMouseRelease(MouseReleaseEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButton()] = false;
		return false;
	}

	bool InterfaceController::OnMouseMove(MouseMoveEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool InterfaceController::OnMouseScroll(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetY();
		io.MouseWheelH += e.GetX();

		return false;
	}

	bool InterfaceController::OnKeyPress(KeyPressEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKey()] = true;

		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[KEY_LEFT_CONTROL] || io.KeysDown[KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[KEY_LEFT_SHIFT] || io.KeysDown[KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[KEY_LEFT_ALT] || io.KeysDown[KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[KEY_LEFT_SUPER] || io.KeysDown[KEY_RIGHT_SUPER];

		return false;
	}

	bool InterfaceController::OnKeyRelease(KeyReleaseEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKey()] = false;

		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[KEY_LEFT_CONTROL] || io.KeysDown[KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[KEY_LEFT_SHIFT] || io.KeysDown[KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[KEY_LEFT_ALT] || io.KeysDown[KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[KEY_LEFT_SUPER] || io.KeysDown[KEY_RIGHT_SUPER];
		return false;
	}

	bool InterfaceController::OnKeyType(KeyTypeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKey();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);
		return false;
	}

	bool InterfaceController::OnWindowResize(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);//TODO Temp
		glViewport(0, 0, e.GetWidth(), e.GetWidth());
		return false;
	}

}