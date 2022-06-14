#pragma once

#include "Stormlight.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Stormlight/Renderer/EditorCamera.h"

namespace Stormlight {
	class EditorLayer : public Layer {

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();
		// UI Panels
		void UI_Toolbar();
	private:
		Stormlight::OrthographicCameraController m_CameraController;

		//temp
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		Entity m_HoveredEntity = {};

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree, m_TextureRoof, m_TextureGrass;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
		size_t m_MapWidth = 0, m_MapHeight = 0;

		int m_GuizmoType = -1;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Editor Resources
		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;
	};
}