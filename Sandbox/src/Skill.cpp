#include "Skill.h"
#include "Animation.h"

Blast::Blast(Stormlight::Scene* gameScene, AudioManager* audioManager)
	: Skill(gameScene, audioManager)
{
	m_EnergyBallTexture = Stormlight::Texture2D::Create("assets/game/textures/SkillBlast.png");

	m_BallSpinSubTextureVec.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_EnergyBallTexture, { 0.0f, 0.0f }, { 0.25f, 1.0f }));
	m_BallSpinSubTextureVec.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_EnergyBallTexture, { 0.25f, 0.0f }, { 0.5f, 1.0f }));
	m_BallSpinSubTextureVec.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_EnergyBallTexture, { 0.5f, 0.0f }, { 0.75f, 1.0f }));
	m_BallSpinSubTextureVec.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_EnergyBallTexture, { 0.75f, 0.0f }, { 1.0f, 1.0f }));

	m_EnergyBallDirections =
	{
		glm::vec2( 1.0f,  0.0f),
		glm::vec2( 0.7f,  0.7f),
		glm::vec2( 0.0f,  1.0f),
		glm::vec2(-0.7f,  0.7f),
		glm::vec2(-1.0f,  0.0f),
		glm::vec2(-0.7f, -0.7f),
		glm::vec2( 0.0f, -1.0f),
		glm::vec2( 0.7f, -0.7f)
	};
}

void Blast::Use()
{
	Hazel::Audio::Play(m_AudioManager->m_ReleseEnergyBall);
	m_IsActive = true;
	m_CooldownTimer.SetTimerDuration(6.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().CooldownReduction);
	m_CooldownTimer.StartTimer();
	auto player = m_GameScene->GetAllWithTag("MainCharacter")[0];
	auto& playerTrans = player.GetComponent<Stormlight::TransformComponent>();

	for (int i = 0; i < 8; i++)
	{
		Stormlight::Entity energyBall = { m_GameScene->CreateEntity("EnergyBall"), m_GameScene };

		auto& mc = energyBall.AddComponent<Stormlight::MovementComponent>();
		mc.XYSpeed = m_EnergyBallDirections[i];

		auto& src = energyBall.AddComponent<Stormlight::SpriteRendererComponent>();
		src.subTexture = m_BallSpinSubTextureVec[0];
		auto& rb2d = energyBall.AddComponent<Stormlight::Rigidbody2DComponent>();
		rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
		energyBall.AddComponent<Stormlight::BoxCollider2DComponent>();

		auto& transform = energyBall.GetComponent<Stormlight::TransformComponent>();
		transform.Translation.x = playerTrans.Translation.x + mc.XYSpeed.x * 0.5f;
		transform.Translation.y = playerTrans.Translation.y + mc.XYSpeed.y * 0.5f;
		transform.Translation.z = 0.2f;

		transform.Scale.x = 0.5f;
		transform.Scale.y = 0.5f;

		m_GameScene->OnRuntimeAddEntity(energyBall);
	}
}

void Blast::Update(Stormlight::Timestep ts)
{
	Skill::Update(ts);
	std::vector<Stormlight::Entity> everyEnergyBall = m_GameScene->GetAllWithTag("EnergyBall");
	std::vector<Stormlight::Entity> everyEnemy = m_GameScene->GetAllWithTag("Enemy");

	for (auto energyBall : everyEnergyBall)
	{
		auto xySpeed = energyBall.GetComponent<Stormlight::MovementComponent>().XYSpeed;
		auto& transform = energyBall.GetComponent<Stormlight::TransformComponent>();
		transform.Translation.x += xySpeed.x * ts * 5.0f;
		transform.Translation.y += xySpeed.y * ts * 5.0f;

		Animation(&energyBall, ts, m_SpinTimeElapsed, m_FrameDuration, m_SpinIndex, m_BallSpinSubTextureVec);

		for (auto enemy : everyEnemy)
		{
			if (m_GameScene->CheckCollision(enemy, energyBall))
			{
				Hazel::Audio::Play(m_AudioManager->m_ProjectileImpact);
				m_GameScene->DestroyEntity(energyBall);

				auto& hp = enemy.GetComponent<Stormlight::HealthComponent>();
				hp.health -= 130.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().MagicDamage;
				hp.UpdateFullHealthBar();
				break;
			}
		}
	}
}

