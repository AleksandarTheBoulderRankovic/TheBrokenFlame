#include "Encounter.h"
#include "EncounterManager.h"
#include "Animation.h"

void Encounter::OnCreate()
{
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_Overworld);
	m_EncounterManager->m_AudioManager->m_EncounterBeginFight.SetGain(0.5f);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_EncounterBeginFight);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_FightNormal);
	m_EncounterManager->m_AudioManager->m_FightNormal.SetLoop(true);
	m_EncounterManager->m_EnemyManager->CreateEnemies();
	m_EncounterManager->ConcealEncounters();
	auto Background = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	Background.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Background_2;

	auto Forground = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	Forground.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Forground_1;

	m_EncounterManager->m_HUDManager->MoveHUDBottom();

	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = -6.6f;
	playerTrans.Translation.y = -3.5f;
}

void Encounter::OnUpdate(Stormlight::Timestep ts)
{
	auto& hc = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::HealthComponent>();
	if (m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>().StartCombat)
	{
		for (auto projectile : this->GetScene()->GetAllWithTag("Projectile"))
		{
			UpdateProjectile(projectile, ts);
		}

		if (m_EncounterType != EncounterType::DARK_ENCOUNTER && m_EncounterManager->m_EnemyManager->UpdateEnemies(ts))
		{
			OnExit();
			return;
		}
	}

	
	if (hc.health <= 0.0f)
	{
		Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightBoss);
		Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightNormal);
		Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_Overworld);
		Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_GameOverKilledByEnemy);
		m_EncounterManager->m_ControlledPlayer->m_GameOver = true;
	}
}

void Encounter::OnExit()
{
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightNormal);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_EncounterCompleted);
	m_EncounterManager->m_AudioManager->m_Overworld.SetGain(0.2f);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_Overworld);
	m_EncounterManager->m_AudioManager->m_Overworld.SetLoop(true);
	for (auto projectile : this->GetScene()->GetAllWithTag("Projectile"))
	{
		this->GetScene()->DestroyEntity(projectile);
	}

	m_EncounterManager->m_PlayerChoosingPath = true;
	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	auto& pc = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>();
	pc.StartCombat = false;
	auto& encounterTrans = m_EncounterManager->m_CurrentEncounter->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = encounterTrans.Translation.x;
	playerTrans.Translation.y = encounterTrans.Translation.y;
	m_EncounterManager->m_ControlledPlayer->OnStageChange();

	m_EncounterManager->RevealEncounters();
	auto& src = m_EncounterManager->m_CurrentEncounter->GetComponent<Stormlight::SpriteRendererComponent>();
	src.Texture = src.secondTexture;
	m_EncounterType = EncounterType::DARK_ENCOUNTER;

	auto forgroundEnt = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	if(forgroundEnt.HasComponent<Stormlight::SpriteRendererComponent>())
		forgroundEnt.RemoveComponent<Stormlight::SpriteRendererComponent>();

	m_EncounterManager->m_HUDManager->MoveHUDTop();

	auto backgroundEnt = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	auto& srcBackground = backgroundEnt.GetComponent<Stormlight::SpriteRendererComponent>();
	srcBackground.Texture = m_EncounterManager->m_TextureRepository->m_Background_1;
}

void Encounter::UpdateProjectile(Stormlight::Entity projectile, Stormlight::Timestep ts)
{
	auto xySpeed = projectile.GetComponent<Stormlight::MovementComponent>().XYSpeed;
	auto& transform = projectile.GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x += xySpeed.x * ts * 6.05f;
	transform.Translation.y += xySpeed.y * ts * 6.05f;

	HandleCollisions(projectile);
}

void Encounter::HandleCollisions(Stormlight::Entity projectile)
{
	if (this->GetScene()->CheckCollision(*(m_EncounterManager->m_ControlledPlayer), projectile))
	{
		Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_ProjectileImpact);
		this->GetScene()->DestroyEntity(projectile);
		auto& hp = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::HealthComponent>();
		hp.health -= 30.0f;
		hp.UpdateFullHealthBar();
		return;
	}

	for (auto ent : this->GetScene()->GetAllWithTag("Border"))
	{
		if (this->GetScene()->CheckCollision(ent, projectile))
		{
			this->GetScene()->DestroyEntity(projectile);
			return;
		}
	}

	for (auto ent : this->GetScene()->Reg().view<Stormlight::PeasantComponent>())
	{
		Stormlight::Entity peasant = { ent, this->GetScene() };
		if (this->GetScene()->CheckCollision(peasant, projectile))
		{
			this->GetScene()->DestroyEntity(projectile);
			auto& hp = peasant.GetComponent<Stormlight::HealthComponent>();
			hp.health -= 50.0f;
			hp.UpdateFullHealthBar();
			return;
		}
	}
}

