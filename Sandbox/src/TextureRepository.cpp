#include "TextureRepository.h"

TextureRepository::TextureRepository()
{
	LoadTextures();
}

void TextureRepository::LoadTextures()
{
	// Player
	m_PlayerSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/HumanSpriteSheet.png");
	m_PlayerSpriteSheetCrossbow = Stormlight::Texture2D::Create("assets/game/textures/HumanSpriteWeapon.png");
	m_PlayerSpriteSheetCrossbowFire = Stormlight::Texture2D::Create("assets/game/textures/HumanSpriteWeaponFire.png");
	m_ProjectileTexture = Stormlight::Texture2D::Create("assets/game/textures/Projectile.png");

	// Backgrounds
	m_Background_0 = Stormlight::Texture2D::Create("assets/game/textures/StagingBackground.png");
	m_Background_1 = Stormlight::Texture2D::Create("assets/game/textures/FirstLevel.png");
	m_Background_2 = Stormlight::Texture2D::Create("assets/game/textures/LevelBackground.png");
	m_MenuBackground = Stormlight::Texture2D::Create("assets/game/textures/MenuImage.png");

	// Level textures
	m_Flag = Stormlight::Texture2D::Create("assets/game/textures/Flag.png");
	m_FlagDark = Stormlight::Texture2D::Create("assets/game/textures/FlagDark.png");
	m_EncounterIcon = Stormlight::Texture2D::Create("assets/game/textures/EncounterIcon.png");
	m_EncounterDarkIcon = Stormlight::Texture2D::Create("assets/game/textures/EncounterDarkIcon.png");
	m_StrongEncounterIcon = Stormlight::Texture2D::Create("assets/game/textures/StrongEncounterIcon.png");
	m_StrongEncounterDarkIcon = Stormlight::Texture2D::Create("assets/game/textures/StrongEncounterDarkIcon.png");
	m_ShopIcon = Stormlight::Texture2D::Create("assets/game/textures/ShopIcon.png");
	m_ShopDarkIcon = Stormlight::Texture2D::Create("assets/game/textures/ShopDarkIcon.png");
	m_BossIcon = Stormlight::Texture2D::Create("assets/game/textures/BossIcon.png");
	m_BoulderIcon = Stormlight::Texture2D::Create("assets/game/textures/BoulderIcon.png");
	m_ArrowIcon = Stormlight::Texture2D::Create("assets/game/textures/ArrowIcon.png");

	// Forgrounds
	m_Forground_0 = Stormlight::Texture2D::Create("assets/game/textures/forground.png");
	m_Forground_1 = Stormlight::Texture2D::Create("assets/game/textures/LevelForground.png");

	// HUD
	m_HUDMoney = Stormlight::Texture2D::Create("assets/game/textures/HUDmoney.png");
	m_HUDCrossbowSlot = Stormlight::Texture2D::Create("assets/game/textures/CrossbowSlot.png");
	m_HUDHookSlot = Stormlight::Texture2D::Create("assets/game/textures/HookSlot.png");
	m_HUDKnifeSlot = Stormlight::Texture2D::Create("assets/game/textures/KnifeSlot.png");
	m_HUDBlastSlot = Stormlight::Texture2D::Create("assets/game/textures/BlastSlot.png");
	m_HUDPeasantSlot = Stormlight::Texture2D::Create("assets/game/textures/Peasant.png");
	m_HUDHPpot = Stormlight::Texture2D::Create("assets/game/textures/HUDHPpot.png");
	m_HUDSpyglass = Stormlight::Texture2D::Create("assets/game/textures/HUDspyglass.png");
	m_HUDWarpStone = Stormlight::Texture2D::Create("assets/game/textures/HUDwarpStone.png");
	m_HUDWanishingPowder = Stormlight::Texture2D::Create("assets/game/textures/HUDwanishingPowder.png");

	CreateBanditSubtextures();
	CreatePlayerSubTextures();
	CreateMinotaurSubTextures();
	CreateFairySubTextures();
	CreateBossSubTextures();
	CreateShopkeeperSubTectures();
}

