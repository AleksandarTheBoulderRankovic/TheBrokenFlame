#include "AudioManager.h"

AudioManager::AudioManager()
{
	Hazel::Audio::Init();
	LoadAudio();
}

void AudioManager::LoadAudio()
{
	// Music
	m_FightBoss = Hazel::AudioSource::LoadFromFile("assets/game/audio/fight_boss.ogg", false);
	m_FightNormal = Hazel::AudioSource::LoadFromFile("assets/game/audio/fight_normal.ogg", false);
	m_Overworld = Hazel::AudioSource::LoadFromFile("assets/game/audio/overworld.ogg", false);

	// Events
	m_UWU = Hazel::AudioSource::LoadFromFile("assets/game/audio/testClip.mp3", false);
	m_MoneyGained = Hazel::AudioSource::LoadFromFile("assets/game/audio/money_gain.ogg", false);
	m_GameOverKilledByEnemy = Hazel::AudioSource::LoadFromFile("assets/game/audio/game_over_killed_by_enemy_01.ogg", false);
	m_EncounterAlreadyCleared = Hazel::AudioSource::LoadFromFile("assets/game/audio/scenario_already_cleared.ogg", false);
	m_EncounterBeginFight = Hazel::AudioSource::LoadFromFile("assets/game/audio/scenario_begin_fight.ogg", false);
	m_EncounterBeginShop = Hazel::AudioSource::LoadFromFile("assets/game/audio/scenario_begin_shop.ogg", false);
	m_EncounterCompleted = Hazel::AudioSource::LoadFromFile("assets/game/audio/scenario_complete_sound.ogg", false);
	
	// Weapons
	m_ReleseArrow = Hazel::AudioSource::LoadFromFile("assets/game/audio/release_arrow_01.ogg", false);
	m_ReleseEnergyBall = Hazel::AudioSource::LoadFromFile("assets/game/audio/release_ball.ogg", false);
	m_ProjectileImpact = Hazel::AudioSource::LoadFromFile("assets/game/audio/projectile_impact_01.ogg", false);
	
	// Skills
	m_SkillCooldownUp = Hazel::AudioSource::LoadFromFile("assets/game/audio/skill_cooldown_up.ogg", false);
	m_HookRelease = Hazel::AudioSource::LoadFromFile("assets/game/audio/hook_release.ogg", false);
	m_HookImapct = Hazel::AudioSource::LoadFromFile("assets/game/audio/hook_impact.ogg", false);
	m_KnivesRelease = Hazel::AudioSource::LoadFromFile("assets/game/audio/release_knives.ogg", false);
	m_PeasantArrive = Hazel::AudioSource::LoadFromFile("assets/game/audio/squires_glory.ogg", false);
	m_DaggerImpact = Hazel::AudioSource::LoadFromFile("assets/game/audio/dagger_attack_impact.ogg", false);

	// Enemies
	m_Fade = Hazel::AudioSource::LoadFromFile("assets/game/audio/fade.ogg", false);
	m_Death = Hazel::AudioSource::LoadFromFile("assets/game/audio/death.ogg", false);
	m_PrepareWhirl = Hazel::AudioSource::LoadFromFile("assets/game/audio/prepare_whirl.ogg", false);
	
	// Items
	m_ItemPickUp = Hazel::AudioSource::LoadFromFile("assets/game/audio/item_pickup.ogg", false);
	m_Spyglass = Hazel::AudioSource::LoadFromFile("assets/game/audio/spyglass.ogg", false);
	m_WarpStone = Hazel::AudioSource::LoadFromFile("assets/game/audio/warp_stone.ogg", false);
	m_Heal = Hazel::AudioSource::LoadFromFile("assets/game/audio/heal.ogg", false);
	
}
