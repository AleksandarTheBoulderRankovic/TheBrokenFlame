#pragma once

#include "Stormlight.h"

class ExampleLayer : public Stormlight::Layer {

public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Stormlight::Event& e) override;
private:
	Stormlight::ShaderLibrary m_ShaderLibrary;
	Stormlight::Ref<Stormlight::Shader> m_Shader;
	Stormlight::Ref<Stormlight::VertexArray> m_VertexArray;

	Stormlight::Ref<Stormlight::Shader> m_FlatColorShader, m_TextureShader;
	Stormlight::Ref<Stormlight::VertexArray> m_SquareVA;

	Stormlight::Ref<Stormlight::Texture2D> m_Texture, m_ChernoLogoTexture;

	Stormlight::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};