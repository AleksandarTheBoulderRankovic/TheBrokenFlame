#include "EncounterManager.h"
#include "Encounter.h"
#include <algorithm>

EncounterManager::EncounterManager(Stormlight::Scene* backgroundScene, Stormlight::Scene* gameScene, Stormlight::Scene* forgroundScene,
	                               Stormlight::Ref<TextureRepository> textureRepository, Player* player, AudioManager* audioManager,
	                               HUDManager* HUDManager)
								   : m_BackgroundScene(backgroundScene), m_GameScene(gameScene), m_ForgroundScene(forgroundScene),
	                               m_TextureRepository(textureRepository), m_ControlledPlayer(player),
	                               m_AudioManager(audioManager), m_HUDManager(HUDManager)
{
	m_EncounterGraph =
	{
		{{-1.5f, -2.5f}, {{-2.0f, -1.25f}, {-0.75f, -1.25f}, { 0.0f, -2.5f}} },
		{{ 0.0f, -2.5f}, {{-1.5f, -2.5f}, {-0.75f, -1.25f}, { 0.75f, -1.25f}, { 1.5f, -2.5f}} },
		{{ 1.5f, -2.5f}, {{ 0.0f, -2.5f}, { 0.75f, -1.25f}, { 2.0f, -1.25f}} },
		{{-2.0f, -1.25f}, {{-1.5f, -2.5f}, {-0.75f, -1.25f}, {-1.5f,  0.0f}} },
		{{-0.75f,-1.25f}, {{-2.0f, -1.25f}, {-1.5f, -2.5f}, { 0.0f, -2.5f}, { 0.75f, -1.25f}, { 0.0f,  0.0f}, {-1.5f,  0.0f}} },
		{{ 0.75f,-1.25f}, {{-0.75f, -1.25f}, { 0.0f, -2.5f}, { 1.5f, -2.5f}, { 2.0f, -1.25f}, { 1.5f,  0.0f}, { 0.0f,  0.0f}} },
		{{ 2.0f, -1.25f}, {{ 0.75f, -1.25f}, { 1.5f, -2.5f}, { 1.5f,  0.0f}} },
		{{-1.5f,  0.0f}, {{-2.0f, -1.25f}, {-0.75f, -1.25f}, { 0.0f,  0.0f}, {-0.75f,  1.25f}, {-2.0f,  1.25f}} },
		{{ 0.0f,  0.0f}, {{-1.5f,  0.0f}, {-0.75f, -1.25f}, { 0.75f, -1.25f}, { 1.5f,  0.0f}, {-0.75f,  1.25f}, { 0.75f,  1.25f}} },
		{{ 1.5f,  0.0f}, {{ 0.0f,  0.0f}, { 0.75f, -1.25f}, { 2.0f, -1.25f}, { 2.0f,  1.25f}, { 0.75f,  1.25f}} },
		{{-2.0f,  1.25f}, {{-1.5f,  0.0f}, {-0.75f,  1.25f}, {-1.5f,  2.5f}} },
		{{-0.75f, 1.25f}, {{-2.0f,  1.25f}, {-1.5f,  0.0f}, { 0.0f,  0.0f}, { 0.75f,  1.25f}, {-1.5f,  2.5f}, { 0.0f,  2.5f}} },
		{{ 0.75f, 1.25f}, {{-0.75f,  1.25f}, { 0.0f, 0.0f}, { 1.5f,  0.0f}, { 2.0f,  1.25f}, { 1.5f,  2.5f}, { 0.0f,  2.5f}} },
		{{ 2.0f,  1.25f}, {{ 0.75f,  1.25f}, { 1.5f, 0.0f}, { 1.5f,  2.5f}} },
		{{-1.5f,  2.5f}, {{-2.0f,  1.25f}, {-0.75f,  1.25f}} },
		{{ 0.0f,  2.5f}, {{-0.75f, 1.25f}, { 0.75f,  1.25f}} },
		{{ 1.5f,  2.5f}, {{ 0.75f, 1.25f}, { 2.0f,   1.25f}} }
	};

	m_EnemyManager = new EnemyManager(m_TextureRepository, m_GameScene, m_AudioManager);
	m_ItemManager = new ItemManager(*m_ControlledPlayer, m_ForgroundScene, m_AudioManager);
	m_EncounterArrow = m_GameScene->CreateEntity("Arrow");
	m_EncounterArrow.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_ArrowIcon;
	auto& transArrow = m_EncounterArrow.GetComponent<Stormlight::TransformComponent>();
	transArrow.Translation.z = -0.999f;
}

