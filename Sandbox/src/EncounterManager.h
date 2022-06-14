#pragma once

#include "Stormlight.h"
#include "TextureRepository.h"
#include "AudioManager.h"
#include "HUDManager.h"
#include "ItemManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include <queue>
class Encounter;
class EncounterManager {
public:
	EncounterManager(Stormlight::Scene* backgroundScene, Stormlight::Scene* gameScene, Stormlight::Scene* forgroundScene,
					 Stormlight::Ref<TextureRepository> textureRepository, Player* player, AudioManager* audioManager, HUDManager* HUDManager);

	void CreateEncounters();
	void RevealEncounters();
	void ConcealEncounters();
	void EnterEncounter();
	void UpdateArrow(Stormlight::Timestep ts);
	void SpyglassUsed();
	void WarpStoneUsed();

	void Reload();
public:
	Player* m_ControlledPlayer;
	std::vector<Encounter*> m_RevealedEncounters;
	std::vector<Encounter*> m_ReachableEncounters;
	Encounter* m_CurrentEncounter;
	Encounter* m_HoveredEncounter;
	bool m_EncounterHovered = false;
	bool m_PlayerChoosingPath = false;
	ItemManager* m_ItemManager;
private:
	void CreateSpecialEncounters(std::vector<std::pair<float, float>>& visited, std::queue<std::pair<float, float>>& BFSQueue);
	void DarkEncounter();
	bool Contains(std::vector<std::pair<float, float>>& vector, std::pair<float, float> element);
	bool CheckMouseOverlap(Stormlight::Entity& entity);
private:
	Stormlight::Scene* m_BackgroundScene = nullptr;
	Stormlight::Scene* m_GameScene = nullptr;
	Stormlight::Scene* m_ForgroundScene = nullptr;
	Stormlight::Ref<TextureRepository> m_TextureRepository;
	
	
	EnemyManager* m_EnemyManager;
	AudioManager* m_AudioManager;
	HUDManager* m_HUDManager;

	std::map<std::pair<float,float>, std::vector<std::pair<float, float>>> m_EncounterGraph;
	std::map<std::pair<float, float>, Encounter*> m_LocationMapping;

	Stormlight::Entity m_EncounterArrow;
	float m_ArrowHowerOffset = 0.0f;

	friend class Encounter;
	friend class StagingGround;
	friend class Flag;
	friend class BasicEncounter;
	friend class StrongEncounter;
	friend class Shop;
	friend class Boulder;
	friend class Boss;
};