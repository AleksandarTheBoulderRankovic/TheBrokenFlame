#pragma once
#include "Stormlight.h"
#include "AudioManager.h"

class Weapon{
public:
	Weapon() = default;
	virtual void StartAttack() {}
	virtual void UpdateAttack(Stormlight::Timestep ts) {}
	virtual void OnDestroy() {}
};

class Sword : public Weapon {
public:
	Sword() = default;
	virtual void StartAttack() override {}
	virtual void UpdateAttack(Stormlight::Timestep ts) override {}
	virtual void OnDestroy() override {}
};

class Crossbow : public Weapon {
public:
	Crossbow(Stormlight::Scene* gameScene);
	virtual void StartAttack() override {}
	virtual void UpdateAttack(Stormlight::Timestep ts) override {}
	virtual void OnDestroy() override {}
protected:
	Stormlight::Entity CreateProjectile(Stormlight::TransformComponent& playerTrans, glm::vec2 direction);
protected:
	Stormlight::Ref<Stormlight::Texture2D> m_ProjectileTexture;
	Stormlight::Scene* m_GameScene;
	Stormlight::Entity m_MainPlayer;
};

class PlayerCrossbow : public Crossbow
{
public:
	PlayerCrossbow(Stormlight::Scene* gameScene, AudioManager* audioManager)
		: Crossbow(gameScene), m_AudioManager(audioManager)
	{
		m_ProjectileTexture = Stormlight::Texture2D::Create("assets/game/textures/Projectile.png");
	}
	virtual void StartAttack() override;
	virtual void UpdateAttack(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	AudioManager* m_AudioManager;
};

class EnemyCrossbow : public Crossbow
{
public:
	EnemyCrossbow(Stormlight::Scene* gameScene, Stormlight::Entity myEnemy) : Crossbow(gameScene), m_MyEnemy(myEnemy) 
	{
		m_ProjectileTexture = Stormlight::Texture2D::Create("assets/game/textures/EnemyArrow.png");
	}
	virtual void StartAttack() override;
private:
	Stormlight::Entity m_MyEnemy;
};