void Blast::OnDestroy()
{
	m_IsActive = false;
	m_CooldownTimer.ResetTimer();
	for (auto ent : m_GameScene->GetAllWithTag("EnergyBall"))
		m_GameScene->DestroyEntity(ent);
}

KnifeThrow::KnifeThrow(Stormlight::Scene* gameScene, AudioManager* audioManager) : Skill(gameScene, audioManager)
{
	m_KnifeTexture = Stormlight::Texture2D::Create("assets/game/textures/knife.png");

	m_KnifeDirections =
	{
		{0.97f,  0.26f},
		{0.99f,  0.13f},
		{1.0f ,  0.0f },
		{0.99f, -0.13f},
		{0.97f, -0.26f}
	};
}

void KnifeThrow::Use()
{
	Hazel::Audio::Play(m_AudioManager->m_KnivesRelease);
	m_IsActive = true;
	m_CooldownTimer.SetTimerDuration(4.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().CooldownReduction);
	m_CooldownTimer.StartTimer();

	auto player = m_GameScene->GetAllWithTag("MainCharacter")[0];
	auto& playerTrans = player.GetComponent<Stormlight::TransformComponent>();

	auto rotationAngle = playerTrans.Rotation.z;

	for (int i = 0; i < m_NumberOfKnives; i++)
	{
		Stormlight::Entity knife = { m_GameScene->CreateEntity("Knife"), m_GameScene };

		auto& mc = knife.AddComponent<Stormlight::MovementComponent>();
		mc.XYSpeed.x = m_KnifeDirections[i].x * cos(rotationAngle) - m_KnifeDirections[i].y * sin(rotationAngle);
		mc.XYSpeed.y = m_KnifeDirections[i].x * sin(rotationAngle) + m_KnifeDirections[i].y * cos(rotationAngle);

		knife.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_KnifeTexture;
		auto& rb2d = knife.AddComponent<Stormlight::Rigidbody2DComponent>();
		rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
		knife.AddComponent<Stormlight::BoxCollider2DComponent>();

		auto& transform = knife.GetComponent<Stormlight::TransformComponent>();
		transform.Translation.x = playerTrans.Translation.x + mc.XYSpeed.x * 0.3f;
		transform.Translation.y = playerTrans.Translation.y + mc.XYSpeed.y * 0.3f;
		transform.Translation.z = 0.2f + (i / 5.0f) * 0.2f;

		transform.Scale.x = 0.6f;
		transform.Scale.y = 0.2f;

		transform.Rotation.z = rotationAngle;

		m_GameScene->OnRuntimeAddEntity(knife);
	}
}

void KnifeThrow::Update(Stormlight::Timestep ts)
{
	Skill::Update(ts);
	std::vector<Stormlight::Entity> everyKnife = m_GameScene->GetAllWithTag("Knife");
	std::vector<Stormlight::Entity> everyEnemy = m_GameScene->GetAllWithTag("Enemy");

	for (auto& knife : everyKnife)
	{
		auto& mc = knife.GetComponent<Stormlight::MovementComponent>();
		auto& transform = knife.GetComponent<Stormlight::TransformComponent>();
		transform.Translation.x += mc.XYSpeed.x * 5.0f * ts;
		transform.Translation.y += mc.XYSpeed.y * 5.0f * ts;

		for (auto enemy : everyEnemy)
		{
			if (m_GameScene->CheckCollision(enemy, knife))
			{
				Hazel::Audio::Play(m_AudioManager->m_ProjectileImpact);
				auto& hp = enemy.GetComponent<Stormlight::HealthComponent>();
				hp.health -= 57.2f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().MagicDamage;
				hp.UpdateFullHealthBar();

				m_GameScene->DestroyEntity(knife);
				break;
			}
		}
	}
}

