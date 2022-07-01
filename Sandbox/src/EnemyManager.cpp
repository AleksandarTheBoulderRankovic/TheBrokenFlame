#include "EnemyManager.h"

void EnemyManager::CreateEnemies()
{
	m_EncounterEnemies.clear();
	int numberOfEnemies = Stormlight::GetRandomInt(3, 5);
	float randomNum = 0.0f;
	
	for (int i = 0; i < numberOfEnemies; i++)
	{
		
		randomNum = Stormlight::GetRandomFloat();
		if (randomNum <= 0.33f)
			CreateBandit();
		else if (randomNum <= 0.66f)
			CreateMinotaur();
		else
		{
			glm::vec2 location;
			location.x = Stormlight::GetRandomFloat(0, 7.0f);
			location.y = Stormlight::GetRandomFloat(-4.5f, 4.5f);
			CreateFairy(location);
		}
		
	}
}

bool EnemyManager::UpdateEnemies(Stormlight::Timestep ts)
{
	
	for (int i = 0; i < m_EncounterEnemies.size(); i++)
	{
		m_EncounterEnemies[i]->GetComponent<Stormlight::BoxCollider2DComponent>().FindCollision(*(m_EncounterEnemies[i]), "Enemy");
		m_EncounterEnemies[i]->OnUpdate(ts);
		if (m_EncounterEnemies[i]->GetComponent<Stormlight::HealthComponent>().health <= 0.0f)
		{
			Hazel::Audio::Play(m_AudioManager->m_Death);
			DestroyEnemy(m_EncounterEnemies[i], i);
			if (m_EncounterEnemies.empty())
				return true;
		}
	}
	
	return false;
}

void EnemyManager::DestroyEnemy(Enemy* enemy, int index)
{
	enemy->OnDestroy();
	m_EncounterEnemies.erase(m_EncounterEnemies.begin() + index);
	m_GameScene->DestroyEntity(*enemy);
	delete enemy;
}

void EnemyManager::CreateBandit()
{
	Enemy* bandit = new Bandit(m_GameScene->CreateEntity("Enemy"), m_AudioManager);

	auto& ac = bandit->AddComponent<Stormlight::AnimationComponent>();
	ac.m_WalkForwardSubTextures = m_TextureRepository->m_BanditWalkSubTextures;
	ac.m_AttackSubTextures = m_TextureRepository->m_BanditAttackSubTextures;

	auto& banditTrans = bandit->GetComponent<Stormlight::TransformComponent>();
	banditTrans.Scale.x = 2.0f;
	banditTrans.Scale.y = 2.0f;

	CreateEnemy(bandit);
	ac.m_FrameDuration = 0.05f;
}

void EnemyManager::CreateMinotaur()
{
	Enemy* minotaur = new Minotaur(m_GameScene->CreateEntity("Enemy"), m_AudioManager);

	auto& ac = minotaur->AddComponent<Stormlight::AnimationComponent>();
	ac.m_WalkForwardSubTextures = m_TextureRepository->m_MinotaurWalkForwardSubTextures;
	ac.m_IdleFireSubTextures = m_TextureRepository->m_MinotaurAttackSubTextures;
	ac.m_IdleSubTextures = m_TextureRepository->m_MinotaurIdleSubTextures;
	ac.m_FrameDuration = 0.15f;
	
	CreateEnemy(minotaur);

	auto& minotaurTrans = minotaur->GetComponent<Stormlight::TransformComponent>();
	minotaurTrans.Scale.x = 1.5f;
	minotaurTrans.Scale.y = 1.5f;

	minotaur->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_WalkForwardSubTextures[0];

	Stormlight::Entity m_WeaponAttackWeapon = m_GameScene->CreateEntity("WeaponAttack");
	auto& rb2d = m_WeaponAttackWeapon.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_WeaponAttackWeapon.AddComponent<Stormlight::BoxCollider2DComponent>();
	m_WeaponAttackWeapon.AddComponent<Stormlight::MovementComponent>();
	m_WeaponAttackWeapon.AddComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleFireSubTextures[1];

	auto& weaponTrans = m_WeaponAttackWeapon.GetComponent<Stormlight::TransformComponent>();
	weaponTrans.Translation.x = minotaurTrans.Translation.x + cos(minotaurTrans.Rotation.z - 1.047f) * 0.8f;
	weaponTrans.Translation.y = minotaurTrans.Translation.y + sin(minotaurTrans.Rotation.z - 1.047f) * 0.8f;
	weaponTrans.Scale.x = 3.4f;
	weaponTrans.Scale.y = 1.4f;

	m_GameScene->OnRuntimeAddEntity(m_WeaponAttackWeapon);

	minotaur->AddComponent<Stormlight::MinotaurComponent>().Axe = m_WeaponAttackWeapon;
}

