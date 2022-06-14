#pragma once

#include "HazelAudio/HazelAudio/src/HazelAudio.h"

class AudioManager 
{
public:
	AudioManager();

	// Music
	Hazel::AudioSource m_FightBoss;
	Hazel::AudioSource m_FightNormal;
	Hazel::AudioSource m_Overworld;

	// Events
	Hazel::AudioSource m_UWU;
	Hazel::AudioSource m_MoneyGained;
	Hazel::AudioSource m_GameOverKilledByEnemy;
	Hazel::AudioSource m_EncounterAlreadyCleared;
	Hazel::AudioSource m_EncounterBeginFight;
	Hazel::AudioSource m_EncounterBeginShop;
	Hazel::AudioSource m_EncounterCompleted;

	
	// Weapons
	Hazel::AudioSource m_ReleseArrow;
	Hazel::AudioSource m_ReleseEnergyBall;
	Hazel::AudioSource m_ProjectileImpact;

	// Skills
	Hazel::AudioSource m_SkillCooldownUp;
	Hazel::AudioSource m_HookRelease;
	Hazel::AudioSource m_HookImapct;
	Hazel::AudioSource m_KnivesRelease;
	Hazel::AudioSource m_PeasantArrive;
	Hazel::AudioSource m_DaggerImpact;

	// Enemy
	Hazel::AudioSource m_PrepareWhirl;
	Hazel::AudioSource m_Fade;
	Hazel::AudioSource m_Death;

	// Items
	Hazel::AudioSource m_ItemPickUp;
	Hazel::AudioSource m_Spyglass;
	Hazel::AudioSource m_WarpStone;
	Hazel::AudioSource m_Heal;
	
private:
	void LoadAudio();
};
