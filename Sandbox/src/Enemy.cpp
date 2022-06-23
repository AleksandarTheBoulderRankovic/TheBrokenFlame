#include "Enemy.h"
#include "Animation.h"

Stormlight::Entity Enemy::s_MainPlayer = { entt::null, nullptr };

void Enemy::OnCreate()
{
	s_MainPlayer = this->GetScene()->GetAllWithTag("MainCharacter")[0];
}

void Enemy::OnDestroy()
{
	this->GetComponent<Stormlight::HealthComponent>().DestroyHealthBars();
}

void Enemy::OnHooked(Stormlight::Timestep ts)
{
	auto& playerTrans = s_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = this->GetComponent<Stormlight::TransformComponent>();

	glm::vec2 direction;
	direction.x = enemyTrans.Translation.x - playerTrans.Translation.x;
	direction.y = enemyTrans.Translation.y - playerTrans.Translation.y;
	direction = glm::normalize(direction);

	enemyTrans.Translation.x -= direction.x * 10.0f * ts;
	enemyTrans.Translation.y -= direction.y * 10.0f * ts;

	if (this->GetScene()->CheckCollision(s_MainPlayer, *this))
	{
		this->GetComponent<Stormlight::EnemyComponent>().Hooked = false;
	}
}

void Bandit::OnCreate()
{
	Enemy::OnCreate();
	auto& src = this->GetComponent<Stormlight::SpriteRendererComponent>();
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	src.subTexture = ac.m_WalkForwardSubTextures[0];
	ac.m_FrameDuration = 0.2f;

	auto& move = this->GetComponent<Stormlight::MovementComponent>();
	move.Speed = 2.0f;

	m_Weapon = new EnemyCrossbow(this->GetScene(), *this);

	m_PatrolHelper = this->GetScene()->CreateEntity("PatrolHelper");
	auto& rb2d = m_PatrolHelper.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_PatrolHelper.AddComponent<Stormlight::BoxCollider2DComponent>();
	m_PatrolHelper.AddComponent<Stormlight::MovementComponent>();
	AssignPatrolRoute();

	this->GetScene()->OnRuntimeAddEntity(m_PatrolHelper);

	m_Functions = { Bandit::Patrol, Bandit::Attack };
	m_PhaseTimer.SetTimerDuration(1.5f);
	m_PhaseTimer.StartTimer();
}

void Bandit::OnUpdate(Stormlight::Timestep ts)
{
	if (this->GetComponent<Stormlight::EnemyComponent>().Hooked)
	{
		OnHooked(ts);
	}
	else
	{
		m_Functions[m_FunctionIndex](*this, ts);

		if (m_PhaseTimer.OnUpdate(ts))
		{
			auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
			if (m_FunctionIndex == 0)
			{
				ac.m_FrameDuration = 0.09f;
				m_PhaseTimer.SetTimerDuration(0.45f);
				ac.m_AttackIndex = 0;
				ac.m_AttackTimeElapsed = 0.0f;
			}
			if (m_FunctionIndex == 1)
			{
				ac.m_FrameDuration = 0.05f;
				m_PhaseTimer.SetTimerDuration(1.5f);
				this->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_AttackSubTextures[4];
				Hazel::Audio::Play(m_AudioManager->m_ReleseArrow);
				m_Weapon->StartAttack();
				AssignPatrolRoute();
			}

			m_PhaseTimer.RestartTimer();
			m_FunctionIndex = (m_FunctionIndex + 1) % 2;
		}
	}
		
	this->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

void Bandit::OnDestroy()
{
	Enemy::OnDestroy();
	this->GetScene()->DestroyEntity(m_PatrolHelper);
	delete m_Weapon;
}

void Bandit::Patrol(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkForwardSubTextures);

	auto& move = entity.GetComponent<Stormlight::MovementComponent>();
	auto& transform = entity.GetComponent<Stormlight::TransformComponent>();

	transform.Translation.x += move.XYSpeed.x * ts * move.GetSpeed();
	transform.Translation.y += move.XYSpeed.y * ts * move.GetSpeed();
}

