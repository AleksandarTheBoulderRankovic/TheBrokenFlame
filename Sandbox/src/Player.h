#pragma once

#include "Character.h"
#include "RaceSkills.h"


class Player : public Character {
public:
	Player() = default;
	Player(Stormlight::Entity const& entity, AudioManager* audioManager) : Character(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;

	void OnEvent(Stormlight::Event& e);
	void OnStageChange();
	void OnOverlap(Stormlight::Entity& otherEntity);
public:
	Skill* m_FirstSkill = nullptr;
	Skill* m_SecondSkill = nullptr;
	Skill* m_RaceSkill = nullptr;
	bool m_GameOver = false;
private:
	void IdleAnimation(Stormlight::Timestep ts);
	void WalkForwardAnimation(Stormlight::Timestep ts);
	void WalkSidewaysAnimation(Stormlight::Timestep ts);
	bool OnKeyPressed(Stormlight::KeyPressedEvent& e);
	bool OnKeyReleased(Stormlight::KeyReleasedEvent& e);
	bool OnMouseButtonPressed(Stormlight::MouseButtonPressedEvent& e);
	void InitFirstSkill(int skillID);
	void InitSecondSkill(int skillID);
private:
	float m_Speed = 5.0f;
	glm::vec2 m_WalkingDirection = { 0.0f, 0.0f };

	Weapon* m_Weapon = nullptr;
	float m_ShootingAnimationTime = 0.5f;
	float m_ShootingAnimationTimeElapsed = 0.0f;
	bool m_ShootingAnimation = false;

	bool m_AttackOnCooldown = false;
	Stormlight::Timer m_AttackCooldown = Stormlight::Timer(1.1f, 0.0f);
};
