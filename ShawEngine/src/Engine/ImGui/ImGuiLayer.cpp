#include "sepch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Engine/Application.h"
#include "Engine/KeyCodes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ShawEngine {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)
	ImGuiLayer::ImGuiLayer(): Layer("ImGuiLayer"){
	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//临时实现，直接绑定GLFW键码，后续应该使用ShawEngine键码
		io.KeyMap[ImGuiKey_Tab] = SE_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SE_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SE_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SE_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SE_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SE_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = SE_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = SE_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = SE_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = SE_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = SE_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SE_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = SE_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = SE_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = SE_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SE_KEY_A;
		io.KeyMap[ImGuiKey_C] = SE_KEY_C;
		io.KeyMap[ImGuiKey_V] = SE_KEY_V;
		io.KeyMap[ImGuiKey_X] = SE_KEY_X;
		io.KeyMap[ImGuiKey_Y] = SE_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = SE_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		if (needRender) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispacther(e);
		dispacther.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
		dispacther.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
		dispacther.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
		dispacther.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased));
		dispacther.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMoved));
		dispacther.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseSrolled));
		dispacther.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResized));
		dispacther.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));

	}

	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseSrolled(MouseScrolledEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();
		return false;
	}

	bool esc_lock = false;
	bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		if (e.GetKeyCode() == io.KeyMap[ImGuiKey_Escape] && !esc_lock) {
			needRender = !needRender;
			esc_lock = true;
		}
		return false;
	}

	bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		if (e.GetKeyCode() == io.KeyMap[ImGuiKey_Escape]) {
			esc_lock = false;
		}
		return false;
	}

	bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);
		return false;
	}

	bool ImGuiLayer::OnWindowResized(WindowResizeEvent& e){
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

}