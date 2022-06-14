#pragma once
#include "Stormlight.h"
#include "Weapon.h"
#include "AudioManager.h"

class Character : public Stormlight::Entity {
public:
	Character() = default;
	Character(Stormlight::Entity const& entity, AudioManager* audioManager = nullptr) : Stormlight::Entity(entity), m_AudioManager(audioManager) {}

	virtual void OnCreate() = 0;
	virtual void OnUpdate(Stormlight::Timestep ts) = 0;
	virtual void OnDestroy() = 0;
protected:
	AudioManager* m_AudioManager;
};
