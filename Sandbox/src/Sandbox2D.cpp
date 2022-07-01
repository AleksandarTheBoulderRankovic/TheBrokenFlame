#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

void Sandbox2D::OnAttach()
{
	m_CameraController.SetZoomLevel(5.0f);

	Stormlight::FramebufferSpecification fbSpec;
	fbSpec.Attachments = { Stormlight::FramebufferTextureFormat::RGBA8, Stormlight::FramebufferTextureFormat::RED_INTEGER, Stormlight::FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Stormlight::Framebuffer::Create(fbSpec);

	m_ViewportSize = { 1280.0f, 720.0f };
	
	m_Level = Stormlight::CreateRef<Level>(m_ViewportSize.x, m_ViewportSize.y);
	m_Level->OnCreate();
}

void Sandbox2D::OnDetach()
{
	//m_Level->OnDestroy();
}

void Sandbox2D::OnUpdate(Stormlight::Timestep ts)
{
	m_Framebuffer->Bind();
	if (m_GameRunning)
	{
		if (Stormlight::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Level->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		m_CameraController.OnUpdate(ts);

		// Renderer
		Stormlight::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Stormlight::RenderCommand::Clear();

		m_Level->OnUpdate(ts);
	}
	else
	{
		Stormlight::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Stormlight::RenderCommand::Clear();
	}
	m_Framebuffer->Unbind();
}

void Sandbox2D::OnImGuiRender()
{

	if (!m_GameRunning)
	{
		int viewportPosX, viewportPosY;
		Stormlight::Application::Get().GetWindow().GetWindowPos(&viewportPosX, &viewportPosY);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		auto& io = ImGui::GetIO();
		io.DisplaySize.x = m_ViewportSize.x;
		io.DisplaySize.y = m_ViewportSize.y;
		ImGui::SetNextWindowSize(ImVec2(m_ViewportSize.x, m_ViewportSize.y));
		auto window = Stormlight::Application::Get().GetWindow().GetNativeWindow();
		ImGui::SetNextWindowPos(ImVec2(viewportPosX, viewportPosY));
		ImGui::Begin("##Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);

		ImGui::Image((ImTextureID)m_Level->m_TextureRepository->m_MenuBackground->GetRendererID(), { 1280, 720 }, { 0, 1 }, { 1, 0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImGui::SetWindowFontScale(2.8);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

		float x = ((-1.0f + 8.8888889f) * m_ViewportSize.x) / 17.7777778f;
		float y = ((-(2.0f - 5.0f)) * m_ViewportSize.y) / 10.0f;

		
		ImGui::SetCursorPos(ImVec2{ x, y });
	
		if (!m_Level->m_ControlledPlayer->m_GameOver && ImGui::Button("Continue   "))
		{
			m_GameRunning = true;
		}
		
		y = ((-(1.0f - 5.0f)) * m_ViewportSize.y) / 10.0f;
		ImGui::SetCursorPos(ImVec2{ x, y });
		if (ImGui::Button("New Game"))
		{
			m_Level->ReloadLevel();
			m_GameRunning = true;
		}

		y = ((-(0.0f - 5.0f)) * m_ViewportSize.y) / 10.0f;
		ImGui::SetCursorPos(ImVec2{ x, y });
		if (ImGui::Button("Close           "))
		{
			OnDetach();
			Stormlight::Application::Get().Close();
		}
		
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(2);
		ImGui::End();

	}

	if (m_GameRunning)
	{
		int viewportPosX, viewportPosY;
		Stormlight::Application::Get().GetWindow().GetWindowPos(&viewportPosX, &viewportPosY);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		auto& io = ImGui::GetIO();
		io.DisplaySize.x = m_ViewportSize.x;
		io.DisplaySize.y = m_ViewportSize.y;
		ImGui::SetNextWindowSize(ImVec2(m_ViewportSize.x, m_ViewportSize.y));
		auto window = Stormlight::Application::Get().GetWindow().GetNativeWindow();
		ImGui::SetNextWindowPos(ImVec2(viewportPosX, viewportPosY));
		ImGui::SetNextWindowFocus();
		ImGui::Begin("##Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Stormlight::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		m_Level->m_HUDManager->RenderHUD();

		ImGui::End();
		ImGui::PopStyleVar();

		if (m_Level->m_ControlledPlayer->m_GameOver)
			m_GameRunning = false;
	}
}

void Sandbox2D::OnEvent(Stormlight::Event& e)
{
	m_CameraController.OnEvent(e);
	if(m_GameRunning)
		m_Level->OnEvent(e);

	Stormlight::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Stormlight::KeyPressedEvent>(SL_BIND_EVENT_FN(Sandbox2D::OnKeyPressed));
}

bool Sandbox2D::OnKeyPressed(Stormlight::KeyPressedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case SL_KEY_ESCAPE:
		{
			m_GameRunning = false;
			break;
		}
	}

	return true;
}
