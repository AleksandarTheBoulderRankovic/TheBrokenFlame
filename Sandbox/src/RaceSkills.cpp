#include "RaceSkills.h"
#include "Skill.h"
#include "Animation.h"

Peasant::Peasant(Stormlight::Scene* gameScene, AudioManager* audioManager) : Skill(gameScene, audioManager)
{
	m_PeasantWeapon = Stormlight::Texture2D::Create("assets/game/textures/PeasantDagger.png");
	auto PeasantSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/PeasantSpriteSheet.png");

	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.0f, 0.0f }, { 0.2f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.2f, 0.0f }, { 0.4f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.4f, 0.0f }, { 0.6f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.6f, 0.0f }, { 0.8f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.8f, 0.0f }, { 1.0f, 0.25f }));

	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.6f, 0.0f }, { 0.8f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.4f, 0.0f }, { 0.6f, 0.25f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.2f, 0.0f }, { 0.4f, 0.25f }));

	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.0f, 0.25f }, { 0.2f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.2f, 0.25f }, { 0.4f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.4f, 0.25f }, { 0.6f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.6f, 0.25f }, { 0.8f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.8f, 0.25f }, { 1.0f, 0.5f }));

	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.6f, 0.25f }, { 0.8f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.4f, 0.25f }, { 0.6f, 0.5f }));
	m_WalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.2f, 0.25f }, { 0.4f, 0.5f }));

	m_AttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(PeasantSpriteSheet, { 0.0f, 0.5f }, { 0.2f, 0.75f }));

	m_CooldownTimer.SetTimerDuration(6.0f);
}

void Peasant::Use()
{
	Hazel::Audio::Play(m_AudioManager->m_PeasantArrive);
	m_IsActive = true;
	m_CooldownTimer.SetTimerDuration(6.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().CooldownReduction);
	m_CooldownTimer.StartTimer();

	Stormlight::Entity peasant = { m_GameScene->CreateEntity("Peasant"), m_GameScene };

	peasant.AddComponent<Stormlight::MovementComponent>();
	auto& hp = peasant.AddComponent<Stormlight::HealthComponent>();
	hp.owner = peasant;
	hp.CreateHealthBars();

	peasant.AddComponent<Stormlight::SpriteRendererComponent>().subTexture = m_AttackSubTextures[0];
	auto& ac = peasant.AddComponent<Stormlight::AnimationComponent>();
	ac.m_WalkForwardSubTextures = m_WalkSubTextures;
	ac.m_AttackSubTextures = m_AttackSubTextures;
	ac.m_FrameDuration = 0.062f;
	auto& rb2d = peasant.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	peasant.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto mouseRelativePosition = Stormlight::Input::GetRelativeMousePosition();
	auto& transform = peasant.GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x = mouseRelativePosition.first;
	transform.Translation.y = mouseRelativePosition.second;
	transform.Translation.z = offset;
	offset -= 0.002f;

	m_GameScene->OnRuntimeAddEntity(peasant);

	Stormlight::Entity m_PeasantDagger = m_GameScene->CreateEntity("PeasantDagger");
	auto& rb2dDagger = m_PeasantDagger.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dDagger.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_PeasantDagger.AddComponent<Stormlight::BoxCollider2DComponent>();
	m_PeasantDagger.AddComponent<Stormlight::MovementComponent>();
	m_PeasantDagger.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_PeasantWeapon;

	auto& weaponTrans = m_PeasantDagger.GetComponent<Stormlight::TransformComponent>();
	weaponTrans.Translation.z = transform.Translation.z - 0.001f;
	weaponTrans.Scale.x = 0.5f;
	weaponTrans.Scale.y = 0.3f;

	m_GameScene->OnRuntimeAddEntity(m_PeasantDagger);

	auto& pc = peasant.AddComponent<Stormlight::PeasantComponent>();
	pc.Dagger = m_PeasantDagger;
	pc.PhaseTimer.StartTimer();
}

void Peasant::Update(Stormlight::Timestep ts)
{
	auto enemyView = m_GameScene->GetAllWithTag("Enemy");
	if (!enemyView.empty())
	{
		for (auto peasant : m_GameScene->Reg().view<Stormlight::PeasantComponent>())
		{
			Stormlight::Entity currentPeasant = { peasant, m_GameScene };
			auto& hp = currentPeasant.GetComponent<Stormlight::HealthComponent>();
			if (hp.health <= 0.0f)
			{
				Hazel::Audio::Play(m_AudioManager->m_Death);
				hp.DestroyHealthBars();
				m_GameScene->DestroyEntity(currentPeasant.GetComponent<Stormlight::PeasantComponent>().Dagger);
				m_GameScene->DestroyEntity(currentPeasant);
				continue;
			}
			hp.UpdateHealthBars();
			Stormlight::Entity Enemy = NearestEnemy(currentPeasant);
			currentPeasant.GetComponent<Stormlight::BoxCollider2DComponent>().FindCollision(currentPeasant, "Peasant");
			UpdatePeasant(currentPeasant, Enemy, ts);
		}
	}
	if (m_CooldownTimer.OnUpdate(ts))
	{
		m_CooldownTimer.ResetTimer();
		m_IsActive = false;
	}
}