void Bandit::Attack(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_AttackTimeElapsed, ac.m_FrameDuration, ac.m_AttackIndex, ac.m_AttackSubTextures);

	auto& playerTrans = s_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = entity.GetComponent<Stormlight::TransformComponent>();
	glm::vec2 direction = playerTrans.Get2DTranslate() - enemyTrans.Get2DTranslate();
	direction = glm::normalize(direction);

	enemyTrans.Rotation.z = atan2(direction.y, direction.x);
}

void Bandit::AssignPatrolRoute()
{
	auto& move = this->GetComponent<Stormlight::MovementComponent>();
	auto& transform = this->GetComponent<Stormlight::TransformComponent>();
	auto& transformPatrolHelper = m_PatrolHelper.GetComponent<Stormlight::TransformComponent>();
	float x_coord = Stormlight::GetRandomFloat(-8.0f, 8.0f);
	float y_coord = Stormlight::GetRandomFloat(-4.5f, 4.5f);
	move.XYSpeed = glm::vec2(x_coord, y_coord) - glm::vec2(transform.Translation.x, transform.Translation.y);
	move.XYSpeed = glm::normalize(move.XYSpeed);

	transformPatrolHelper.Translation.x = x_coord;
	transformPatrolHelper.Translation.y = y_coord;

	transform.Rotation.z = std::atan2(move.XYSpeed.y, move.XYSpeed.x);
}

void Minotaur::OnCreate()
{
	Enemy::OnCreate();

	m_Functions = { Minotaur::MoveToPlayer, Minotaur::WindUpAttack, Minotaur::Attack, Minotaur::Idle };
	m_PhaseTimer.StartTimer();
}

void Minotaur::OnUpdate(Stormlight::Timestep ts)
{
	if (this->GetComponent<Stormlight::EnemyComponent>().Hooked)
	{
		OnHooked(ts);
		auto& mc = this->GetComponent<Stormlight::MinotaurComponent>();
		auto& enemyTrans = this->GetComponent<Stormlight::TransformComponent>();
		auto& weaponTrans = mc.Axe.GetComponent<Stormlight::TransformComponent>();
		weaponTrans.Translation.x = enemyTrans.Translation.x + cos(enemyTrans.Rotation.z - 1.047f) * 0.8f;
		weaponTrans.Translation.y = enemyTrans.Translation.y + sin(enemyTrans.Rotation.z - 1.047f) * 0.8f;
	}
	else
	{
		m_Functions[this->GetComponent<Stormlight::MinotaurComponent>().PhaseIndex](*this, ts);
	}
		
	this->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

void Minotaur::OnDestroy()
{
	Enemy::OnDestroy();
	auto& mc = this->GetComponent<Stormlight::MinotaurComponent>();
	this->GetScene()->DestroyEntity(mc.Axe);
}

void Minotaur::MoveToPlayer(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& mc = entity.GetComponent<Stormlight::MinotaurComponent>();
	mc.AnimationParametar += ts * 2.0f;
	auto& playerTrans = s_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = entity.GetComponent<Stormlight::TransformComponent>();
	glm::vec2 direction = playerTrans.Get2DTranslate() - enemyTrans.Get2DTranslate();
	direction = glm::normalize(direction);

	float rotationAngle = atan2(direction.y, direction.x);
	enemyTrans.Translation.x += direction.x * ts * 3.0f;
	enemyTrans.Translation.y += direction.y * ts * 3.0f;

	enemyTrans.Rotation.z = rotationAngle;

	auto& weaponTrans = mc.Axe.GetComponent<Stormlight::TransformComponent>();
	weaponTrans.Translation.x = enemyTrans.Translation.x + cos(enemyTrans.Rotation.z - 1.047f) * 0.8f;
	weaponTrans.Translation.y = enemyTrans.Translation.y + sin(enemyTrans.Rotation.z - 1.047f) * 0.8f;
	weaponTrans.Rotation.z = enemyTrans.Rotation.z + 0.3333333333f * sin(1.5f * mc.AnimationParametar) + 0.349f;

	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkForwardSubTextures);

	entity.GetComponent<Stormlight::BoxCollider2DComponent>().HandleCollision(entity, ts);

	if (entity.GetScene()->CheckCollision(entity, s_MainPlayer))
	{
		mc.PhaseIndex = (mc.PhaseIndex + 1) % 4;
		mc.AnimationParametar = 0.0f;
		weaponTrans.Translation.x = enemyTrans.Translation.x + cos(enemyTrans.Rotation.z - 0.047f) * 0.6f;
		weaponTrans.Translation.y = enemyTrans.Translation.y + sin(enemyTrans.Rotation.z - 0.047f) * 0.6f;
		mc.AttackWindUpAnimationTimer.StartTimer();
	}
}

