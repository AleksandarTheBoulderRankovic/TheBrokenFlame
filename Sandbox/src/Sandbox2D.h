#pragma once

#include "Stormlight.h"

#include "Level.h"

class Sandbox2D : public Stormlight::Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Stormlight::Event& e) override;
	bool OnKeyPressed(Stormlight::KeyPressedEvent& e);
private:
	Stormlight::OrthographicCameraController m_CameraController;
	Stormlight::Ref<Stormlight::Framebuffer> m_Framebuffer;

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	// Game Characters
	Stormlight::Ref<Level> m_Level;
	bool m_GameRunning = true;

	Stormlight::Ref<Stormlight::Shader> m_FlatColorShader;
	Stormlight::Ref<Stormlight::VertexArray> m_SquareVA;
};