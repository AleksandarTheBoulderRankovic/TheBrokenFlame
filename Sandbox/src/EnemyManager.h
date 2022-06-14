#pragma once
#include "Stormlight.h"
#include "Enemy.h"
#include "TextureRepository.h"
#include "AudioManager.h"

class EnemyManager {
public:
	EnemyManager(Stormlight::Ref<TextureRepository> textureRepository, Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr)
		: m_TextureRepository(textureRepository), m_GameScene(gameScene), m_AudioManager(audioManager) {}

	void CreateEnemies();
	bool UpdateEnemies(Stormlight::Timestep ts);
	FairyBoss* CreateBoss(float health = 1000.0f);
	void CreateFairy(glm::vec2 location);
	void CreateMinotaurSG();
private:
	void CreateBandit();
	void CreateMinotaur();
	void CreateEnemy(Enemy* enemy);
	void DestroyEnemy(Enemy* enemy, int index);
private:
	std::vector<Enemy*> m_EncounterEnemies;
	Stormlight::Ref<TextureRepository> m_TextureRepository;
	Stormlight::Scene* m_GameScene;
	float offset = -0.2f;
	AudioManager* m_AudioManager;
};