void Minotaur::WindUpAttack(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& mc = entity.GetComponent<Stormlight::MinotaurComponent>();
	mc.AnimationParametar += ts * 10;
	auto& enemyTrans = entity.GetComponent<Stormlight::TransformComponent>();
	auto& weaponTrans = mc.Axe.GetComponent<Stormlight::TransformComponent>();

	weaponTrans.Rotation.z = enemyTrans.Rotation.z + mc.AnimationParametar + 0.349f;

	if (mc.AttackWindUpAnimationTimer.OnUpdate(ts))
	{
		mc.AttackWindUpAnimationTimer.ResetTimer();
		mc.PhaseIndex = (mc.PhaseIndex + 1) % 4;
		mc.AnimationParametar = 0.0f;

		weaponTrans.Rotation.z = enemyTrans.Rotation.z + 3.14f / 0.2f;
		auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
		entity.GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleFireSubTextures[0];
		mc.AttackAnimationTimer.StartTimer();
	}
}

void Minotaur::Attack(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& mc = entity.GetComponent<Stormlight::MinotaurComponent>();
	auto& bodyTrans = entity.GetComponent<Stormlight::TransformComponent>();
	auto& weaponTrans = mc.Axe.GetComponent<Stormlight::TransformComponent>();

	bodyTrans.Rotation.z += ts * 6.0f;
	weaponTrans.Rotation.z += ts * 6.0f;

	weaponTrans.Translation.x = bodyTrans.Translation.x + cos(bodyTrans.Rotation.z) * 0.55f;
	weaponTrans.Translation.y = bodyTrans.Translation.y + sin(bodyTrans.Rotation.z) * 0.55f;

	if (!entity.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision && entity.GetScene()->CheckCollision(mc.Axe, s_MainPlayer))
	{
		auto& hp = s_MainPlayer.GetComponent<Stormlight::HealthComponent>();
		hp.health -= entity.GetComponent<Stormlight::EnemyComponent>().AttackDamage;
		hp.UpdateFullHealthBar();

		entity.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = true;
	}

	if (mc.AttackAnimationTimer.OnUpdate(ts))
	{
		mc.AttackAnimationTimer.ResetTimer();
		mc.PhaseIndex = (mc.PhaseIndex + 1) % 4;
		mc.IdleAnimationTimer.StartTimer();

		entity.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = false;
		weaponTrans.Rotation.z = bodyTrans.Rotation.z + 3.14f / 0.2f;
		weaponTrans.Translation.x = bodyTrans.Translation.x + cos(bodyTrans.Rotation.z) * 0.55f;
		weaponTrans.Translation.y = bodyTrans.Translation.y + sin(bodyTrans.Rotation.z) * 0.55f;
	}
}

void Minotaur::Idle(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleSubTextures);

	auto& mc = entity.GetComponent<Stormlight::MinotaurComponent>();
	if (mc.IdleAnimationTimer.OnUpdate(ts))
	{
		mc.IdleAnimationTimer.ResetTimer();
		mc.PhaseIndex = (mc.PhaseIndex + 1) % 4;
	}
}

void Fairy::OnCreate()
{
	Enemy::OnCreate();

	m_ProjectileDirections = { { 0.97f, 0.26f }, { 1.0f ,  0.0f }, { 0.97f, -0.26f } };

	m_EnergyProjectileTexture = Stormlight::Texture2D::Create("assets/game/textures/EnergyProjectile.png");
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	this->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleSubTextures[0];
	m_Functions = { Fairy::Idle,  Fairy::Fade, Fairy::Attack };
	m_PhaseTimer.StartTimer();
}