void EnemyManager::CreateMinotaurSG()
{
	m_EncounterEnemies.clear();
	Enemy* minotaur = new MinotaurSG(m_GameScene->CreateEntity("Enemy"));

	auto& ac = minotaur->AddComponent<Stormlight::AnimationComponent>();
	ac.m_IdleSubTextures = m_TextureRepository->m_MinotaurSGIdleSubTextures;
	ac.m_FrameDuration = 0.16f;

	auto& minosTrans = minotaur->GetComponent<Stormlight::TransformComponent>();
	minosTrans.Scale.x = 2.0f;
	minosTrans.Scale.y = 2.0f;

	CreateEnemy(minotaur);

	minosTrans.Rotation.z = -3.14f / 2.0f;
	minosTrans.Translation.x = 4.0f;
	minosTrans.Translation.y = 2.0f;

	minotaur->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleSubTextures[0];
	minotaur->GetComponent<Stormlight::SpriteRendererComponent>().Color = { 1.0f, 0.8f, 0.0f, 1.0f };
	minotaur->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();

	Enemy* minotaur2 = new MinotaurSG(m_GameScene->CreateEntity("Enemy"));

	auto& ac2 = minotaur2->AddComponent<Stormlight::AnimationComponent>();
	ac2.m_IdleSubTextures = m_TextureRepository->m_MinotaurSGIdleSubTextures;
	ac2.m_FrameDuration = 0.17f;

	CreateEnemy(minotaur2);

	auto& minosTrans2 = minotaur2->GetComponent<Stormlight::TransformComponent>();
	minosTrans2.Scale.x = 2.0f;
	minosTrans2.Scale.y = 2.0f;
	minosTrans2.Rotation.z = -3.14f / 2.0f;
	minosTrans2.Translation.x = -4.0f;
	minosTrans2.Translation.y = 2.0f;

	minotaur2->GetComponent<Stormlight::SpriteRendererComponent>().subTexture = ac2.m_IdleSubTextures[0];
	minotaur2->GetComponent<Stormlight::SpriteRendererComponent>().Color = { 1.0f, 0.8f, 0.0f, 1.0f};
	minotaur2->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

void EnemyManager::CreateFairy(glm::vec2 location)
{
	Enemy* fairy = new Fairy(m_GameScene->CreateEntity("Enemy"), m_AudioManager);

	auto& ac = fairy->AddComponent<Stormlight::AnimationComponent>();
	ac.m_IdleSubTextures = m_TextureRepository->m_FairyIdleSubTextures;
	ac.m_AttackSubTextures = m_TextureRepository->m_FairyAttackSubTextures;
	ac.m_IdleFireSubTextures = m_TextureRepository->m_FairyFadeSubTextures;
	ac.m_FrameDuration = 0.15f;

	CreateEnemy(fairy);

	auto& fairyTrans = fairy->GetComponent<Stormlight::TransformComponent>();
	fairyTrans.Scale.y = 1.2f;
	fairyTrans.Translation.x = location.x;
	fairyTrans.Translation.y = location.y;

	fairy->GetComponent<Stormlight::HealthComponent>().UpdateHealthBars();
}

FairyBoss* EnemyManager::CreateBoss(float health)
{
	FairyBoss* boss = new FairyBoss(m_GameScene->CreateEntity("Boss"), m_AudioManager);

	auto& ac = boss->AddComponent<Stormlight::AnimationComponent>();
	ac.m_IdleSubTextures = m_TextureRepository->m_BossIdleSubTextures;
	ac.m_AttackSubTextures = m_TextureRepository->m_BossAttackSubTextures;
	ac.m_IdleFireSubTextures = m_TextureRepository->m_BossFadeSubTextures;
	ac.m_FrameDuration = 0.1f;

	boss->AddComponent<Stormlight::MovementComponent>();
	boss->AddComponent<Stormlight::EnemyComponent>();
	boss->AddComponent<Stormlight::SpriteRendererComponent>();
	boss->AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& rb2dboss = boss->AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dboss.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;

	auto& bossTrans = boss->GetComponent<Stormlight::TransformComponent>();
	bossTrans.Translation.x = Stormlight::GetRandomFloat(0, 7.0f);
	bossTrans.Translation.y = Stormlight::GetRandomFloat(-4.5f, 4.5f);

	bossTrans.Scale.x = 4.0f;
	bossTrans.Scale.y = 4.0f;

	auto& hp = boss->AddComponent<Stormlight::HealthComponent>();
	hp.owner = *boss;
	hp.CreateHealthBars(health);
	hp.UpdateHealthBars();
	hp.maxHealth = 1000.0f;

	boss->OnCreate();
	m_GameScene->OnRuntimeAddEntity(*boss);

	return boss;
}

void EnemyManager::CreateEnemy(Enemy* enemy)
{
	enemy->AddComponent<Stormlight::MovementComponent>();
	enemy->AddComponent<Stormlight::EnemyComponent>();
	enemy->AddComponent<Stormlight::SpriteRendererComponent>();
	enemy->AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& rb2denemy = enemy->AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2denemy.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;

	auto& enemyTrans = enemy->GetComponent<Stormlight::TransformComponent>();
	enemyTrans.Translation.x = Stormlight::GetRandomFloat(0, 7.0f);
	enemyTrans.Translation.y = Stormlight::GetRandomFloat(-4.5f, 4.5f);

	auto& hp = enemy->AddComponent<Stormlight::HealthComponent>();
	hp.owner = *enemy;
	hp.CreateHealthBars(350.0f);
	hp.UpdateHealthBars();

	enemy->OnCreate();
	m_GameScene->OnRuntimeAddEntity(*enemy);
	m_EncounterEnemies.push_back(enemy);
}
