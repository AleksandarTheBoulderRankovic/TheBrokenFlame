#pragma once
#include "Skill.h"

class Peasant : public Skill {
public:
	Peasant(Stormlight::Scene* gameScene, AudioManager* audioManager = nullptr);
	virtual void Use() override;
	virtual void Update(Stormlight::Timestep ts) override;
	virtual void OnDestroy() override;
private:
	void UpdatePeasant(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts);
	void OnAttack(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts);
	void OnMove(Stormlight::Entity& peasant, Stormlight::Entity& enemy, Stormlight::Timestep ts);
	Stormlight::Entity NearestEnemy(Stormlight::Entity& peasant);
private:
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_AttackSubTextures;
	Stormlight::Ref<Stormlight::Texture2D> m_PeasantWeapon;
	float offset = -0.31f;
};