void Fairy::OnUpdate(Stormlight::Timestep ts)
{
	if (this->GetComponent<Stormlight::EnemyComponent>().Hooked)
	{
		OnHooked(ts);
	}
	else
	{
		auto& playerTrans = s_MainPlayer.GetComponent<Stormlight::TransformComponent>();
		auto& fairyTrans = this->GetComponent<Stormlight::TransformComponent>();
		glm::vec2 direction = playerTrans.Get2DTranslate() - fairyTrans.Get2DTranslate();
		direction = glm::normalize(direction);
		fairyTrans.Rotation.z = atan2(direction.y, direction.x);
		m_Functions[m_FunctionIndex](*this, ts);

		if (m_PhaseTimer.OnUpdate(ts))
		{
			auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
			if (m_FunctionIndex == 0)
			{
				m_PhaseTimer.SetTimerDuration(0.5f);
				ac.m_FrameDuration = 0.1f;
				ac.m_IdleIndex = 0;
				ac.m_IdleTimeElapsed = 0.0f;
				this->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleFireSubTextures[0];
			}
			else if (m_FunctionIndex == 1)
			{
				ac.m_AttackIndex = 0;
				ac.m_AttackTimeElapsed = 0.0f;
				ac.m_FrameDuration = 0.07f;
				fairyTrans.Scale.x = 1.3f;
				fairyTrans.Translation.x += direction.x * 0.15f;
				fairyTrans.Translation.y += direction.y * 0.15f;
				m_PhaseTimer.SetTimerDuration(0.7f);
				Teleport();
				Hazel::Audio::Play(m_AudioManager->m_Fade);
			}
			else if (m_FunctionIndex == 2)
			{
				m_PhaseTimer.SetTimerDuration(1.2f);
				ac.m_FrameDuration = 0.15f;
				ac.m_IdleIndex = 0;
				ac.m_IdleTimeElapsed = 0.0f;
				fairyTrans.Scale.x = 1.0f;
				fairyTrans.Translation.x -= direction.x * 0.15f;
				fairyTrans.Translation.y -= direction.y * 0.15f;
				this->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleSubTextures[0];
				Fire(direction);
			}

			m_PhaseTimer.RestartTimer();
			m_FunctionIndex = (m_FunctionIndex + 1) % 3;
			this->GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = false;
		}
	}
	
	this->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

void Fairy::OnDestroy()
{
	Enemy::OnDestroy();
}

void Fairy::Attack(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_AttackTimeElapsed, ac.m_FrameDuration, ac.m_AttackIndex, ac.m_AttackSubTextures);
}

void Fairy::Idle(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleSubTextures);
}

void Fairy::Fade(Stormlight::Entity& entity, Stormlight::Timestep ts)
{
	auto& ac = entity.GetComponent<Stormlight::AnimationComponent>();
	Animation(&entity, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleFireSubTextures);

	if (entity.GetScene()->CheckCollision(entity, s_MainPlayer))
	{
		auto& hp = s_MainPlayer.GetComponent<Stormlight::HealthComponent>();
		hp.health -= 0.5f;
		hp.UpdateFullHealthBar();
	}
}

void Fairy::Teleport()
{
	float x_coord = Stormlight::GetRandomFloat(-8.0f, 8.0f);
	float y_coord = Stormlight::GetRandomFloat(-4.5f, 4.5f);

	auto& enemyTrans = this->GetComponent<Stormlight::TransformComponent>();
	enemyTrans.Translation.x = x_coord;
	enemyTrans.Translation.y = y_coord;
}

void Fairy::Fire(glm::vec2 direction)
{
	float randomNum = Stormlight::GetRandomFloat();

	if (randomNum < 0.5f)
		CreateProjectile(direction);
	else
	{
		float rotationAngle = atan2(direction.y, direction.x);
		glm::vec2 projDirection;
		for (int i = 0; i < 3; i++)
		{
			projDirection.x = m_ProjectileDirections[i].x * cos(rotationAngle) - m_ProjectileDirections[i].y * sin(rotationAngle);
			projDirection.y = m_ProjectileDirections[i].x * sin(rotationAngle) + m_ProjectileDirections[i].y * cos(rotationAngle);
			CreateProjectile(projDirection);
		}
	}
}