void TextureRepository::CreateBanditSubtextures()
{
	m_BanditSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/Bandit.png");

	m_BanditAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.0f, 0.66666666f }, { 0.2f, 1.0f }));
	m_BanditAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.2f, 0.66666666f }, { 0.4f, 1.0f }));
	m_BanditAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.4f, 0.66666666f }, { 0.6f, 1.0f }));
	m_BanditAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.6f, 0.66666666f }, { 0.8f, 1.0f }));
	m_BanditAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.8f, 0.66666666f }, { 1.0f, 1.0f }));
	
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.0f, 0.33333333f }, { 0.2f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.2f, 0.33333333f }, { 0.4f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.4f, 0.33333333f }, { 0.6f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.6f, 0.33333333f }, { 0.8f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.8f, 0.33333333f }, { 1.0f, 0.66666666f }));

	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.6f, 0.33333333f }, { 0.8f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.4f, 0.33333333f }, { 0.6f, 0.66666666f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.2f, 0.33333333f }, { 0.4f, 0.66666666f }));

	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.0f, 0.0f }, { 0.2f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.2f, 0.0f }, { 0.4f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.4f, 0.0f }, { 0.6f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.6f, 0.0f }, { 0.8f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.8f, 0.0f }, { 1.0f, 0.33333333f }));

	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.6f, 0.0f }, { 0.8f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.4f, 0.0f }, { 0.6f, 0.33333333f }));
	m_BanditWalkSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BanditSpriteSheet, { 0.2f, 0.0f }, { 0.4f, 0.33333333f }));
}

void TextureRepository::CreateMinotaurSubTextures()
{
	m_MinotaurIdleSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/MinotaurIdle.png");
	m_MinotaurWalkSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/MinotaurWalking.png");
	m_MinotaurAttackWeaponTexture = Stormlight::Texture2D::Create("assets/game/textures/MinotaurWeaponAttack.png");
	m_MinotaurAttackBodyTexture = Stormlight::Texture2D::Create("assets/game/textures/MinotaurBodyAttack.png");

	m_MinotaurIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurIdleSpriteSheet, { 0.0f, 0.0f }, { 0.33333333f, 1.0f }));
	m_MinotaurIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurIdleSpriteSheet, { 0.33333333f, 0.0f }, { 0.66666666f, 1.0f }));
	m_MinotaurIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurIdleSpriteSheet, { 0.66666666f, 0.0f }, { 0.99999999f, 1.0f }));
	m_MinotaurIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurIdleSpriteSheet, { 0.33333333f, 0.0f }, { 0.66666666f, 1.0f }));

	m_MinotaurAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurAttackBodyTexture, { 0.0f, 0.0f }, { 1.0f, 1.0f }));
	m_MinotaurAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurAttackWeaponTexture, { 0.0f, 0.0f }, { 1.0f, 1.0f }));
	
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.0f, 0.5f }, { 0.33333333f, 1.0f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 1, 0.5f }, { 0.33333333f * 2, 1.0f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 2, 0.5f }, { 0.33333333f * 3, 1.0f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 1, 0.5f }, { 0.33333333f * 2, 1.0f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.0f, 0.5f }, { 0.33333333f, 1.0f }));

	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.0f, 0.0f }, { 0.33333333f, 0.5f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 1, 0.0f }, { 0.33333333f * 2, 0.5f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 2, 0.0f }, { 0.33333333f * 3, 0.5f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.33333333f * 1, 0.0f }, { 0.33333333f * 2, 0.5f }));
	m_MinotaurWalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurWalkSpriteSheet, { 0.0f, 0.0f }, { 0.33333333f, 0.5f }));

	m_MinotaurSGIdleSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/MinotaurSG.png");

	m_MinotaurSGIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurSGIdleSpriteSheet, { 0.0f, 0.0f }, { 0.33333333f, 1.0f }));
	m_MinotaurSGIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurSGIdleSpriteSheet, { 0.33333333f, 0.0f }, { 0.66666666f, 1.0f }));
	m_MinotaurSGIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurSGIdleSpriteSheet, { 0.66666666f, 0.0f }, { 0.99999999f, 1.0f }));
	m_MinotaurSGIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_MinotaurSGIdleSpriteSheet, { 0.33333333f, 0.0f }, { 0.66666666f, 1.0f }));
}

