#include "Level.h"
#include "Animation.h"

Level::Level(float windowWidth, float windowHeight)
	: m_WindowWidth(windowWidth), m_WindowHeight(windowHeight)
{
	m_TextureRepository = Stormlight::CreateRef<TextureRepository>();
	m_BackgroundScene = new Stormlight::Scene();
	m_BackgroundScene->SetOffset(-0.8f);
	m_GameScene = new Stormlight::Scene();
	m_GameScene->SetOffset(0.0f);
	m_ForgroundScene = new Stormlight::Scene();
	m_ForgroundScene->SetOffset(0.9f);

	m_AudioManager = new AudioManager();
	CreateControlledPlayer();
	m_HUDManager = new HUDManager(m_ForgroundScene, m_TextureRepository, m_ControlledPlayer);
	m_HUDManager->CreateHUD();
	CreateScenes();
	m_EncounterManager = new EncounterManager(m_BackgroundScene, m_GameScene, m_ForgroundScene, m_TextureRepository, m_ControlledPlayer, m_AudioManager, m_HUDManager);
	
}

void Level::OnCreate()
{
	m_EncounterManager->m_CurrentEncounter = new StagingGround(m_GameScene->CreateEntity("StagingGround"), m_EncounterManager, glm::vec2(0, 0));
	
	m_BackgroundScene->OnRuntimeStart();
	m_GameScene->OnRuntimeStart();
	m_ForgroundScene->OnRuntimeStart();

	m_EncounterManager->m_CurrentEncounter->OnCreate();
}

void Level::OnUpdate(Stormlight::Timestep ts)
{
	m_ControlledPlayer->OnUpdate(ts);
	
	m_EncounterManager->m_CurrentEncounter->OnUpdate(ts);

	if (m_EncounterManager->m_PlayerChoosingPath)
	{
		m_EncounterManager->UpdateArrow(ts);
	}

	m_BackgroundScene->OnUpdateRuntime(ts);
	m_GameScene->OnUpdateRuntime(ts);
	m_ForgroundScene->OnUpdateRuntime(ts);
}

void Level::OnResize(uint32_t m_ViewportSize_x, uint32_t m_ViewportSize_y)
{
	m_BackgroundScene->OnViewportResize(m_ViewportSize_x, m_ViewportSize_y);
	m_BackgroundCameraEntity.GetComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(m_ViewportSize_x, m_ViewportSize_y);

	m_GameScene->OnViewportResize(m_ViewportSize_x, m_ViewportSize_y);
	m_GameCameraEntity.GetComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(m_ViewportSize_x, m_ViewportSize_y);

	m_ForgroundScene->OnViewportResize(m_ViewportSize_x, m_ViewportSize_y);
	m_ForgroundCameraEntity.GetComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(m_ViewportSize_x, m_ViewportSize_y);
}

void Level::OnEvent(Stormlight::Event& e)
{
	if(!m_EncounterManager->m_PlayerChoosingPath)
		m_ControlledPlayer->OnEvent(e);

	Stormlight::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Stormlight::MouseButtonPressedEvent>(SL_BIND_EVENT_FN(Level::OnMouseButtonPressed));
}

void Level::OnDestroy()
{
	m_ControlledPlayer->OnDestroy();
	delete m_ControlledPlayer;
	m_GameScene->Reg().each([&](auto& ent) { m_GameScene->DestroyEntity({ ent, m_GameScene }); });

}

void Level::ReloadLevel()
{
	OnDestroy();
	CreateControlledPlayer();
	m_EncounterManager->m_ControlledPlayer = m_ControlledPlayer;
	m_HUDManager->m_MainPlayer = m_ControlledPlayer;
	m_HUDManager->Reload();
	m_EncounterManager->m_ItemManager->Reload();
	CreateLevelBorders();
	m_GameCameraEntity = m_GameScene->CreateEntity("Camera");
	m_GameCameraEntity.AddComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(1280, 720);
	m_EncounterManager->Reload();
	m_EncounterManager->m_CurrentEncounter = new StagingGround(m_GameScene->CreateEntity("StagingGround"), m_EncounterManager, glm::vec2(0, 0));

	m_GameScene->Reg().each([&](auto& ent) 
		{ 
		Stormlight::Entity entity = { ent, m_GameScene };
		if(entity.HasComponent<Stormlight::Rigidbody2DComponent>())
			m_GameScene->OnRuntimeAddEntity(entity);
		});
     
	m_EncounterManager->m_CurrentEncounter->OnCreate();
}


void Level::CreateCameras()
{
	m_BackgroundCameraEntity = m_BackgroundScene->CreateEntity("Camera");
	m_BackgroundCameraEntity.AddComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(1280, 720);

	m_GameCameraEntity = m_GameScene->CreateEntity("Camera");
	m_GameCameraEntity.AddComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(1280, 720);

	m_ForgroundCameraEntity = m_ForgroundScene->CreateEntity("Camera");
	m_ForgroundCameraEntity.AddComponent<Stormlight::CameraComponent>().Camera.SetViewportSize(1280, 720);
}

void Level::CreateScenes()
{
	CreateCameras();
	CreateBackgroundScene();
	CreateLevelBorders();
	CreateForgroundScene();
}