void Fairy::CreateProjectile(glm::vec2 direction)
{
	Hazel::Audio::Play(m_AudioManager->m_ReleseEnergyBall);
	auto& enemyTrans = this->GetComponent<Stormlight::TransformComponent>();

	Stormlight::Entity Projectile = { this->GetScene()->CreateEntity("Projectile"), this->GetScene() };

	auto& mc = Projectile.AddComponent<Stormlight::MovementComponent>();
	mc.XYSpeed.x = direction.x;
	mc.XYSpeed.y = direction.y;

	Projectile.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EnergyProjectileTexture;
	auto& rb2d = Projectile.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	Projectile.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform = Projectile.GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x = enemyTrans.Translation.x + mc.XYSpeed.x * 0.35f;
	transform.Translation.y = enemyTrans.Translation.y + mc.XYSpeed.y * 0.35f;
	transform.Translation.z = -0.5f - Stormlight::GetRandomFloat(0.0f, 0.1f);

	transform.Scale.x = 0.3f;
	transform.Scale.y = 0.3f;

	this->GetScene()->OnRuntimeAddEntity(Projectile);
}

void FairyBoss::OnCreate()
{
	Enemy::OnCreate();

	m_ProjectileDirections = { { 0.97f, 0.26f }, { 1.0f ,  0.0f }, { 0.97f, -0.26f } };

	m_EnergyProjectileTexture = Stormlight::Texture2D::Create("assets/game/textures/EnergyProjectile.png");
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	this->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleSubTextures[0];
	m_Functions = { Fairy::Attack, Fairy::Fade };
	m_PhaseTimer.SetTimerDuration(0.4f);
	m_PhaseTimer.StartTimer();
}

void FairyBoss::OnUpdate(Stormlight::Timestep ts)
{
	if (this->GetComponent<Stormlight::EnemyComponent>().Hooked)
	{
		OnHooked(ts);
	}
	else
	{
		auto& playerTrans = s_MainPlayer.GetComponent<Stormlight::TransformComponent>();
		auto& fairyTrans = this->GetComponent<Stormlight::TransformComponent>();
		glm::vec2 direction = playerTrans.Get2DTranslate() - fairyTrans.Get2DTranslate();
		direction = glm::normalize(direction);
		fairyTrans.Rotation.z = atan2(direction.y, direction.x);
		m_Functions[m_FunctionIndex](*this, ts);

			if (m_PhaseTimer.OnUpdate(ts))
			{
				auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
				if (m_FunctionIndex == 0)
				{
					ac.m_IdleIndex = 0;
					ac.m_IdleTimeElapsed = 0.0f;
					Fire(direction);
					m_FunctionIndex = (m_FunctionIndex + 1) % 2;
					m_TeleportCounter = 0;
				}
				else if (m_FunctionIndex == 1)
				{

					ac.m_IdleIndex = 0;
					ac.m_IdleTimeElapsed = 0.0f;
					Teleport();
					Hazel::Audio::Play(m_AudioManager->m_Fade);
					m_Teleporting = true;
					if (m_TeleportCounter == 2)
					{
						ac.m_AttackIndex = 0;
						ac.m_AttackTimeElapsed = 0.0f;
						m_FunctionIndex = (m_FunctionIndex + 1) % 2;
					}
					m_TeleportCounter++;
				}

			m_PhaseTimer.RestartTimer();
			
			this->GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = false;
		}
	}

	this->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

void FairyBoss::Fire(glm::vec2 direction)
{
	float rotationAngle = atan2(direction.y, direction.x);
	glm::vec2 projDirection;
	for (int i = 0; i < 3; i++)
	{
		projDirection.x = m_ProjectileDirections[i].x * cos(rotationAngle) - m_ProjectileDirections[i].y * sin(rotationAngle);
		projDirection.y = m_ProjectileDirections[i].x * sin(rotationAngle) + m_ProjectileDirections[i].y * cos(rotationAngle);
		CreateProjectile(projDirection);
	}
}

void MinotaurSG::OnUpdate(Stormlight::Timestep ts)
{
	if (this->GetComponent<Stormlight::EnemyComponent>().Hooked)
	{
		OnHooked(ts);
	}
	else
	{
		auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
		Animation(this, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleSubTextures);
	}

	this->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}