void TextureRepository::CreateFairySubTextures()
{
	m_FairyFadeSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/FairyIdle.png");

	m_FairyFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyFadeSpriteSheet, { 0.0f, 0.0f }, { 0.2f, 0.5f }));
	m_FairyFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyFadeSpriteSheet, { 0.2f, 0.0f }, { 0.4f, 0.5f }));
	m_FairyFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyFadeSpriteSheet, { 0.4f, 0.0f }, { 0.6f, 0.5f }));
	m_FairyFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyFadeSpriteSheet, { 0.6f, 0.0f }, { 0.8f, 0.5f }));
	m_FairyFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyFadeSpriteSheet, { 0.8f, 0.0f }, { 1.0f, 0.5f }));

	m_FairyIdleSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/FairyIdle.png");

	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.8f, 0.5f }, { 1.0f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.6f, 0.5f }, { 0.8f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.4f, 0.5f }, { 0.6f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.2f, 0.5f }, { 0.4f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.0f, 0.5f }, { 0.2f, 1.0f }));

	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.2f, 0.5f }, { 0.4f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.4f, 0.5f }, { 0.6f, 1.0f }));
	m_FairyIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyIdleSpriteSheet, { 0.6f, 0.5f }, { 0.8f, 1.0f }));

	m_FairyAttackSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/FairyAttack.png");

	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.0f, 0.0f }, { 0.1f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.1f, 0.0f }, { 0.2f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.2f, 0.0f }, { 0.3f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.3f, 0.0f }, { 0.4f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.4f, 0.0f }, { 0.5f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.5f, 0.0f }, { 0.6f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.6f, 0.0f }, { 0.7f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.7f, 0.0f }, { 0.8f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.8f, 0.0f }, { 0.9f, 1.0f }));
	m_FairyAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_FairyAttackSpriteSheet, { 0.9f, 0.0f }, { 1.0f, 1.0f }));
}

void TextureRepository::CreateBossSubTextures()
{
	m_BossSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/BossSpriteSheet.png");

	m_BossAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.0f, 0.5f }, { 0.25f, 1.0f }));
	m_BossAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.25f, 0.5f }, { 0.5f, 1.0f }));
	m_BossAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.5f, 0.5f }, { 0.75f, 1.0f }));
	m_BossAttackSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.75f, 0.5f }, { 1.0f, 1.0f }));

	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.0f, 0.5f }, { 0.25f, 1.0f }));
	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.25f, 0.5f }, { 0.5f, 1.0f }));
	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.5f, 0.5f }, { 0.75f, 1.0f }));
	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.75f, 0.5f }, { 1.0f, 1.0f }));
	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.5f, 0.5f }, { 0.75f, 1.0f }));
	m_BossIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.25f, 0.5f }, { 0.5f, 1.0f }));

	m_BossFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.0f, 0.0f }, { 0.25f, 0.5f }));
	m_BossFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.25f, 0.0f }, { 0.5f, 0.5f }));
	m_BossFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.5f, 0.0f }, { 0.75f, 0.5f }));
	m_BossFadeSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_BossSpriteSheet, { 0.75f, 0.0f }, { 1.0f, 0.5f }));
}

