#pragma once

#include "entt.hpp"

#include "Stormlight/Core/Timestep.h"
#include "Stormlight/Renderer/EditorCamera.h"


class b2World;


namespace Stormlight {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnRuntimeAddEntity(Entity entity);

		entt::registry& Reg() { return m_Registry; }

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimeryCameraEntity();
		std::vector<Entity> GetAllWithTag(const std::string& tag);
	public:
		bool CheckCollision(Entity entityA, Entity entityB);
		bool CheckCollision(entt::entity entityA, entt::entity entityB);
		//bool CheckEntityCollision(Entity entityA, Entity entityB);
		bool CheckMouseEntityCollision(glm::vec2 mouseLocation, Entity& entity);
		void SetOffset(float offset) { m_Offset = offset; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
		float m_Offset = 0.0f;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}