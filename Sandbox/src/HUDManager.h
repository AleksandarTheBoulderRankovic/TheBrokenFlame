#pragma once
#include "Stormlight.h"
#include "TextureRepository.h"
#include "Player.h"

class HUDManager 
{
public:
	HUDManager(Stormlight::Scene* forgroundScene, Stormlight::Ref<TextureRepository> textureRepository, Player* player)
		       : m_ForgroundScene(forgroundScene), m_TextureRepository(textureRepository), m_MainPlayer(player) {}
	void CreateHUD();
	void Reload();
	void MoveHUDTop();
	void MoveHUDBottom();
	void ItemPriceTag(Stormlight::Entity& item);
	void RenderHUD();
public:
	Player* m_MainPlayer;
private:
	void InitSkillSlot(Stormlight::Entity skillSlot, int skillID);
	void UpdateConsumables();
private:
	Stormlight::Scene* m_ForgroundScene;
	Stormlight::Ref<TextureRepository> m_TextureRepository;
	Stormlight::Entity m_HUDMoney;
	Stormlight::Entity m_HUDWeapon;
	Stormlight::Entity m_HUDFirstSkill;
	Stormlight::Entity m_HUDSecondSkill;
	Stormlight::Entity m_HUDRaceSkill;
	Stormlight::Entity m_HUDHPpot;
	Stormlight::Entity m_HUDSpyglass;
	Stormlight::Entity m_HUDWarpStone;
	Stormlight::Entity m_HUDWanishingPowder;
};