void StagingGround::OnCreate()
{
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightBoss);
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightNormal); 
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_Overworld);
	m_EncounterManager->m_EnemyManager->CreateMinotaurSG();

	auto Background = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	Background.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Background_0;
	
	auto Forground = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	if(Forground.HasComponent<Stormlight::SpriteRendererComponent>())
		Forground.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Forground_0;
	else
		Forground.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Forground_0;

	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = 0.0f;
	playerTrans.Translation.y = 0.0f;
}

void StagingGround::OnExit()
{
	m_EncounterManager->m_AudioManager->m_Overworld.SetGain(0.2f);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_Overworld);
	m_EncounterManager->m_AudioManager->m_Overworld.SetLoop(true);
	for (auto projectile : this->GetScene()->GetAllWithTag("Projectile"))
	{
		this->GetScene()->DestroyEntity(projectile);
	}

	Encounter* tmpEncounter = m_EncounterManager->m_CurrentEncounter;
	m_EncounterManager->CreateEncounters();
	m_EncounterManager->m_PlayerChoosingPath = true;
	auto forgroundEnt = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	forgroundEnt.RemoveComponent<Stormlight::SpriteRendererComponent>();

	m_EncounterManager->m_HUDManager->MoveHUDTop();

	auto backgroundEnt = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	auto& src = backgroundEnt.GetComponent<Stormlight::SpriteRendererComponent>();
	src.Texture = m_EncounterManager->m_TextureRepository->m_Background_1;

	auto& flagTrans = m_EncounterManager->m_CurrentEncounter->GetComponent<Stormlight::TransformComponent>();
	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = flagTrans.Translation.x;
	playerTrans.Translation.y = flagTrans.Translation.y;

	m_EncounterManager->m_ControlledPlayer->OnStageChange();
	
	this->GetScene()->DestroyEntity(this->GetScene()->GetAllWithTag("StagingGround")[0]);
	delete tmpEncounter;
}

Flag::Flag(Stormlight::Entity const& entity, EncounterManager* encounterManager, glm::vec2 location, EncounterType encounterType)
		   : Encounter(entity, encounterManager, location, encounterType)
{
	auto& srcFlag = this->AddComponent<Stormlight::SpriteRendererComponent>();
	srcFlag.Texture = m_EncounterManager->m_TextureRepository->m_FlagDark;
	srcFlag.secondTexture = m_EncounterManager->m_TextureRepository->m_FlagDark;
}

void BasicEncounter::OnExit()
{
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_MoneyGained);
	m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>().AddGold(20);
	Encounter::OnExit();
}

void BasicEncounter::Reveal()
{
	auto& srcEnc = this->AddComponent<Stormlight::SpriteRendererComponent>();
	srcEnc.Texture = m_EncounterManager->m_TextureRepository->m_EncounterIcon;
	srcEnc.secondTexture = m_EncounterManager->m_TextureRepository->m_EncounterDarkIcon;
}

void StrongEncounter::OnCreate()
{
	Encounter::OnCreate();
	float random = Stormlight::GetRandomFloat();
	if(random < 0.5f)
		m_FieldTrap = new Musketeers(m_EncounterManager->m_GameScene);
	else
		m_FieldTrap = new ArcaneTrap(m_EncounterManager->m_GameScene);
	m_FieldTrap->OnCreate();
}

void StrongEncounter::OnUpdate(Stormlight::Timestep ts)
{
	if (m_EncounterType != EncounterType::DARK_ENCOUNTER && m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>().StartCombat)
		m_FieldTrap->OnUpdate(ts);
	Encounter::OnUpdate(ts);
}

void StrongEncounter::OnExit()
{
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_MoneyGained);
	auto& pc = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>();
	pc.AddGold(20);
	pc.GainConsumable();
	Encounter::OnExit();
	m_FieldTrap->OnDestroy();
	delete m_FieldTrap;
}