void EncounterManager::CreateEncounters()
{
	std::vector<std::pair<float, float>> visited;
	std::queue<std::pair<float, float>> BFSQueue;

	CreateSpecialEncounters(visited, BFSQueue);

	while (!BFSQueue.empty())
	{
		if (!Contains(visited, BFSQueue.front()))
		{
			Encounter* encounter;
			float r = Stormlight::GetRandomFloat();

			if (r > 0.3f)
				encounter = new BasicEncounter(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(BFSQueue.front().first, BFSQueue.front().second));
			else
				encounter = new StrongEncounter(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(BFSQueue.front().first, BFSQueue.front().second));
			
			visited.push_back(BFSQueue.front());
			m_LocationMapping.insert(std::pair(BFSQueue.front(), encounter));

			for (auto location : m_EncounterGraph[BFSQueue.front()])
			{
				BFSQueue.push(location);
			}
		}
		BFSQueue.pop();
	}

	RevealEncounters();
}

void EncounterManager::RevealEncounters()
{
	m_ReachableEncounters.clear();
	auto& encounterTrans = m_CurrentEncounter->GetComponent<Stormlight::TransformComponent>();
	for (auto encounterLocation : m_EncounterGraph[{encounterTrans.Translation.x, encounterTrans.Translation.y}])
	{
		m_ReachableEncounters.push_back(m_LocationMapping[encounterLocation]);
		if (std::count(m_RevealedEncounters.begin(), m_RevealedEncounters.end(), m_LocationMapping[encounterLocation]) == 0)
		{
			m_RevealedEncounters.push_back(m_LocationMapping[encounterLocation]);
			(m_LocationMapping[encounterLocation])->Reveal();
		}
	}

	for (auto encounter : m_RevealedEncounters)
	{
		auto& encounterTransform = encounter->GetComponent<Stormlight::TransformComponent>();
		encounterTransform.Translation.z = -0.1f;
	}
}

void EncounterManager::ConcealEncounters()
{
	for (auto encounter : m_RevealedEncounters)
	{
		auto& encounterTransform = encounter->GetComponent<Stormlight::TransformComponent>();
		encounterTransform.Translation.z = -0.999f;
	}

	auto& transArrow = m_EncounterArrow.GetComponent<Stormlight::TransformComponent>();
	transArrow.Translation.z = -0.999f;
}

void EncounterManager::EnterEncounter()
{
	if (m_EncounterHovered)
	{
		m_EncounterHovered = false;
		m_CurrentEncounter = m_HoveredEncounter;
		if (std::count(m_RevealedEncounters.begin(), m_RevealedEncounters.end(), m_CurrentEncounter) == 0)
		{
			m_RevealedEncounters.push_back(m_CurrentEncounter);
			m_CurrentEncounter->Reveal();
		}
		if (m_CurrentEncounter->m_EncounterType != EncounterType::DARK_ENCOUNTER)
		{
			m_ControlledPlayer->GetComponent<Stormlight::PlayerComponent>().StartCombat = false;
			m_CurrentEncounter->OnCreate();
			m_PlayerChoosingPath = false;
		}
		else
		{
			Hazel::Audio::Play(m_AudioManager->m_EncounterAlreadyCleared);
			DarkEncounter();
		}
	}
}

void EncounterManager::UpdateArrow(Stormlight::Timestep ts)
{
	m_EncounterHovered = false;

	for (auto encounter : m_ReachableEncounters)
	{
		if (!(encounter->m_EncounterType == EncounterType::BOULDER) && CheckMouseOverlap(*encounter))
		{
			m_EncounterHovered = true;
			m_HoveredEncounter = encounter;
		}
	}

	m_ArrowHowerOffset += ts;
	if (m_EncounterHovered)
	{
		auto& transArrow = m_EncounterArrow.GetComponent<Stormlight::TransformComponent>();
		auto& transEncounter = m_HoveredEncounter->GetComponent<Stormlight::TransformComponent>();
		transArrow.Translation.x = transEncounter.Translation.x;
		transArrow.Translation.y = transEncounter.Translation.y + 0.25f * sin(10.0f * m_ArrowHowerOffset) + 0.5f;
		transArrow.Translation.z = 0.1f;
	}
	else if (!m_EncounterHovered)
	{
		auto& transArrow = m_EncounterArrow.GetComponent<Stormlight::TransformComponent>();
		transArrow.Translation.z = -0.999f;
	}
}

void EncounterManager::SpyglassUsed()
{
	for (auto const& [encounterLocation, encounter] : m_LocationMapping)
	{
		if (std::count(m_RevealedEncounters.begin(), m_RevealedEncounters.end(), m_LocationMapping[encounterLocation]) == 0)
		{
			m_RevealedEncounters.push_back(m_LocationMapping[encounterLocation]);
			encounter->Reveal();
		}
	}
}

void EncounterManager::WarpStoneUsed()
{
	for (auto const& [encounterLocation, encounter] : m_LocationMapping)
	{
		if (std::count(m_ReachableEncounters.begin(), m_ReachableEncounters.end(), m_LocationMapping[encounterLocation]) == 0)
		{
			m_ReachableEncounters.push_back(m_LocationMapping[encounterLocation]);
			//encounter->Reveal();
		}
	}
}