void Peasant::OnDestroy()
{
	m_IsActive = false;
	for (auto peasant : m_GameScene->GetAllWithTag("Peasant"))
	{
		peasant.GetComponent<Stormlight::HealthComponent>().DestroyHealthBars();
		m_GameScene->DestroyEntity(peasant.GetComponent<Stormlight::PeasantComponent>().Dagger);
		m_GameScene->DestroyEntity(peasant);
	}
}

void Peasant::UpdatePeasant(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts)
{
	auto& pc = peasant.GetComponent<Stormlight::PeasantComponent>();
	
	if (m_GameScene->CheckCollision(peasant, enemy))
	{
		OnAttack(peasant, enemy, ts);
	}
	else
		OnMove(peasant, enemy, ts);

	if (pc.PhaseTimer.OnUpdate(ts))
	{
		pc.PhaseTimer.RestartTimer();
		pc.AnimationParametar = 0.0f;
		peasant.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = false;
	}
}

void Peasant::OnAttack(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts)
{
	auto& ac = peasant.GetComponent<Stormlight::AnimationComponent>();
	Animation(&peasant, ts, ac.m_AttackTimeElapsed, ac.m_FrameDuration, ac.m_AttackIndex, ac.m_AttackSubTextures);

	auto& pc = peasant.GetComponent<Stormlight::PeasantComponent>();
	pc.AnimationParametar += ts;
	auto& daggerTrans = pc.Dagger.GetComponent<Stormlight::TransformComponent>();
	auto& peasantTrans = peasant.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = enemy.GetComponent<Stormlight::TransformComponent>();

	glm::vec2 direction = enemyTrans.Get2DTranslate() - peasantTrans.Get2DTranslate();
	direction = glm::normalize(direction);

	peasantTrans.Rotation.z = atan2(direction.y, direction.x);

	daggerTrans.Translation.x = peasantTrans.Translation.x + cos(peasantTrans.Rotation.z - 0.9f * sin(pc.AnimationParametar * 3.0f) - 1.0f) * 0.7f;
	daggerTrans.Translation.y = peasantTrans.Translation.y + sin(peasantTrans.Rotation.z - 0.9f * sin(pc.AnimationParametar * 3.0f) - 1.0f) * 0.7f;
	daggerTrans.Rotation.z = peasantTrans.Rotation.z;
	if (!peasant.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision && m_GameScene->CheckCollision(pc.Dagger, enemy))
	{
		Hazel::Audio::Play(m_AudioManager->m_DaggerImpact);
		auto& hp = enemy.GetComponent<Stormlight::HealthComponent>();
		hp.health -= pc.AttackDamage;
		hp.UpdateFullHealthBar();

		peasant.GetComponent<Stormlight::BoxCollider2DComponent>().NoCollision = true;
	}
}

void Peasant::OnMove(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts)
{
	auto& ac = peasant.GetComponent<Stormlight::AnimationComponent>();
	Animation(&peasant, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkForwardSubTextures);

	auto& peasantTrans = peasant.GetComponent<Stormlight::TransformComponent>();
	auto& enemyTrans = enemy.GetComponent<Stormlight::TransformComponent>();

	glm::vec2 direction = enemyTrans.Get2DTranslate() - peasantTrans.Get2DTranslate();
	direction = glm::normalize(direction);

	float rotationAngle = atan2(direction.y, direction.x);

	peasant.GetComponent<Stormlight::BoxCollider2DComponent>().HandleCollision(peasant, ts);

	peasantTrans.Translation.x += direction.x * ts * 3.0f;
	peasantTrans.Translation.y += direction.y * ts * 3.0f;

	peasantTrans.Rotation.z = rotationAngle;
	auto& pc = peasant.GetComponent<Stormlight::PeasantComponent>();
	pc.AnimationParametar += ts;
	Stormlight::Entity& dagger = pc.Dagger;
	auto& daggerTrans = dagger.GetComponent<Stormlight::TransformComponent>();

	daggerTrans.Translation.x = peasantTrans.Translation.x + 0.7 * cos(peasantTrans.Rotation.z - 1.5f - 0.3f * sin(pc.AnimationParametar * 6.0f)) * 0.9f;
	daggerTrans.Translation.y = peasantTrans.Translation.y + 0.7 * sin(peasantTrans.Rotation.z - 1.5f - 0.3f * sin(pc.AnimationParametar * 6.0f)) * 0.9f;

	daggerTrans.Rotation.z = peasantTrans.Rotation.z;
}

Stormlight::Entity Peasant::NearestEnemy(Stormlight::Entity& peasant)
{
	auto& peasantTrans = peasant.GetComponent<Stormlight::TransformComponent>();
	float minDistance = 20.0f;
	float currentDistance = 0.0f;
	Stormlight::Entity minEnemy;
	
	for (auto enemy : m_GameScene->GetAllWithTag("Enemy"))
	{
		auto& enemyTrans = enemy.GetComponent<Stormlight::TransformComponent>();
		currentDistance = glm::length(peasantTrans.Get2DTranslate() - enemyTrans.Get2DTranslate());
		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			minEnemy = enemy;
		}
	}
	return minEnemy;
}