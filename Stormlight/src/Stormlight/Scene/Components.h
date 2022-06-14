#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Stormlight/Scene/SceneCamera.h"
#include "Stormlight/Scene/ScriptableEntity.h"
#include "Stormlight/Renderer/Texture.h"
#include "Stormlight/Renderer/SubTexture2D.h"
#include "Stormlight/Utils/UtilityFunctions.h"

namespace Stormlight {

	struct TagComponent
	{
		std::string Tag;
		std::string SecondTag = "";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::vec2 Get2DTranslate() const 
		{
			return glm::vec2(Translation.x, Translation.y);
		}

		glm::vec4 GetBounds() const
		{
			// right, top, left, bottom edge
			return glm::vec4(Translation.x + Scale.x / 2, Translation.y + Scale.y / 2, Translation.x - Scale.x / 2, Translation.y - Scale.y / 2);
		}

	};

	struct MovementComponent
	{
		float Speed = 5.0f;
		glm::vec2 XYSpeed = { 0.0f, 0.0f };

		MovementComponent() = default;
		MovementComponent(const MovementComponent&) = default;
		MovementComponent(const float speed) : Speed(speed) {}

		float GetSpeed() const
		{
			return Speed;
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		Ref<Texture2D> secondTexture;
		Ref<SubTexture2D> subTexture;
		Ref<SubTexture2D> SecondSubTexture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct AnimationComponent
	{
		// Movement
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_IdleSubTextures;
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkForwardSubTextures;
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkSidewaysSubTextures;
		float m_WalkTimeElapsed = 0.0f;
		size_t m_WalkIndex = 0;

		// Movement while shooting
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_IdleFireSubTextures;
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkForwardFireSubTextures;
		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkSidewaysFireSubTextures;
		float m_IdleTimeElapsed = 0.0f;
		float m_FrameDuration = 0.2f;
		size_t m_IdleIndex = 0;

		std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_AttackSubTextures;
		float m_AttackTimeElapsed = 0.0f;
		size_t m_AttackIndex = 0;

		AnimationComponent() = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	// Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		bool Collided = false;
		Entity collisionEntity;
		bool NoCollision = false;
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
		void HandleCollision(Entity& myEntity, Timestep ts);
		void FindCollision(Entity& myEntity, const std::string& tag);
	};

	struct PlayerComponent
	{
		int Gold = 150;
		int FirstSkill = 0;
		int SecondSkill = 1;
		int NumOfSpyglass = 1;
		int NumOfWarpStone = 1;
		int NumOfHPpot = 1;
		float AttackDamage = 100.0f;
		float AttackSpeed = 1.0f;
		float MagicDamage = 1.0f;
		float CooldownReduction = 1.0f;
		bool StartCombat = false;
		PlayerComponent(int first, int second) : FirstSkill(first), SecondSkill(second) {}

		void AddGold(int gained) { Gold += gained; }
		void SubtractGold(int lost) { Gold += lost; }
		void GainConsumable();
	};

	struct ItemComponent
	{
		uint32_t itemID;
		float HpBonus = 0.0f;
		float AttackBonus = 0.0f;
		float MagicBonus = 0.0f;
		float AttackSpeedBonus = 0.0f;
		float MovementSpeedBonus = 0.0f;
		float CooldownReductionBonus = 0.0f;
		int Price = 0;

		ItemComponent() = default;
	};

	struct EnemyComponent
	{
		bool Hooked = false;
		float AttackDamage = 60.0f;
		EnemyComponent() = default;
	};

	struct MinotaurComponent
	{
		Timer AttackAnimationTimer = Timer{ 2.0f, 0.0f };
		Timer IdleAnimationTimer = Timer{ 4.0f, 0.0f };
		Timer AttackWindUpAnimationTimer = Timer{ 0.4f, 0.0f };
		int PhaseIndex = 0;
		float AnimationParametar = 0.0f;
		Entity Axe;

		MinotaurComponent() = default;
	};

	struct PeasantComponent
	{
		Timer PhaseTimer = Timer(1.06f, 0.0f);
		int PhaseIndex = 0;
		Entity Dagger;
		float AttackDamage = 46.0f;
		float AnimationParametar = 0.0f;
		PeasantComponent() = default;
	};

	struct HealthComponent
	{
		float health = 100.0f;
		float maxHealth = 100.0f;
		float offset = 0.0f;
		Entity m_EmptyBar;
		Entity m_FullBar;
		Entity owner;

		HealthComponent() = default;
		void CreateHealthBars(float maxHP = 100.0f);
		void UpdateFullHealthBar();
		void UpdateHealthBars();
		void DestroyHealthBars();
	};
}