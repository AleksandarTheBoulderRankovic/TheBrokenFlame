#pragma once
#include "Stormlight.h"


class FieldTrap {
public:
	FieldTrap(Stormlight::Scene* gameScene) : m_GameScene(gameScene) {}

	virtual void OnCreate() {}
	virtual void OnUpdate(Stormlight::Timestep ts) {}
	virtual void OnDestroy() {}
protected:
	Stormlight::Scene* m_GameScene;
	Stormlight::Timer m_FireTimer = Stormlight::Timer(5.0f, 0.0f);
};

class Musketeers : public FieldTrap {
public:
	Musketeers(Stormlight::Scene* gameScene) : FieldTrap(gameScene) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	void CreateBullets();
	void CreateBullet(glm::vec2 location);
private:
	Stormlight::Entity m_MainPlayer;
	Stormlight::Ref<Stormlight::Texture2D> m_BulletTexture;
};

class ArcaneTrap : public FieldTrap {
public:
	ArcaneTrap(Stormlight::Scene* gameScene) : FieldTrap(gameScene) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	void CreateArcaneTrap();
public:
	class ArcaneTendril {
	public:
		Stormlight::Entity Head;
		Stormlight::Entity Body;

		Stormlight::Timer m_AppearTimer = Stormlight::Timer(1.0f, 0.0f);
		Stormlight::Timer m_HoldTimer = Stormlight::Timer(2.0f, 0.0f);
		Stormlight::Timer m_MoveTimer = Stormlight::Timer(5.0f, 0.0f);
		Stormlight::Timer m_DamageTickTimer = Stormlight::Timer(0.5f, 0.0f);
		float m_Sign = 1.0f;
		float m_Speed = 15.0f;
		bool m_StartAttack = false;
		bool m_TickDamageReady = true;
	public:
		void OnCreate(ArcaneTrap* arcaneTrap, glm::vec2 location);
		void OnUpdate(Stormlight::Timestep ts);
		void OnDestroy();
	private:
		ArcaneTrap* m_ArcaneTrap;
	};
private:
	Stormlight::Entity m_MainPlayer;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_HeadSubTextures;
	Stormlight::Ref<Stormlight::Texture2D> m_BodyTexture;
	std::vector<ArcaneTendril*> m_ArcaneTendrils;
	friend class ArcaneTendril;
};