void EncounterManager::Reload()
{
	m_RevealedEncounters.clear();
	m_ReachableEncounters.clear();
	m_CurrentEncounter = nullptr;
	m_HoveredEncounter = nullptr;
	m_EncounterHovered = false;
	m_PlayerChoosingPath = false;
	m_ArrowHowerOffset = 0.0f;
	m_LocationMapping.clear();
	m_EncounterArrow = m_GameScene->CreateEntity("Arrow");
	m_EncounterArrow.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_ArrowIcon;
	auto& transArrow = m_EncounterArrow.GetComponent<Stormlight::TransformComponent>();
	transArrow.Translation.z = -0.999f;
}

void EncounterManager::CreateSpecialEncounters(std::vector<std::pair<float, float>>& visited, std::queue<std::pair<float, float>>& BFSQueue)
{
	// Create Flag Encounter
	std::vector<std::pair<float, float>> tmpFlagVec = { {-1.5f, -2.5f}, {0.0f, -2.5f}, { 1.5f, -2.5f} };
	int flagIndex = Stormlight::GetRandomInt(0, 2);

	Encounter* flag = new Flag(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(tmpFlagVec[flagIndex].first, tmpFlagVec[flagIndex].second));
	m_LocationMapping.insert(std::pair(tmpFlagVec[flagIndex], flag));
	m_RevealedEncounters.push_back(flag);
	m_CurrentEncounter = flag;

	visited.push_back(tmpFlagVec[flagIndex]);
	for (auto location : m_EncounterGraph[tmpFlagVec[flagIndex]])
	{
		BFSQueue.push(location);
	}

	// Create Shop
	std::vector<std::pair<float, float>> tmpShopVec = { {-2.0f, -1.25f}, {-0.75f,-1.25f}, {0.75f,-1.25f}, { 2.0f, -1.25f},
	{-1.5f,  0.0f}, { 0.0f,  0.0f}, { 1.5f,  0.0f}, {-2.0f,  1.25f}, {-0.75f, 1.25f}, { 0.75f, 1.25f}, { 2.0f,  1.25f}};

	int shopIndex = Stormlight::GetRandomInt(0, 10);
	Encounter* shop = new Shop(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(tmpShopVec[shopIndex].first, tmpShopVec[shopIndex].second));
	m_LocationMapping.insert(std::pair(tmpShopVec[shopIndex], shop));

	visited.push_back(tmpShopVec[shopIndex]);
	for (auto location : m_EncounterGraph[tmpShopVec[shopIndex]])
	{
		BFSQueue.push(location);
	}

	// Create Boss
	std::vector<std::pair<float, float>> tmpBossVec = {{-1.5f,  2.5f}, {0.0f,  2.5f}, {1.5f,  2.5f}};

	int bossIndex = Stormlight::GetRandomInt(0, 2);
	for (int i = 0; i < 3; i++)
	{
		if (i == bossIndex)
		{
			Encounter* boss = new Boss(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(tmpBossVec[i].first, tmpBossVec[i].second));
			m_LocationMapping.insert(std::pair(tmpBossVec[i], boss));
			m_RevealedEncounters.push_back(boss);
		}
		else
		{
			Encounter* boulder = new Boulder(m_GameScene->CreateEntity("Encounter"), this, glm::vec2(tmpBossVec[i].first, tmpBossVec[i].second));
			m_LocationMapping.insert(std::pair(tmpBossVec[i], boulder));
			m_RevealedEncounters.push_back(boulder);
		}

		visited.push_back(tmpBossVec[i]);
	}	
}

void EncounterManager::DarkEncounter()
{
	auto& encounterTrans = m_CurrentEncounter->GetComponent<Stormlight::TransformComponent>();
	auto& playerTrans = m_ControlledPlayer->GetComponent<Stormlight::TransformComponent>();
	playerTrans.Translation.x = encounterTrans.Translation.x;
	playerTrans.Translation.y = encounterTrans.Translation.y;

	RevealEncounters();
}

bool EncounterManager::Contains(std::vector<std::pair<float, float>>& vector, std::pair<float, float> element)
{
	for (auto v : vector)
	{
		if (v.first == element.first && v.second == element.second)
			return true;
	}
	return false;
}

bool EncounterManager::CheckMouseOverlap(Stormlight::Entity& entity)
{
	auto mousePosition = Stormlight::Input::GetRelativeMousePosition();
	auto bounds = entity.GetComponent<Stormlight::TransformComponent>().GetBounds();

	if (mousePosition.first <= bounds.x && mousePosition.first >= bounds.z &&
		mousePosition.second <= bounds.y && mousePosition.second >= bounds.w)
		return true;

	return false;
}
