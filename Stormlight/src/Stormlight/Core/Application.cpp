#include "slpch.h"
#include "Application.h"
#include "Stormlight/Core/Log.h"
#include "Stormlight/Core/Input.h"

#include "Stormlight/Renderer/Renderer.h"

#include "GLFW/glfw3.h"

namespace Stormlight {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {

		SL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(SL_BIND_EVENT_FN(Application::OnEvent));
		
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		}

	Application::~Application() {

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SL_BIND_EVENT_FN(Application::OnWindowResize));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	
	}

	void Application::Run() {
		
		while (m_Running) {

			float time = (float)glfwGetTime(); // this should be Platform::GetTime() call
			Timestep timestep = time - m_LastFrameTime;
			timestep = timestep > 0.2f ? 0.2f : timestep;
			m_LastFrameTime = time;
			
			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}

