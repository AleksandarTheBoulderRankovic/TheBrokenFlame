#pragma once

#include "Stormlight/Core/Core.h"

#include "Window.h"
#include "Stormlight/Core/LayerStack.h"
#include "Stormlight/Events/Event.h"
#include "Stormlight/Events/ApplicationEvent.h"

#include "Stormlight/ImGui/ImGuiLayer.h"

#include "Stormlight/Core/Timestep.h"

namespace Stormlight {

	//class STORMLIGHT_API Application
	class Application
	{
	public:
		Application(const std::string& name = "Stormlight App");
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance;  }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();

}
