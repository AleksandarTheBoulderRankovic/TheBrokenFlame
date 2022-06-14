#include "Weapon.h"

Crossbow::Crossbow(Stormlight::Scene* gameScene) : m_GameScene(gameScene)
{
	m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0];
}

void PlayerCrossbow::StartAttack()
{
	auto& playerTrans = m_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	float rotationAngle = playerTrans.Rotation.z;

	glm::vec2 direction;
	direction.x = 1.0f * cos(rotationAngle) - 0.0f * sin(rotationAngle);
	direction.y = 1.0f * sin(rotationAngle) + 0.0f * cos(rotationAngle);

	Stormlight::Entity projectile = CreateProjectile(playerTrans, direction);
	projectile.GetComponent<Stormlight::TagComponent>().Tag = "PlayerProjectile";
}

void PlayerCrossbow::UpdateAttack(Stormlight::Timestep ts)
{
	Crossbow::UpdateAttack(ts);

	for (auto projectile : m_GameScene->GetAllWithTag("PlayerProjectile"))
	{
		auto xySpeed = projectile.GetComponent<Stormlight::MovementComponent>().XYSpeed;
		auto& transform = projectile.GetComponent<Stormlight::TransformComponent>();
		transform.Translation.x += xySpeed.x * ts * 8.05f;
		transform.Translation.y += xySpeed.y * ts * 8.05f;

		bool collision_detected = false;
		for (auto ent : m_GameScene->GetAllWithTag("Border"))
		{
			if (m_GameScene->CheckCollision(ent, projectile))
			{
				collision_detected = true;
			}
		}

		for (auto ent : m_GameScene->Reg().view<Stormlight::EnemyComponent>())
		{
			Stormlight::Entity entity = { ent, m_GameScene };
			if (m_GameScene->CheckCollision(ent, projectile))
			{
				Hazel::Audio::Play(m_AudioManager->m_ProjectileImpact);
				collision_detected = true;
				auto& hp = entity.GetComponent<Stormlight::HealthComponent>();
				hp.health -= m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().AttackDamage;
				hp.UpdateFullHealthBar();
			}
		}

		if (collision_detected)
		{
			m_GameScene->DestroyEntity(projectile);
		}
	}
}

void PlayerCrossbow::OnDestroy()
{
	for (auto projectile : m_GameScene->GetAllWithTag("PlayerProjectile"))
	{
		m_GameScene->DestroyEntity(projectile);
	}
}

void EnemyCrossbow::StartAttack()
{
	auto& playerTrans = m_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = m_MyEnemy.GetComponent<Stormlight::TransformComponent>();

	glm::vec2 direction;
	direction.x = playerTrans.Translation.x - enemyTrans.Translation.x;
	direction.y = playerTrans.Translation.y - enemyTrans.Translation.y;
	direction = glm::normalize(direction);

	CreateProjectile(enemyTrans, direction);
}

Stormlight::Entity Crossbow::CreateProjectile(Stormlight::TransformComponent& playerTrans, glm::vec2 direction)
{
//	m_OnCooldown = true;
//	m_ShootingCooldownTimeElapsed = 0.0f;

	Stormlight::Entity m_Projectile = { m_GameScene->CreateEntity("Projectile"), m_GameScene };

	auto& mc = m_Projectile.AddComponent<Stormlight::MovementComponent>();
	mc.XYSpeed.x = direction.x;
	mc.XYSpeed.y = direction.y;

	auto& src = m_Projectile.AddComponent<Stormlight::SpriteRendererComponent>();
	src.Texture = m_ProjectileTexture;
	auto& rb2d = m_Projectile.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_Projectile.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform = m_Projectile.GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x = playerTrans.Translation.x + mc.XYSpeed.x * 0.35f;
	transform.Translation.y = playerTrans.Translation.y + mc.XYSpeed.y * 0.35f;
	transform.Translation.z = 0.1f;
	transform.Rotation.z = playerTrans.Rotation.z;

	transform.Scale.x = 0.4f;
	transform.Scale.y = 0.2f;

	m_GameScene->OnRuntimeAddEntity(m_Projectile);
//	m_ShootingTimer.StartTimer();
	return m_Projectile;
}
