#pragma once
#include "Stormlight.h"

class TextureRepository {
public:
	TextureRepository();

	// Player
	Stormlight::Ref<Stormlight::Texture2D> m_PlayerSpriteSheet;
	Stormlight::Ref<Stormlight::Texture2D> m_PlayerSpriteSheetCrossbow;
	Stormlight::Ref<Stormlight::Texture2D> m_PlayerSpriteSheetCrossbowFire;
	Stormlight::Ref<Stormlight::Texture2D> m_ProjectileTexture;

	// Backgrounds
	Stormlight::Ref<Stormlight::Texture2D> m_Background_0;
	Stormlight::Ref<Stormlight::Texture2D> m_Background_1;
	Stormlight::Ref<Stormlight::Texture2D> m_Background_2;
	Stormlight::Ref<Stormlight::Texture2D> m_MenuBackground;

	// Level textures
	Stormlight::Ref<Stormlight::Texture2D> m_Flag;
	Stormlight::Ref<Stormlight::Texture2D> m_FlagDark;
	Stormlight::Ref<Stormlight::Texture2D> m_EncounterIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_EncounterDarkIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_StrongEncounterIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_StrongEncounterDarkIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_ShopIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_ShopDarkIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_BossIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_BoulderIcon;
	Stormlight::Ref<Stormlight::Texture2D> m_ArrowIcon;

	// Forgrounds
	Stormlight::Ref<Stormlight::Texture2D> m_Forground_0;
	Stormlight::Ref<Stormlight::Texture2D> m_Forground_1;

	// HUD textures
	Stormlight::Ref<Stormlight::Texture2D> m_HUDMoney;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDCrossbowSlot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDHookSlot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDKnifeSlot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDBlastSlot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDPeasantSlot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDHPpot;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDSpyglass;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDWarpStone;
	Stormlight::Ref<Stormlight::Texture2D> m_HUDWanishingPowder;

	// Bandit
	Stormlight::Ref<Stormlight::Texture2D> m_BanditSpriteSheet;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BanditAttackSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BanditWalkSubTextures;
	
	// Minotaur
	Stormlight::Ref<Stormlight::Texture2D> m_MinotaurIdleSpriteSheet;
	Stormlight::Ref<Stormlight::Texture2D> m_MinotaurWalkSpriteSheet;
	Stormlight::Ref<Stormlight::Texture2D> m_MinotaurAttackWeaponTexture;
	Stormlight::Ref<Stormlight::Texture2D> m_MinotaurAttackBodyTexture;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_MinotaurIdleSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_MinotaurAttackSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_MinotaurWalkForwardSubTextures;

	// MinotaurSG
	Stormlight::Ref<Stormlight::Texture2D> m_MinotaurSGIdleSpriteSheet;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_MinotaurSGIdleSubTextures;

	// Fairy
	Stormlight::Ref<Stormlight::Texture2D> m_FairyIdleSpriteSheet;
	Stormlight::Ref<Stormlight::Texture2D> m_FairyAttackSpriteSheet;
	Stormlight::Ref<Stormlight::Texture2D> m_FairyFadeSpriteSheet;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_FairyIdleSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_FairyAttackSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_FairyFadeSubTextures;

	// Boss
	Stormlight::Ref<Stormlight::Texture2D> m_BossSpriteSheet;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BossAttackSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BossIdleSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_BossFadeSubTextures;

	// Shopkeeper
	Stormlight::Ref<Stormlight::Texture2D> m_ShopkeeperSpriteSheet;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_ShopkeeperIdleSubTextures;

	// Player
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_IdleSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkForwardSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkSidewaysSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_IdleFireSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkForwardFireSubTextures;
	std::vector<Stormlight::Ref<Stormlight::SubTexture2D>> m_WalkSidewaysFireSubTextures;

private:
	void LoadTextures();
	void CreateBanditSubtextures();
	void CreateMinotaurSubTextures();
	void CreateFairySubTextures();
	void CreateBossSubTextures();
	void CreateShopkeeperSubTectures();
	void CreatePlayerSubTextures();
};
