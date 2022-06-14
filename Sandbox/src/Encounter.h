#pragma once

#include "Stormlight.h"
#include "TextureRepository.h"
#include "Enemy.h"
#include "FieldTrap.h"

class EncounterManager;

enum class EncounterType {
	BASIC_ENCOUNTER = 0,
	STRONG_ENCOUNTER,
	FLAG,
	SHOP,
	BOULDER,
	BOSS,
	STAGING_GROUND,
	DARK_ENCOUNTER
};

class Encounter : public Stormlight::Entity 
{
public:
	Encounter(Stormlight::Entity const& entity,
			  EncounterManager* encounterManager,
			  glm::vec2 location,
		      EncounterType encounterType = EncounterType::BASIC_ENCOUNTER)
	: Stormlight::Entity(entity),
	  m_EncounterManager(encounterManager),
	  m_EncounterType(encounterType) 
	{
		auto& EncounterTransform = this->GetComponent<Stormlight::TransformComponent>();
		EncounterTransform.Translation.x = location.x;
		EncounterTransform.Translation.y = location.y;
		EncounterTransform.Translation.z = -0.1f;
	}
public:
	EncounterType m_EncounterType;
public:
	virtual void OnCreate();
	virtual void OnUpdate(Stormlight::Timestep ts);
	virtual void OnExit();

	virtual void Reveal() {}
protected:
	void UpdateProjectile(Stormlight::Entity projectile, Stormlight::Timestep ts);
	void HandleCollisions(Stormlight::Entity projectile);
protected:
	EncounterManager* m_EncounterManager;
};

class StagingGround : public Encounter {
public:
	StagingGround(Stormlight::Entity const& entity,
				  EncounterManager* encounterManager,
				  glm::vec2 location,
				  EncounterType encounterType = EncounterType::STAGING_GROUND)
				  : Encounter(entity, encounterManager, location, encounterType) {}

	virtual void OnCreate() override;
	//virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnExit() override;
};

class Flag : public Encounter {
public:
	Flag(Stormlight::Entity const& entity,
		EncounterManager* encounterManager,
		glm::vec2 location,
		EncounterType encounterType = EncounterType::DARK_ENCOUNTER);
	virtual void OnCreate() override {}
	virtual void OnUpdate(Stormlight::Timestep ts) override {}
	virtual void OnExit() override {}
};

class BasicEncounter : public Encounter {
public:
	BasicEncounter(Stormlight::Entity const& entity, EncounterManager* encounterManager, glm::vec2 location)
		: Encounter(entity, encounterManager, location) {}

	virtual void OnExit() override;
	
	virtual void Reveal() override;
};

class StrongEncounter : public Encounter {
public:
	StrongEncounter(Stormlight::Entity const& entity,
		EncounterManager* encounterManager,
		glm::vec2 location,
		EncounterType encounterType = EncounterType::STRONG_ENCOUNTER)
		: Encounter(entity, encounterManager, location, encounterType) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnExit() override;

	virtual void Reveal() override;
private:
	FieldTrap* m_FieldTrap;
};

class Shop : public Encounter {
public:
	Shop(Stormlight::Entity const& entity,
		EncounterManager* encounterManager,
		glm::vec2 location,
		EncounterType encounterType = EncounterType::SHOP)
		: Encounter(entity, encounterManager, location, encounterType) {}

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnExit() override;

	virtual void Reveal() override;
private:
	Stormlight::Entity m_Shopkeeper;
	Stormlight::Entity m_DoorEntity;
	Stormlight::Entity m_DoorImage;
};

class Boulder : public Encounter {
public:
	Boulder(Stormlight::Entity const& entity,
		EncounterManager* encounterManager,
		glm::vec2 location,
		EncounterType encounterType = EncounterType::BOULDER);
};

class Boss : public Encounter {
public:
	Boss(Stormlight::Entity const& entity,
		EncounterManager* encounterManager,
		glm::vec2 location,
		EncounterType encounterType = EncounterType::BOSS);

	virtual void OnCreate() override;
	virtual void OnUpdate(Stormlight::Timestep ts) override;
	virtual void OnExit() override;
private:
	FairyBoss* m_Boss = nullptr;
	FairyBoss* m_BossCopy_1 = nullptr;
	FairyBoss* m_BossCopy_2 = nullptr;
	bool m_Split = false;
};