void StrongEncounter::Reveal()
{
	auto& srcEnc = this->AddComponent<Stormlight::SpriteRendererComponent>();
	srcEnc.Texture = m_EncounterManager->m_TextureRepository->m_StrongEncounterIcon;
	srcEnc.secondTexture = m_EncounterManager->m_TextureRepository->m_StrongEncounterDarkIcon;
}

void Shop::OnCreate()
{
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_EncounterBeginShop);
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_Overworld);
	m_Shopkeeper = m_EncounterManager->m_GameScene->CreateEntity("Shopkeeper");

	auto& ac = m_Shopkeeper.AddComponent<Stormlight::AnimationComponent>();
	ac.m_IdleSubTextures = m_EncounterManager->m_TextureRepository->m_ShopkeeperIdleSubTextures;
	m_Shopkeeper.AddComponent<Stormlight::SpriteRendererComponent>().subTexture = ac.m_IdleSubTextures[0];

	auto& shopkeeperTrans = m_Shopkeeper.GetComponent<Stormlight::TransformComponent>();

	shopkeeperTrans.Translation.x = 0.0f;
	shopkeeperTrans.Translation.y = 3.0f;

	m_DoorEntity = m_EncounterManager->m_GameScene->CreateEntity("Door");

	m_DoorEntity.AddComponent<Stormlight::SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_DoorEntity.AddComponent<Stormlight::Rigidbody2DComponent>();
	m_DoorEntity.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& doorTrans = m_DoorEntity.GetComponent<Stormlight::TransformComponent>();

	doorTrans.Translation.x = 0.0f;
	doorTrans.Translation.y = -5.0f;

	m_EncounterManager->m_GameScene->OnRuntimeAddEntity(m_DoorEntity);

	m_DoorImage = m_EncounterManager->m_GameScene->CreateEntity("DoorImage");

	auto DoorTexture = Stormlight::Texture2D::Create("assets/game/textures/Door.png");
	m_DoorImage.AddComponent<Stormlight::SpriteRendererComponent>().Texture = DoorTexture;

	auto& doorImageTrans = m_DoorImage.GetComponent<Stormlight::TransformComponent>();

	doorImageTrans.Translation.x = 0.0f;
	doorImageTrans.Translation.y = -3.2f;

	doorImageTrans.Scale.x = 1.5f;
	doorImageTrans.Scale.y = 1.5f;

	m_EncounterManager->m_ItemManager->CreateConsumableItems();
	m_EncounterManager->m_ItemManager->CreateBuffItems();

	m_EncounterManager->ConcealEncounters();
	auto Background = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	Background.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Background_0;

	auto Forground = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	Forground.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Forground_0;

	m_EncounterManager->m_HUDManager->MoveHUDBottom();

	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = 0.0f;
	playerTrans.Translation.y = -2.0f;
}

void Shop::OnUpdate(Stormlight::Timestep ts)
{
	if (m_EncounterType != EncounterType::DARK_ENCOUNTER)
	{
		auto& ac = m_Shopkeeper.GetComponent<Stormlight::AnimationComponent>();
		Animation(&m_Shopkeeper, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleSubTextures);

		for (auto& item : m_EncounterManager->m_GameScene->GetAllWithTag("Item"))
		{
			if (m_EncounterManager->m_GameScene->CheckCollision(*(m_EncounterManager->m_ControlledPlayer), item))
			{
				auto& pc = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>();
				auto& ic = item.GetComponent<Stormlight::ItemComponent>();
				if (pc.Gold >= ic.Price)
				{
					m_EncounterManager->m_ItemManager->ItemCollected(item);
					pc.Gold -= ic.Price;
					m_EncounterManager->m_GameScene->DestroyEntity(item);
				}
			}
		}

		if (m_EncounterManager->m_GameScene->CheckCollision(*(m_EncounterManager->m_ControlledPlayer), m_DoorEntity))
		{
			OnExit();
		}
	}
}

void Shop::OnExit()
{
	Encounter::OnExit();
	m_EncounterManager->m_GameScene->DestroyEntity(m_Shopkeeper);
	m_EncounterManager->m_GameScene->DestroyEntity(m_DoorEntity);
	m_EncounterManager->m_GameScene->DestroyEntity(m_DoorImage);

	for (auto& item : m_EncounterManager->m_GameScene->GetAllWithTag("Item"))
	{
		m_EncounterManager->m_GameScene->DestroyEntity(item);
	}
}