void Level::CreateControlledPlayer()
{
	m_ControlledPlayer = new Player(m_GameScene->CreateEntity("MainCharacter"), m_AudioManager);
	m_ControlledPlayer->GetComponent<Stormlight::TagComponent>().SecondTag = "Player_01";
	m_ControlledPlayer->AddComponent<Stormlight::SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	auto& hp = m_ControlledPlayer->AddComponent<Stormlight::HealthComponent>();
	hp.owner = *m_ControlledPlayer;
	hp.CreateHealthBars(500.0f);
	auto& ac = m_ControlledPlayer->AddComponent<Stormlight::AnimationComponent>();
	ac.m_IdleSubTextures = m_TextureRepository->m_IdleSubTextures;
	ac.m_WalkForwardSubTextures = m_TextureRepository->m_WalkForwardSubTextures;
	ac.m_WalkSidewaysSubTextures = m_TextureRepository->m_WalkSidewaysSubTextures;
	ac.m_IdleFireSubTextures = m_TextureRepository->m_IdleFireSubTextures;
	ac.m_WalkForwardFireSubTextures = m_TextureRepository->m_WalkForwardFireSubTextures;
	ac.m_WalkSidewaysFireSubTextures = m_TextureRepository->m_WalkSidewaysFireSubTextures;
	m_ControlledPlayer->AddComponent<Stormlight::MovementComponent>();
	auto& rb2d = m_ControlledPlayer->AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2d.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	m_ControlledPlayer->AddComponent<Stormlight::BoxCollider2DComponent>();

	int first = Stormlight::GetRandomInt(0, 2);
	int second = Stormlight::GetRandomInt(0, 2);
	while(second == first)
		second = Stormlight::GetRandomInt(0, 2);

	m_ControlledPlayer->AddComponent<Stormlight::PlayerComponent>(first, second);

	m_ControlledPlayer->OnCreate();
}

void Level::CreateLevelBorders()
{
	m_TopBorder = m_GameScene->CreateEntity("Border");
	m_TopBorder.AddComponent<Stormlight::Rigidbody2DComponent>();
	m_TopBorder.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform = m_TopBorder.GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x = 0.0f;
	transform.Translation.y = 5.0f;
	transform.Translation.z = 0.1f;
	transform.Scale.x = 17.7777778f;
	transform.Scale.y = 0.2f;
	transform.Rotation.z = glm::radians(180.0f);

	m_LeftBorder = m_GameScene->CreateEntity("Border");
	m_LeftBorder.AddComponent<Stormlight::Rigidbody2DComponent>();
	m_LeftBorder.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform2 = m_LeftBorder.GetComponent<Stormlight::TransformComponent>();
	transform2.Translation.x = -8.88888889f;
	transform2.Translation.y = 0.0f;
	transform2.Translation.z = 0.9f;
	transform2.Scale.x = 0.2f;
	transform2.Scale.y = 10.0f;

	m_BottomBorder = m_GameScene->CreateEntity("Border");

	m_BottomBorder.AddComponent<Stormlight::Rigidbody2DComponent>();
	m_BottomBorder.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform3 = m_BottomBorder.GetComponent<Stormlight::TransformComponent>();
	transform3.Translation.x = 0.0f;
	transform3.Translation.y = -5.0f;
	transform3.Translation.z = 0.1f;
	transform3.Scale.x = 17.7777778f;
	transform3.Scale.y = 0.2f;

	m_RightBorder = m_GameScene->CreateEntity("Border");
	m_RightBorder.AddComponent<Stormlight::Rigidbody2DComponent>();
	m_RightBorder.AddComponent<Stormlight::BoxCollider2DComponent>();

	auto& transform4 = m_RightBorder.GetComponent<Stormlight::TransformComponent>();
	transform4.Translation.x = 8.88888889f;
	transform4.Translation.y = 0.0f;
	transform4.Scale.x = 0.2f;
	transform4.Scale.y = 10.0f;
	transform4.Rotation.z = glm::radians(180.0f);
}

void Level::CreateBackgroundScene()
{
	auto Background = m_BackgroundScene->CreateEntity("Background");

	auto& srcBackground = Background.AddComponent<Stormlight::SpriteRendererComponent>();
	srcBackground.Texture = m_TextureRepository->m_Background_0;

	auto& transformBackground = Background.GetComponent<Stormlight::TransformComponent>();
	transformBackground.Scale.x = 17.7777778f;
	transformBackground.Scale.y = 10.0f;
	transformBackground.Translation.z = -0.99f;
}

void Level::CreateForgroundScene()
{
	auto Forground = m_ForgroundScene->CreateEntity("Forground");

	auto& srcForground = Forground.AddComponent<Stormlight::SpriteRendererComponent>();
	srcForground.Texture = m_TextureRepository->m_Forground_0;

	auto& transformForground = Forground.GetComponent<Stormlight::TransformComponent>();
	transformForground.Scale.x = 17.7777778f;
	transformForground.Scale.y = 10.0f;
	transformForground.Translation.z = 0.99f;
}

bool Level::OnMouseButtonPressed(Stormlight::MouseButtonPressedEvent& e)
{
	m_EncounterManager->EnterEncounter();
	for (auto& consumable : m_ForgroundScene->GetAllWithTag("HUDConsumables"))
	{
		auto& tc = consumable.GetComponent<Stormlight::TransformComponent>();
		auto mousePos = Stormlight::Input::GetRelativeMousePosition();
		if (tc.Translation.y >= 0 && m_ForgroundScene->CheckMouseEntityCollision(glm::vec2(mousePos.first, mousePos.second), consumable))
		{
			uint32_t itemID = consumable.GetComponent<Stormlight::ItemComponent>().itemID;
			m_EncounterManager->m_ItemManager->ItemUsed(itemID);
			if (itemID == 1)
			{
				m_EncounterManager->SpyglassUsed();
			}
			if (itemID == 3)
			{
				m_EncounterManager->WarpStoneUsed();
			}
		}
	}
	return false;
}