void KnifeThrow::OnDestroy()
{
	m_IsActive = false;
	m_CooldownTimer.ResetTimer();
	for (auto& knife : m_GameScene->GetAllWithTag("Knife"))
	{
		m_GameScene->DestroyEntity(knife);
	}
}

void Skill::Update(Stormlight::Timestep ts)
{
	if (m_CooldownTimer.OnUpdate(ts))
	{
		Hazel::Audio::Play(m_AudioManager->m_SkillCooldownUp);
		OnDestroy();
	}
}

Hook::Hook(Stormlight::Scene* gameScene, AudioManager* audioManager) : Skill(gameScene, audioManager)
{
	m_HookHeadTexture = Stormlight::Texture2D::Create("assets/game/textures/hook.png");
	m_HookHeadReturnTexture = Stormlight::Texture2D::Create("assets/game/textures/hookReturn.png");
	m_HookChainTexture = Stormlight::Texture2D::Create("assets/game/textures/chain.png");
	m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0];
}

void Hook::Use()
{
	Hazel::Audio::Play(m_AudioManager->m_HookRelease);
	m_IsActive = true;
	m_CooldownTimer.SetTimerDuration(6.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().CooldownReduction);
	m_CooldownTimer.StartTimer();
	
	auto& playerTrans = m_MainPlayer.GetComponent<Stormlight::TransformComponent>();
	float rotationAngle = playerTrans.Rotation.z;

	glm::vec2 direction;	
	direction.x = 1.0f * cos(rotationAngle) - 0.0f * sin(rotationAngle);
	direction.y = 1.0f * sin(rotationAngle) + 0.0f * cos(rotationAngle);
	direction = glm::normalize(direction);
	m_HookHead = m_GameScene->CreateEntity("Hook");

	auto& mc = m_HookHead.AddComponent<Stormlight::MovementComponent>();
	mc.XYSpeed.x = direction.x;
	mc.XYSpeed.y = direction.y;

	m_HookHead.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_HookHeadTexture;
	auto& rb2d = m_HookHead.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_HookHead.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& headTrans = m_HookHead.GetComponent<Stormlight::TransformComponent>();
	headTrans.Translation.x = playerTrans.Translation.x + mc.XYSpeed.x * 0.3f;
	headTrans.Translation.y = playerTrans.Translation.y + mc.XYSpeed.y * 0.3f;
	headTrans.Translation.z = 0.1f;

	headTrans.Scale.x = 0.8f;
	headTrans.Scale.y = 0.6f;

	headTrans.Rotation.z = rotationAngle;

	m_GameScene->OnRuntimeAddEntity(m_HookHead);

	m_HookChain = m_GameScene->CreateEntity("Hook");

	auto& mcChain = m_HookChain.AddComponent<Stormlight::MovementComponent>();
	mcChain.XYSpeed.x = direction.x;
	mcChain.XYSpeed.y = direction.y;

	m_HookChain.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_HookChainTexture;
	auto& rb2dChain = m_HookChain.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dChain.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_HookChain.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& chainTrans = m_HookChain.GetComponent<Stormlight::TransformComponent>();
	chainTrans.Translation.x = playerTrans.Translation.x + mc.XYSpeed.x * 0.3f;
	chainTrans.Translation.y = playerTrans.Translation.y + mc.XYSpeed.y * 0.3f;
	chainTrans.Translation.z = 0.09f;

	chainTrans.Scale.x = 0.01f;
	chainTrans.Scale.y = 0.15f;

	chainTrans.Rotation.z = rotationAngle;

	m_GameScene->OnRuntimeAddEntity(m_HookChain);
}

