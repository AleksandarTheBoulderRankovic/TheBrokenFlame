#pragma once
#include "Stormlight.h"
#include "AudioManager.h"

class ItemManager {
public:
	ItemManager(Stormlight::Entity mainPlayer, Stormlight::Scene* forgroundScene, AudioManager* audioManager);

	void ItemCollected(Stormlight::Entity item);
	void ItemUsed(uint32_t itemID);

	void CreateConsumableItems();
	void CreateBuffItems();

	void Reload();

	enum class ItemType
	{
		CHEESE = 0,
		SPYGLASS,
		HEALTH_POT,
		WARP_STONE,
		BOOTS_OF_HASTE,
		BROADSWORD,
		DARK_BRACERS,
		GLOWING_ORB,
		GNARLED_STAFF,
		LEATHER_GLOVES,
		OAK_WAND,
		STEELE_AXE,
		WIZARD_CAPE,
		WIZARD_HAT,
		WIZARD_MANTLE
	};
private:
	void CreateItem(uint32_t itemId, glm::vec2 location);
private:
	Stormlight::Entity m_MainPlayer;
	Stormlight::Scene* m_GameScene;
	Stormlight::Scene* m_ForgroundScene;
	AudioManager* m_AudioManager;
	int m_NumberOfBuffItems = 0;
	Stormlight::Ref<Stormlight::Texture2D> m_CheeseItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_HPPotItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_SpyglassItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_WarpStoneItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_BootsOfHasteItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_BroadswordItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_DarkBracersItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_GlowingOrbItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_GnarledStaffItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_LeatherGlovesItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_OakWandItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_SteelAxeItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_WizardCapeItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_WizardHatItemTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_WizardMantleItemTexture;
};