void Shop::Reveal()
{
	auto& srcEnc = this->AddComponent<Stormlight::SpriteRendererComponent>();
	srcEnc.Texture = m_EncounterManager->m_TextureRepository->m_ShopIcon;
	srcEnc.secondTexture = m_EncounterManager->m_TextureRepository->m_ShopDarkIcon;
}

Boulder::Boulder(Stormlight::Entity const& entity, EncounterManager* encounterManager, glm::vec2 location, EncounterType encounterType)
	             : Encounter(entity, encounterManager, location, encounterType)
{
	this->AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_BoulderIcon;
}

Boss::Boss(Stormlight::Entity const& entity, EncounterManager* encounterManager, glm::vec2 location, EncounterType encounterType)
 	       : Encounter(entity, encounterManager, location, encounterType)
{
	auto& src = this->AddComponent<Stormlight::SpriteRendererComponent>();
	src.Texture = m_EncounterManager->m_TextureRepository->m_BossIcon;
	src.secondTexture = m_EncounterManager->m_TextureRepository->m_BossIcon;
}

void Boss::OnCreate()
{
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_Overworld);
	m_EncounterManager->m_AudioManager->m_FightBoss.SetGain(0.2f);
	Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_FightBoss);
	m_EncounterManager->m_AudioManager->m_FightBoss.SetLoop(true);
	m_EncounterManager->ConcealEncounters();
	auto Background = m_EncounterManager->m_BackgroundScene->GetAllWithTag("Background")[0];
	Background.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Background_2;

	auto Forground = m_EncounterManager->m_ForgroundScene->GetAllWithTag("Forground")[0];
	Forground.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_EncounterManager->m_TextureRepository->m_Forground_1;

	m_EncounterManager->m_HUDManager->MoveHUDBottom();
	m_Boss = m_EncounterManager->m_EnemyManager->CreateBoss(1000.0f);

	auto& playerTrans = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = -6.6f;
	playerTrans.Translation.y = -3.5f;
}

void Boss::OnUpdate(Stormlight::Timestep ts)
{
	if (m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>().StartCombat)
	{
		for (auto projectile : this->GetScene()->GetAllWithTag("Projectile"))
		{
			UpdateProjectile(projectile, ts);
		}


		m_EncounterManager->m_EnemyManager->UpdateEnemies(ts);

		m_Boss->OnUpdate(ts);
		if (m_BossCopy_1)
			m_BossCopy_1->OnUpdate(ts);
		if (m_BossCopy_2)
			m_BossCopy_2->OnUpdate(ts);
	}

	if (m_Boss->GetComponent<Stormlight::HealthComponent>().health < 0.0f)
	{
		OnExit();
		return;
	}

	if (m_Boss->m_Teleporting)
	{
		m_Boss->m_Teleporting = false;
		float random = Stormlight::GetRandomFloat();
		if (random < 0.025f)
		{
			auto& bossTrans = m_Boss->GetComponent<Stormlight::TransformComponent>();
			m_EncounterManager->m_EnemyManager->CreateFairy(glm::vec2(bossTrans.Translation.x, bossTrans.Translation.y));
		}
	}

	if (!m_Split && m_Boss->GetComponent<Stormlight::HealthComponent>().health < 250.0f)
	{
		m_Split = true;
		m_BossCopy_1 = m_EncounterManager->m_EnemyManager->CreateBoss(250.0f);
		m_BossCopy_1->GetComponent<Stormlight::SpriteRendererComponent>().Color = { 0.0f, 0.8f, 0.0f, 1.0f };
		m_BossCopy_2 = m_EncounterManager->m_EnemyManager->CreateBoss(250.0f);
		m_BossCopy_2->GetComponent<Stormlight::SpriteRendererComponent>().Color = { 0.0f, 0.8f, 0.0f, 1.0f };
	}

	auto& hc = m_EncounterManager->m_ControlledPlayer->GetComponent<Stormlight::HealthComponent>();
	if (hc.health <= 0.0f)
	{
		OnExit();
		Hazel::Audio::Play(m_EncounterManager->m_AudioManager->m_GameOverKilledByEnemy);
	}
}

void Boss::OnExit()
{
	Hazel::Audio::Stop(m_EncounterManager->m_AudioManager->m_FightBoss);
	Encounter::OnExit();
	m_EncounterManager->m_ControlledPlayer->m_GameOver = true;
}