void Hook::Update(Stormlight::Timestep ts)
{
	if (!m_Hooked && m_CooldownTimer.GetElapsedTimerDuration() < 0.8f)
	{
		auto& playerTrans = m_MainPlayer.GetComponent<Stormlight::TransformComponent>();
		auto& mc = m_HookHead.GetComponent<Stormlight::MovementComponent>();
		auto& headTrans = m_HookHead.GetComponent<Stormlight::TransformComponent>();
		headTrans.Translation.x += mc.XYSpeed.x * 10.0f * ts;
		headTrans.Translation.y += mc.XYSpeed.y * 10.0f * ts;

		glm::vec2 delta = { headTrans.Translation.x - playerTrans.Translation.x, headTrans.Translation.y - playerTrans.Translation.y };
		float rotationAngle = std::atan2(delta.y, delta.x);
		headTrans.Rotation.z = rotationAngle;

		auto& chainTrans = m_HookChain.GetComponent<Stormlight::TransformComponent>();
		chainTrans.Translation.x = playerTrans.Translation.x + (headTrans.Translation.x - playerTrans.Translation.x) / 2.0f;
		chainTrans.Translation.y = playerTrans.Translation.y + (headTrans.Translation.y - playerTrans.Translation.y) / 2.0f;
		chainTrans.Scale.x = glm::sqrt(glm::pow((headTrans.Translation.x - playerTrans.Translation.x), 2.0f) + glm::pow((headTrans.Translation.y - playerTrans.Translation.y), 2.0f));
		chainTrans.Rotation.z = rotationAngle;

		for (auto enemyEnt : m_GameScene->Reg().view<Stormlight::EnemyComponent>())
		{
			Stormlight::Entity enemy = { enemyEnt, m_GameScene };
			if (m_GameScene->CheckCollision(enemy, m_HookHead))
			{
				Hazel::Audio::Stop(m_AudioManager->m_HookRelease);
				Hazel::Audio::Play(m_AudioManager->m_HookImapct);
				m_Hooked = true;
				enemy.GetComponent<Stormlight::EnemyComponent>().Hooked = true;
				auto& hp = enemy.GetComponent<Stormlight::HealthComponent>();
				hp.health -= 130.0f * m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().MagicDamage;
				hp.UpdateFullHealthBar();
			}
		}
	}
	else if (!m_Finished)
	{
		auto& playerTrans = m_MainPlayer.GetComponent<Stormlight::TransformComponent>();
		auto& headTrans = m_HookHead.GetComponent<Stormlight::TransformComponent>();
		m_HookHead.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_HookHeadReturnTexture;
		glm::vec2 direction;
		direction.x = headTrans.Translation.x - playerTrans.Translation.x;
		direction.y = headTrans.Translation.y - playerTrans.Translation.y;
		direction = glm::normalize(direction);

		headTrans.Translation.x -= direction.x * 10.0f * ts;
		headTrans.Translation.y -= direction.y * 10.0f * ts;

		float rotationAngle = std::atan2(direction.y, direction.x);
		headTrans.Rotation.z = rotationAngle;

		auto& chainTrans = m_HookChain.GetComponent<Stormlight::TransformComponent>();
		chainTrans.Translation.x = playerTrans.Translation.x + (headTrans.Translation.x - playerTrans.Translation.x) / 2.0f;
		chainTrans.Translation.y = playerTrans.Translation.y + (headTrans.Translation.y - playerTrans.Translation.y) / 2.0f;
		chainTrans.Scale.x = glm::sqrt(glm::pow((headTrans.Translation.x - playerTrans.Translation.x), 2.0f) + glm::pow((headTrans.Translation.y - playerTrans.Translation.y), 2.0f));
		chainTrans.Rotation.z = rotationAngle;

		if (m_GameScene->CheckCollision(m_MainPlayer, m_HookHead))
		{
			Hazel::Audio::Stop(m_AudioManager->m_HookRelease);
			m_Finished = true;
			for (auto ent : m_GameScene->GetAllWithTag("Hook"))
				m_GameScene->DestroyEntity(ent);
		}
	}
	Skill::Update(ts);
}

void Hook::OnDestroy()
{
	Hazel::Audio::Stop(m_AudioManager->m_HookRelease);
	m_IsActive = false;
	m_Finished = false;
	m_Hooked = false;
	m_CooldownTimer.ResetTimer();
	for (auto ent : m_GameScene->GetAllWithTag("Hook"))
		m_GameScene->DestroyEntity(ent);
}