void TextureRepository::CreateShopkeeperSubTectures()
{
	m_ShopkeeperSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/Shopkeeper.png");

	m_ShopkeeperIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_ShopkeeperSpriteSheet, { 0.0f, 0.0f }, { 0.25f, 1.0f }));
	m_ShopkeeperIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_ShopkeeperSpriteSheet, { 0.25f, 0.0f }, { 0.5f, 1.0f }));
	m_ShopkeeperIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_ShopkeeperSpriteSheet, { 0.5f, 0.0f }, { 0.75f, 1.0f }));
	m_ShopkeeperIdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_ShopkeeperSpriteSheet, { 0.75f, 0.0f }, { 1.0f, 1.0f }));
}

void TextureRepository::CreatePlayerSubTextures()
{

	// Idle animation frames
	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.667f }, { 0.214f, 0.834f }));
	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.667f }, { 0.464f, 0.834f }));
	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.667f }, { 0.714f, 0.834f }));
	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.75f, 0.667f }, { 0.964f, 0.834f }));

	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.667f }, { 0.714f, 0.834f }));
	m_IdleSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.667f }, { 0.464f, 0.834f }));

	// Walk forward animation frames
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.334f }, { 0.202f, 0.5f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.334f }, { 0.452f, 0.5f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.334f }, { 0.702f, 0.5f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.75f, 0.334f }, { 0.952f, 0.5f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.334f }, { 0.202f, 0.5f }));

	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.5f }, { 0.202f, 0.667f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.5f }, { 0.452f, 0.667f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.5f }, { 0.702f, 0.667f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.75f, 0.5f }, { 0.952f, 0.667f }));
	m_WalkForwardSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.5f }, { 0.202f, 0.667f }));

	// Walk sideways animation frames
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.0f }, { 0.2054f, 0.167f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.0f }, { 0.4554f, 0.167f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.0f }, { 0.7054f, 0.167f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.75f, 0.0f }, { 0.9554f, 0.167f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.0f }, { 0.2054f, 0.167f }));

	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.167f }, { 0.2054f, 0.334f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.25f, 0.167f }, { 0.4554f, 0.334f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.5f, 0.167f }, { 0.7054f, 0.334f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.75f, 0.167f }, { 0.9554f, 0.334f }));
	m_WalkSidewaysSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbow, { 0.0f, 0.167f }, { 0.2054f, 0.334f }));

	// Fire Projectile
	// Idle animation frames
	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.667f }, { 0.2154f, 0.834f }));
	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.667f }, { 0.4654f, 0.834f }));
	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.667f }, { 0.7154f, 0.834f }));
	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.75f, 0.667f }, { 0.9654f, 0.834f }));

	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.667f }, { 0.7154f, 0.834f }));
	m_IdleFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.667f }, { 0.4654f, 0.834f }));

	// Walk forward animation frames
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.334f }, { 0.2154f, 0.5f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.334f }, { 0.4654f, 0.5f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.334f }, { 0.7154f, 0.5f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.75f, 0.334f }, { 0.9654f, 0.5f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.334f }, { 0.2154f, 0.5f }));

	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.5f }, { 0.2154f, 0.667f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.5f }, { 0.4654f, 0.667f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.5f }, { 0.7154f, 0.667f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.75f, 0.5f }, { 0.9654f, 0.667f }));
	m_WalkForwardFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.5f }, { 0.2154f, 0.667f }));

	// Walk sideways animation frames
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.0f }, { 0.2154f, 0.167f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.0f }, { 0.4654f, 0.167f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.0f }, { 0.7154f, 0.167f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.75f, 0.0f }, { 0.9654f, 0.167f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.0f }, { 0.2154f, 0.167f }));

	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.167f }, { 0.2154f, 0.334f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.25f, 0.167f }, { 0.4654f, 0.334f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.5f, 0.167f }, { 0.7154f, 0.334f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.75f, 0.167f }, { 0.9654f, 0.334f }));
	m_WalkSidewaysFireSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(m_PlayerSpriteSheetCrossbowFire, { 0.0f, 0.167f }, { 0.2154f, 0.334f }));

}