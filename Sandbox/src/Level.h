#pragma once

#include "Stormlight.h"
#include "Enemy.h"
#include "TextureRepository.h"
#include "Player.h"
#include "EncounterManager.h"
#include "AudioManager.h"
#include "HUDManager.h"
#include "Encounter.h"

class Level {
public:
	Level(float windowWidth, float windowHeight);
	void OnCreate();
	void OnUpdate(Stormlight::Timestep ts);
	void OnResize(uint32_t m_ViewportSize_x, uint32_t m_ViewportSize_y);
	void OnEvent(Stormlight::Event& e);
	void OnDestroy();

	void ReloadLevel();
public:
	HUDManager* m_HUDManager;
	Stormlight::Ref<TextureRepository> m_TextureRepository;
	Player* m_ControlledPlayer = nullptr;
private:
	void CreateCameras();
	void CreateScenes();
	void CreateControlledPlayer();
	void CreateLevelBorders();

	void CreateBackgroundScene();
	void CreateForgroundScene();

	bool OnMouseButtonPressed(Stormlight::MouseButtonPressedEvent& e);
private:
	Stormlight::Entity m_TopBorder, m_LeftBorder, m_BottomBorder, m_RightBorder;

	Stormlight::Entity m_BackgroundCameraEntity;
	Stormlight::Entity m_GameCameraEntity;
	Stormlight::Entity m_ForgroundCameraEntity;
	Stormlight::Scene* m_BackgroundScene;
	Stormlight::Scene* m_GameScene;
	Stormlight::Scene* m_ForgroundScene;
	EncounterManager* m_EncounterManager;
	AudioManager* m_AudioManager;

	float m_WindowWidth = 0.0f, m_WindowHeight = 0.0f;

	// HUD Entities
	Stormlight::Entity m_HUDMoney;
	Stormlight::Entity m_HUDWeapon;
	Stormlight::Entity m_HUDFirstSkill;
	Stormlight::Entity m_HUDSecondSkill;

};