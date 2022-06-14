#include "ItemManager.h"

ItemManager::ItemManager(Stormlight::Entity mainPlayer, Stormlight::Scene* forgroundScene, AudioManager* audioManager)
{
	m_MainPlayer = mainPlayer;
	m_GameScene = m_MainPlayer.GetScene();
	m_ForgroundScene = forgroundScene;
	m_AudioManager = audioManager;
	m_CheeseItemTexture = Stormlight::Texture2D::Create("assets/game/textures/Cheese.png");
	m_HPPotItemTexture = Stormlight::Texture2D::Create("assets/game/textures/HealthPot.png");
	m_SpyglassItemTexture = Stormlight::Texture2D::Create("assets/game/textures/Spyglass.png");
	m_WarpStoneItemTexture = Stormlight::Texture2D::Create("assets/game/textures/WarpStone.png");
	m_BootsOfHasteItemTexture = Stormlight::Texture2D::Create("assets/game/textures/BootsOfHaste.png");
	m_BroadswordItemTexture = Stormlight::Texture2D::Create("assets/game/textures/Broadsword.png");
	m_DarkBracersItemTexture = Stormlight::Texture2D::Create("assets/game/textures/DarkBracers.png");
	m_GlowingOrbItemTexture = Stormlight::Texture2D::Create("assets/game/textures/GlowingOrb.png");
	m_GnarledStaffItemTexture = Stormlight::Texture2D::Create("assets/game/textures/GnarledStaff.png");
	m_LeatherGlovesItemTexture = Stormlight::Texture2D::Create("assets/game/textures/LeatherGloves.png");
	m_OakWandItemTexture = Stormlight::Texture2D::Create("assets/game/textures/OakWand.png");
	m_SteelAxeItemTexture = Stormlight::Texture2D::Create("assets/game/textures/SteelAxe.png");
	m_WizardCapeItemTexture = Stormlight::Texture2D::Create("assets/game/textures/WizardCape.png");
	m_WizardHatItemTexture = Stormlight::Texture2D::Create("assets/game/textures/WizardHat.png");
	m_WizardMantleItemTexture = Stormlight::Texture2D::Create("assets/game/textures/WizardMantle.png");
}

void ItemManager::ItemCollected(Stormlight::Entity item)
{
	Hazel::Audio::Play(m_AudioManager->m_ItemPickUp);
	auto& ic = item.GetComponent<Stormlight::ItemComponent>();
	uint32_t itemId = ic.itemID;

	switch ((ItemType)itemId)
	{
	case ItemType::CHEESE:
		SL_TRACE("Cheese was Collected!");
		{
			Hazel::Audio::Play(m_AudioManager->m_Heal);
			auto& playerHc = m_MainPlayer.GetComponent<Stormlight::HealthComponent>();
			playerHc.health = playerHc.maxHealth;
			playerHc.UpdateFullHealthBar();
		}
		break;
	case ItemType::SPYGLASS:
		m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfSpyglass += 1;
		SL_TRACE("Spyglass was Collected!");
		break;
	case ItemType::HEALTH_POT:
		m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfHPpot += 1;
		SL_TRACE("HPpot was Collected!");
		break;
	case ItemType::WARP_STONE:
		m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfWarpStone += 1;
		SL_TRACE("Warp Stone was Collected!");
		break;
	default:
	{
		auto& pc = m_MainPlayer.GetComponent<Stormlight::PlayerComponent>();
		auto& hc = m_MainPlayer.GetComponent<Stormlight::HealthComponent>();
		auto& mc = m_MainPlayer.GetComponent<Stormlight::MovementComponent>();
		hc.maxHealth += ic.HpBonus;
		pc.AttackDamage += ic.AttackBonus;
		pc.MagicDamage += ic.MagicBonus;
		pc.AttackSpeed *= (1.0f - ic.AttackSpeedBonus);
		mc.Speed += ic.MovementSpeedBonus;
		pc.CooldownReduction *= (1.0f - ic.CooldownReductionBonus);

		Stormlight::Entity newItem = m_ForgroundScene->CreateEntity("ItemSlot");
		newItem.AddComponent<Stormlight::SpriteRendererComponent>().Texture = item.GetComponent<Stormlight::SpriteRendererComponent>().Texture;

		auto& tc = newItem.GetComponent<Stormlight::TransformComponent>();

		tc.Translation.x = (m_NumberOfBuffItems * 0.5) + 6.0f;
		tc.Translation.y = ((m_NumberOfBuffItems / 5) * 0.5) - 4.5f;
		tc.Translation.z = 0.999999f;

		tc.Scale.x = 0.35f;
		tc.Scale.y = 0.35f;

		m_NumberOfBuffItems++;
		break;
	}
	}
}

