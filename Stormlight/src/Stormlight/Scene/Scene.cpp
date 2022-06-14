#include "slpch.h"
#include "Scene.h"

#include "Stormlight/Scene/Components.h"
#include "Stormlight/Scene/Entity.h"
#include "Stormlight/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
//#include "Stormlight/Renderer/EditorCamera.h"

namespace Stormlight {

	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}
		SL_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		// Here it's probably 0 0 in my game
		m_PhysicsWorld = new b2World({ 0.0f, 0.0f });
		auto view = m_Registry.view<Rigidbody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				bc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	// Refactor this
	void Scene::OnRuntimeAddEntity(Entity entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		bodyDef.angle = transform.Rotation.z;
		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RuntimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
			bc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					//TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			// Retrive transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				if (!entity.HasComponent<MovementComponent>())
				{
					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}

		// Render2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			float offset = m_Offset;
			for (auto ent : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(ent);
				if (m_Registry.get<TagComponent>(ent).Tag != "ItemSlot" && m_Registry.get<TagComponent>(ent).Tag != "Encounter" && m_Registry.get<TagComponent>(ent).Tag != "Arrow" && m_Registry.get<TagComponent>(ent).Tag != "HealthBar")// && m_Registry.get<TagComponent>(ent).Tag != "Bullet")
				{
					auto& trans = m_Registry.get<TransformComponent>(ent);
					trans.Translation.z = offset;
					offset += 0.001f;
				}
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)ent);

			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto ent : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(ent);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)ent);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize out non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}

	}

	Entity Scene::GetPrimeryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	std::vector<Entity> Scene::GetAllWithTag(const std::string& tag)
	{
		std::vector<Entity> tagEntities;

		m_Registry.each([&](auto ent) {

			Entity entity = { ent, this };
			auto e_tag = entity.GetComponent<TagComponent>().Tag;
			if (e_tag == tag)
				tagEntities.push_back(entity);
			});

		return tagEntities;
	}

	bool Scene::CheckCollision(Entity entityA, Entity entityB)
	{
		auto& transformA = entityA.GetComponent<TransformComponent>();
		auto& transformB = entityB.GetComponent<TransformComponent>();

		auto& bc2dA = entityA.GetComponent<BoxCollider2DComponent>();
		auto& bc2dB = entityB.GetComponent<BoxCollider2DComponent>();

		b2Transform b2transformA(b2Vec2(transformA.Translation.x, transformA.Translation.y), b2Rot(transformA.Rotation.z));
		b2Transform b2transformB(b2Vec2(transformB.Translation.x, transformB.Translation.y), b2Rot(transformB.Rotation.z));

		bool overlap = b2TestOverlap(((b2Fixture*)bc2dA.RuntimeFixture)->GetShape(), 0, ((b2Fixture*)bc2dB.RuntimeFixture)->GetShape(), 0, b2transformA, b2transformB);
		return overlap;
	}

	bool Scene::CheckCollision(entt::entity entityA, entt::entity entityB)
	{
		auto& transformA = m_Registry.get<TransformComponent>(entityA);
		auto& transformB = m_Registry.get<TransformComponent>(entityB);

		auto& bc2dA = m_Registry.get<BoxCollider2DComponent>(entityA);
		auto& bc2dB = m_Registry.get<BoxCollider2DComponent>(entityB);

		b2Transform b2transformA(b2Vec2(transformA.Translation.x, transformA.Translation.y), b2Rot(transformA.Rotation.z));
		b2Transform b2transformB(b2Vec2(transformB.Translation.x, transformB.Translation.y), b2Rot(transformB.Rotation.z));

		bool overlap = b2TestOverlap(((b2Fixture*)bc2dA.RuntimeFixture)->GetShape(), 0, ((b2Fixture*)bc2dB.RuntimeFixture)->GetShape(), 0, b2transformA, b2transformB);
		return overlap;
	}

	bool Scene::CheckMouseEntityCollision(glm::vec2 mouseLocation, Entity& entity)
	{
		auto& tc = entity.GetComponent<Stormlight::TransformComponent>();
		glm::vec4 bounds = tc.GetBounds();

		if (mouseLocation.x < bounds.x && mouseLocation.x > bounds.z && mouseLocation.y < bounds.y && mouseLocation.y > bounds.w)
			return true;

		return false;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<MovementComponent>(Entity entity, MovementComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<AnimationComponent>(Entity entity, AnimationComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}


	template<>
	void Scene::OnComponentAdded<HealthComponent>(Entity entity, HealthComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<EnemyComponent>(Entity entity, EnemyComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<MinotaurComponent>(Entity entity, MinotaurComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<PlayerComponent>(Entity entity, PlayerComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<ItemComponent>(Entity entity, ItemComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<PeasantComponent>(Entity entity, PeasantComponent& component)
	{

	}
}