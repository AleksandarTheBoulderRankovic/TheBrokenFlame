#pragma once
#include "Stormlight.h"
#include "AudioManager.h"

class Skill {
public:
	Skill(Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr) : m_GameScene(gameScene), m_AudioManager(audioManager)
	{ m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0]; }

	virtual void Use() {}
	virtual void Update(Stormlight::Timestep ts);
	virtual void OnDestroy() {}
public:
	bool m_IsActive = false;
	Stormlight::Timer m_CooldownTimer = Stormlight::Timer(4.0f, 0.0f);
protected:
	Stormlight::Scene* m_GameScene;
	AudioManager* m_AudioManager;
	Stormlight::Entity m_MainPlayer;
};

class Blast : public Skill {
public:
	Blast(Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr);

	virtual void Use() override;
	virtual void Update(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	Stormlight::Ref<Stormlight::Texture2D> m_EnergyBallTexture;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BallSpinSubTextureVec;
	std::vector<glm::vec2> m_EnergyBallDirections;
	float m_SpinTimeElapsed = 0.0f;
	float m_FrameDuration = 0.1f;
	size_t m_SpinIndex = 0;
};

class KnifeThrow : public Skill
{
public:
	KnifeThrow(Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr);

	virtual void Use() override;
	virtual void Update(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	int m_NumberOfKnives = 5;
	Stormlight::Ref<Stormlight::Texture2D> m_KnifeTexture;
	std::vector<glm::vec2> m_KnifeDirections;
};

class Hook : public Skill
{
public:
	Hook(Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr);

	virtual void Use() override;
	virtual void Update(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	Stormlight::Ref<Stormlight::Texture2D> m_HookHeadTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_HookHeadReturnTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_HookChainTexture;
	Stormlight::Entity m_HookHead;
	Stormlight::Entity m_HookChain;
	float m_MaxScale = 0.0f;
	bool m_Finished = false;
	bool m_Hooked = false;
};