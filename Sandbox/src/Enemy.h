#pragma once
#include "Character.h"

class Enemy : public Character {
public:
	Enemy() = default;
	Enemy(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Character(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	bool operator<(const Enemy& otherEnemy) const { return (uint32_t)(*this) < (uint32_t)otherEnemy; }
protected:
	void OnHooked(Stormlight::Timestep ts);
	
protected:
	static Stormlight::Entity s_MainPlayer;
	int m_FunctionIndex = 0;
	std::vector<void (*)(Stormlight::Entity& entity, Stormlight::Timestep ts)> m_Functions;
	Stormlight::Timer m_PhaseTimer = Stormlight::Timer(1.2f, 0.0f);

};

class Bandit : public Enemy {
public:
	Bandit() = default;
	Bandit(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Enemy(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	static void Patrol(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void Attack(Stormlight::Entity& entity, Stormlight::Timestep ts);
private:
	void AssignPatrolRoute();
private:
	Weapon* m_Weapon = nullptr;
	Entity m_PatrolHelper;
	bool m_PatrolInProgress = false;
};

class Minotaur : public Enemy {
public:
	Minotaur() = default;
	Minotaur(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Enemy(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	static void MoveToPlayer(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void WindUpAttack(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void Attack(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void Idle(Stormlight::Entity& entity, Stormlight::Timestep ts);
private:
	Stormlight::Entity m_WeaponAttackWeapon;
	float m_AnimationParametar = 0.0f;
};

class MinotaurSG : public Enemy {
public:
	MinotaurSG() = default;
	MinotaurSG(Stormlight::Entity const& entity) : Enemy(entity) {}

	virtual void OnUpdate(Stormlight::Timestep ts) override;
};


class Fairy : public Enemy {
public:
	Fairy() = default;
	Fairy(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Enemy(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
protected:
	static void Attack(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void Idle(Stormlight::Entity& entity, Stormlight::Timestep ts);
	static void Fade(Stormlight::Entity& entity, Stormlight::Timestep ts);
	void Teleport();
	void Fire(glm::vec2 direction);
	void CreateProjectile(glm::vec2 direction);
protected:
	std::vector<glm::vec2> m_ProjectileDirections;
	Stormlight::Ref<Stormlight::Texture2D> m_EnergyProjectileTexture;
};

class FairyBoss : public Fairy {
public:
	FairyBoss() = default;
	FairyBoss(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Fairy(entity, audioManager) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
public:
	bool m_Teleporting = true;
private:
	void Fire(glm::vec2 direction);
private:
	int m_TeleportCounter = 0;
};