void ItemManager::ItemUsed(uint32_t itemID)
{
	switch ((ItemType)itemID)
	{
	case ItemType::CHEESE:
	{
		break;
	}
	case ItemType::SPYGLASS:
	{
		if (m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfSpyglass > 0)
		{
			Hazel::Audio::Play(m_AudioManager->m_Spyglass);
			m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfSpyglass -= 1;
		}
		SL_TRACE("Spyglass was Used!");
		break;
	}
	case ItemType::HEALTH_POT:
	{
		if (m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfHPpot > 0)
		{
			Hazel::Audio::Play(m_AudioManager->m_Heal);
			m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfHPpot -= 1;
			auto& hp = m_MainPlayer.GetComponent<Stormlight::HealthComponent>();
			hp.health += 50.0f;
			hp.UpdateFullHealthBar();
		}
		SL_TRACE("HPpot was Used!");
		break;
	}
	case ItemType::WARP_STONE:
	{
		if (m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfWarpStone > 0)
		{
			Hazel::Audio::Play(m_AudioManager->m_WarpStone);
			m_MainPlayer.GetComponent<Stormlight::PlayerComponent>().NumOfWarpStone -= 1;
		}
		
		SL_TRACE("Warp Stone was Used!");
		break;
	}
	}
}

void ItemManager::CreateItem(uint32_t itemId, glm::vec2 location)
{
	Stormlight::Entity item = m_GameScene->CreateEntity("Item");
	item.AddComponent<Stormlight::BoxCollider2DComponent>();
	item.AddComponent<Stormlight::Rigidbody2DComponent>();
	auto& ic = item.AddComponent<Stormlight::ItemComponent>();
	ic.itemID = itemId;
	
	auto& itemTrans = item.GetComponent<Stormlight::TransformComponent>();

	itemTrans.Translation.x = location.x;
	itemTrans.Translation.y = location.y;

	switch ((ItemType)itemId)
	{
	case ItemType::CHEESE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_CheeseItemTexture;
		ic.Price = 10;
		break;
	case ItemType::SPYGLASS:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_SpyglassItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 25 : 35;
		break;
	case ItemType::HEALTH_POT:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_HPPotItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 25 : 35;
		break;
	case ItemType::WARP_STONE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_WarpStoneItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 25 : 35;
		break;
	case ItemType::BOOTS_OF_HASTE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_BootsOfHasteItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		break;
	case ItemType::BROADSWORD:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_BroadswordItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackBonus = 25.0f;
		ic.HpBonus = 90.0f;
		break;
	case ItemType::DARK_BRACERS:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_DarkBracersItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackSpeedBonus = 0.2f;
		ic.AttackBonus = 20.0f;
		break;
	case ItemType::GLOWING_ORB:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_GlowingOrbItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.MagicBonus = 0.35f;
		break;
	case ItemType::GNARLED_STAFF:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_GnarledStaffItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.HpBonus = 90.0f;
		ic.MagicBonus = 0.25f;
		break;
	case ItemType::LEATHER_GLOVES:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_LeatherGlovesItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackSpeedBonus = 0.35f;
		break;
	case ItemType::OAK_WAND:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_OakWandItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackBonus = 0.15f;
		ic.MagicBonus = 0.25f;
		break;
	case ItemType::STEELE_AXE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_SteelAxeItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackBonus = 35.0f;
		break;
	case ItemType::WIZARD_CAPE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_WizardCapeItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.MovementSpeedBonus = 0.07f;
		ic.CooldownReductionBonus = 0.15f;
		break;
	case ItemType::WIZARD_HAT:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_WizardHatItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.CooldownReductionBonus = 0.2f;
		break;
	case ItemType::WIZARD_MANTLE:
		item.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_WizardMantleItemTexture;
		ic.Price = Stormlight::GetRandomFloat() < 0.1f ? 65 : 90;
		ic.AttackSpeedBonus = 0.25f;
		ic.CooldownReductionBonus = 0.1f;
		break;
	}

	m_GameScene->OnRuntimeAddEntity(item);
}

void ItemManager::CreateConsumableItems()
{
	uint32_t firstItem = Stormlight::GetRandomInt(1, 3);
	uint32_t secondItem = Stormlight::GetRandomInt(1, 3);
	while(firstItem == secondItem)
		secondItem = Stormlight::GetRandomInt(1, 3);

	CreateItem(firstItem, glm::vec2(-3.0f, 1.5f));
	CreateItem(secondItem, glm::vec2(-1.5f, 1.5f));
	CreateItem(0, glm::vec2(0.0f, 1.5f));
}

void ItemManager::CreateBuffItems()
{
	uint32_t firstItem = Stormlight::GetRandomInt(4, 14);
	uint32_t secondItem = Stormlight::GetRandomInt(4, 14);
	while (firstItem == secondItem)
		secondItem = Stormlight::GetRandomInt(4, 14);

	CreateItem(firstItem, glm::vec2(1.5f, 1.5f));
	CreateItem(secondItem, glm::vec2(3.0f, 1.5f));
}

void ItemManager::Reload()
{
	m_NumberOfBuffItems = 0;
	m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0];
	for(auto& ent : m_ForgroundScene->GetAllWithTag("ItemSlot"))
		m_ForgroundScene->DestroyEntity(ent);
}
