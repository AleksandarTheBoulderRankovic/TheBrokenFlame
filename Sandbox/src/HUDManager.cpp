#include "HUDManager.h"
#include "imgui/imgui.h"

void HUDManager::CreateHUD()
{
	m_HUDMoney = m_ForgroundScene->CreateEntity("HUDMoney");

	auto& srcMoney = m_HUDMoney.AddComponent<Stormlight::SpriteRendererComponent>();
	srcMoney.Texture = m_TextureRepository->m_HUDMoney;

	auto& transformMoney = m_HUDMoney.GetComponent<Stormlight::TransformComponent>();
	transformMoney.Scale.x = 1.4f;
	transformMoney.Scale.y = 0.5f;
	transformMoney.Translation.x = 2.8f;
	transformMoney.Translation.y = -4.75f;
	transformMoney.Translation.z = 0.999f;

	m_HUDWeapon = m_ForgroundScene->CreateEntity("HUDWeapon");

	m_HUDWeapon.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDCrossbowSlot;

	auto& weaponSlotTrans = m_HUDWeapon.GetComponent<Stormlight::TransformComponent>();
	weaponSlotTrans.Scale.x = 0.73f;
	weaponSlotTrans.Scale.y = 0.77f;
	weaponSlotTrans.Translation.x = -1.2f;
	weaponSlotTrans.Translation.y = -4.57f;
	weaponSlotTrans.Translation.z = 0.999f;

	auto& pc = m_MainPlayer->GetComponent<Stormlight::PlayerComponent>();
	m_HUDFirstSkill = m_ForgroundScene->CreateEntity("HUDFirstSkill");
	m_HUDFirstSkill.AddComponent<Stormlight::SpriteRendererComponent>();
	InitSkillSlot(m_HUDFirstSkill, pc.FirstSkill);

	auto& firstSkillSlotTrans = m_HUDFirstSkill.GetComponent<Stormlight::TransformComponent>();
	firstSkillSlotTrans.Scale.x = 0.73f;
	firstSkillSlotTrans.Scale.y = 0.77f;
	firstSkillSlotTrans.Translation.x = -0.4f;
	firstSkillSlotTrans.Translation.y = -4.57f;
	firstSkillSlotTrans.Translation.z = 0.999f;

	m_HUDSecondSkill = m_ForgroundScene->CreateEntity("HUDSecondSkill");
	m_HUDSecondSkill.AddComponent<Stormlight::SpriteRendererComponent>();
	InitSkillSlot(m_HUDSecondSkill, pc.SecondSkill);

	auto& secondSkillSlotTrans = m_HUDSecondSkill.GetComponent<Stormlight::TransformComponent>();
	secondSkillSlotTrans.Scale.x = 0.73f;
	secondSkillSlotTrans.Scale.y = 0.77f;
	secondSkillSlotTrans.Translation.x = 0.4f;
	secondSkillSlotTrans.Translation.y = -4.57f;
	secondSkillSlotTrans.Translation.z = 0.999f;

	m_HUDRaceSkill = m_ForgroundScene->CreateEntity("HUDRaceSkill");

	m_HUDRaceSkill.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDPeasantSlot;

	auto& raceSkillSlotTrans = m_HUDRaceSkill.GetComponent<Stormlight::TransformComponent>();
	raceSkillSlotTrans.Scale.x = 0.73f;
	raceSkillSlotTrans.Scale.y = 0.77f;
	raceSkillSlotTrans.Translation.x = 1.2f;
	raceSkillSlotTrans.Translation.y = -4.57f;
	raceSkillSlotTrans.Translation.z = 0.999f;

	m_HUDHPpot = m_ForgroundScene->CreateEntity("HUDConsumables");
	m_HUDHPpot.AddComponent<Stormlight::ItemComponent>().itemID = 2;
	m_HUDHPpot.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDHPpot;

	auto& hpPotTrans = m_HUDHPpot.GetComponent<Stormlight::TransformComponent>();
	hpPotTrans.Scale.x = 0.3f;
	hpPotTrans.Scale.y = 0.35f;
	hpPotTrans.Translation.x = -3.56f;
	hpPotTrans.Translation.y = -4.85f;
	hpPotTrans.Translation.z = 0.999f;

	m_HUDSpyglass = m_ForgroundScene->CreateEntity("HUDConsumables");
	m_HUDSpyglass.AddComponent<Stormlight::ItemComponent>().itemID = 1;
	m_HUDSpyglass.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDSpyglass;

	auto& spyglassTrans = m_HUDSpyglass.GetComponent<Stormlight::TransformComponent>();
	spyglassTrans.Scale.x = 0.3f;
	spyglassTrans.Scale.y = 0.35f;
	spyglassTrans.Translation.x = -3.87f;
	spyglassTrans.Translation.y = -4.85f;
	spyglassTrans.Translation.z = 0.999f;

	m_HUDWarpStone = m_ForgroundScene->CreateEntity("HUDConsumables");
	m_HUDWarpStone.AddComponent<Stormlight::ItemComponent>().itemID = 3;
	m_HUDWarpStone.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDWarpStone;

	auto& warpStoneTrans = m_HUDWarpStone.GetComponent<Stormlight::TransformComponent>();
	warpStoneTrans.Scale.x = 0.3f;
	warpStoneTrans.Scale.y = 0.35f;
	warpStoneTrans.Translation.x = -4.19f;
	warpStoneTrans.Translation.y = -4.85f;
	warpStoneTrans.Translation.z = 0.999f;

	m_HUDWanishingPowder = m_ForgroundScene->CreateEntity("HUDConsumablesNA");

	m_HUDWanishingPowder.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDWanishingPowder;

	auto& wanishingPowderTrans = m_HUDWanishingPowder.GetComponent<Stormlight::TransformComponent>();
	wanishingPowderTrans.Scale.x = 0.3f;
	wanishingPowderTrans.Scale.y = 0.35f;
	wanishingPowderTrans.Translation.x = -4.51f;
	wanishingPowderTrans.Translation.y = -4.85f;
	wanishingPowderTrans.Translation.z = 0.999f;
}

void HUDManager::Reload()
{
	auto& pc = m_MainPlayer->GetComponent<Stormlight::PlayerComponent>();

	InitSkillSlot(m_HUDFirstSkill, pc.FirstSkill);
	InitSkillSlot(m_HUDSecondSkill, pc.SecondSkill);

	MoveHUDBottom();
}

void HUDManager::MoveHUDTop()
{
	auto& moneyTrans = m_HUDMoney.GetComponent<Stormlight::TransformComponent>();
	moneyTrans.Translation.y = 4.7f;

	auto& weaponSlotTrans = m_HUDWeapon.GetComponent<Stormlight::TransformComponent>();
	weaponSlotTrans.Scale.x = 0.45f;
	weaponSlotTrans.Scale.y = 0.5f;
	weaponSlotTrans.Translation.x = -4.625f;
	weaponSlotTrans.Translation.y = 4.75f;

	auto& firstSkillSlotTrans = m_HUDFirstSkill.GetComponent<Stormlight::TransformComponent>();
	firstSkillSlotTrans.Scale.x = 0.45f;
	firstSkillSlotTrans.Scale.y = 0.5f;
	firstSkillSlotTrans.Translation.x = -4.125f;
	firstSkillSlotTrans.Translation.y = 4.75f;

	auto& secondSkillSlotTrans = m_HUDSecondSkill.GetComponent<Stormlight::TransformComponent>();
	secondSkillSlotTrans.Scale.x = 0.45f;
	secondSkillSlotTrans.Scale.y = 0.5f;
	secondSkillSlotTrans.Translation.x = -3.625f;
	secondSkillSlotTrans.Translation.y = 4.75f;

	auto& raceSkillSlotTrans = m_HUDRaceSkill.GetComponent<Stormlight::TransformComponent>();
	raceSkillSlotTrans.Scale.x = 0.45f;
	raceSkillSlotTrans.Scale.y = 0.5f;
	raceSkillSlotTrans.Translation.x = -3.125f;
	raceSkillSlotTrans.Translation.y = 4.75f;

	auto& hpPotTrans = m_HUDHPpot.GetComponent<Stormlight::TransformComponent>();
	hpPotTrans.Scale.x = 0.73f;
	hpPotTrans.Scale.y = 0.77f;
	hpPotTrans.Translation.x = 1.15f;
	hpPotTrans.Translation.y = 4.6f;

	auto& spyglassTrans = m_HUDSpyglass.GetComponent<Stormlight::TransformComponent>();
	spyglassTrans.Scale.x = 0.73f;
	spyglassTrans.Scale.y = 0.77f;
	spyglassTrans.Translation.x = 0.38f;
	spyglassTrans.Translation.y = 4.6f;

	auto& warpStoneTrans = m_HUDWarpStone.GetComponent<Stormlight::TransformComponent>();
	warpStoneTrans.Scale.x = 0.73f;
	warpStoneTrans.Scale.y = 0.77f;
	warpStoneTrans.Translation.x = -0.38f;
	warpStoneTrans.Translation.y = 4.6f;

	auto& wanishingPowderTrans = m_HUDWanishingPowder.GetComponent<Stormlight::TransformComponent>();
	wanishingPowderTrans.Scale.x = 0.73f;
	wanishingPowderTrans.Scale.y = 0.77f;
	wanishingPowderTrans.Translation.x = -1.13f;
	wanishingPowderTrans.Translation.y = 4.6f;

	for (auto& itemSlot : m_ForgroundScene->GetAllWithTag("ItemSlot"))
	{
		auto& itemSlotTrans = itemSlot.GetComponent<Stormlight::TransformComponent>();
		itemSlotTrans.Translation.y += 9.35f;
	}
}

void HUDManager::MoveHUDBottom()
{
	auto& moneyTrans = m_HUDMoney.GetComponent<Stormlight::TransformComponent>();
	moneyTrans.Translation.y = -4.75f;

	auto& weaponSlotTrans = m_HUDWeapon.GetComponent<Stormlight::TransformComponent>();
	weaponSlotTrans.Scale.x = 0.73f;
	weaponSlotTrans.Scale.y = 0.77f;
	weaponSlotTrans.Translation.x = -1.2f;
	weaponSlotTrans.Translation.y = -4.57f;

	auto& firstSkillSlotTrans = m_HUDFirstSkill.GetComponent<Stormlight::TransformComponent>();
	firstSkillSlotTrans.Scale.x = 0.73f;
	firstSkillSlotTrans.Scale.y = 0.77f;
	firstSkillSlotTrans.Translation.x = -0.4f;
	firstSkillSlotTrans.Translation.y = -4.57f;

	auto& secondSkillSlotTrans = m_HUDSecondSkill.GetComponent<Stormlight::TransformComponent>();
	secondSkillSlotTrans.Scale.x = 0.73f;
	secondSkillSlotTrans.Scale.y = 0.77f;
	secondSkillSlotTrans.Translation.x = 0.4f;
	secondSkillSlotTrans.Translation.y = -4.57f;

	auto& raceSkillSlotTrans = m_HUDRaceSkill.GetComponent<Stormlight::TransformComponent>();
	raceSkillSlotTrans.Scale.x = 0.73f;
	raceSkillSlotTrans.Scale.y = 0.77f;
	raceSkillSlotTrans.Translation.x = 1.2f;
	raceSkillSlotTrans.Translation.y = -4.57f;

	auto& hpPotTrans = m_HUDHPpot.GetComponent<Stormlight::TransformComponent>();
	hpPotTrans.Scale.x = 0.35f;
	hpPotTrans.Scale.y = 0.35f;
	hpPotTrans.Translation.x = -3.55f;
	hpPotTrans.Translation.y = -4.85f;

	auto& spyglassTrans = m_HUDSpyglass.GetComponent<Stormlight::TransformComponent>();
	spyglassTrans.Scale.x = 0.3f;
	spyglassTrans.Scale.y = 0.35f;
	spyglassTrans.Translation.x = -3.87f;
	spyglassTrans.Translation.y = -4.85f;

	auto& warpStoneTrans = m_HUDWarpStone.GetComponent<Stormlight::TransformComponent>();
	warpStoneTrans.Scale.x = 0.3f;
	warpStoneTrans.Scale.y = 0.35f;
	warpStoneTrans.Translation.x = -4.19f;
	warpStoneTrans.Translation.y = -4.85f;

	auto& wanishingPowderTrans = m_HUDWanishingPowder.GetComponent<Stormlight::TransformComponent>();
	wanishingPowderTrans.Scale.x = 0.3f;
	wanishingPowderTrans.Scale.y = 0.35f;
	wanishingPowderTrans.Translation.x = -4.51f;
	wanishingPowderTrans.Translation.y = -4.85f;

	for (auto& itemSlot : m_ForgroundScene->GetAllWithTag("ItemSlot"))
	{
		auto& itemSlotTrans = itemSlot.GetComponent<Stormlight::TransformComponent>();
		itemSlotTrans.Translation.y -= 9.35f;
	}
}

void HUDManager::ItemPriceTag(Stormlight::Entity& item)
{
	auto m_VieportSize_x = Stormlight::Application::Get().GetWindow().GetWidth();
	auto m_VieportSize_y = Stormlight::Application::Get().GetWindow().GetHeight();
	auto& ic = item.GetComponent<Stormlight::ItemComponent>();
	auto& trans = item.GetComponent<Stormlight::TransformComponent>();
	float xm = ((trans.Translation.x + 8.8888889f - 0.15f) * m_VieportSize_x) / 17.7777778f;
	float ym = ((-((trans.Translation.y + 1.0f) - 5.0f)) * m_VieportSize_y) / 10.0f;

	ImGui::SetCursorPos(ImVec2{ xm, ym });
	ImGui::SetWindowFontScale(1.2);
	if(ic.Price == 25)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	else
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

	ImGui::Text("%d", ic.Price);
	ImGui::PopStyleColor();
}

void HUDManager::RenderHUD()
{
	for (auto item : m_MainPlayer->GetScene()->GetAllWithTag("Item"))
	{
		ItemPriceTag(item);
	}

	UpdateConsumables();

	auto m_VieportSize_x = Stormlight::Application::Get().GetWindow().GetWidth();
	auto m_VieportSize_y = Stormlight::Application::Get().GetWindow().GetHeight();
	auto& pc = m_MainPlayer->GetComponent<Stormlight::PlayerComponent>();
	auto& trans = m_HUDMoney.GetComponent<Stormlight::TransformComponent>();
	float xm = ((trans.Translation.x + 8.8888889f) * m_VieportSize_x) / 17.7777778f;
	float ym = ((-((trans.Translation.y + 0.2f) - 5.0f)) * m_VieportSize_y) / 10.0f;

	ImGui::SetCursorPos(ImVec2{ xm, ym });
	ImGui::SetWindowFontScale(1.2);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.165f, 0.165f, 1.0f));
	ImGui::Text("%d", pc.Gold);
	ImGui::PopStyleColor();

	if (m_MainPlayer->m_FirstSkill->m_IsActive)
	{
		m_HUDFirstSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = {0.5f, 0.5f, 0.5f, 0.5f};
		auto& firstSkillTrans = m_HUDFirstSkill.GetComponent<Stormlight::TransformComponent>();
		xm = (((firstSkillTrans.Translation.x - 0.25f) + 8.8888889f) * m_VieportSize_x) / 17.7777778f;
		ym = ((-((firstSkillTrans.Translation.y + 0.2f) - 5.0f)) * m_VieportSize_y) / 10.0f;

		ImGui::SetCursorPos(ImVec2{ xm, ym });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::Text("%.2f", (m_MainPlayer->m_FirstSkill->m_CooldownTimer.GetRemainingTimerDuration()));
		ImGui::PopStyleColor();
	}
	else
	{
		m_HUDFirstSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	if (m_MainPlayer->m_SecondSkill->m_IsActive)
	{
		m_HUDSecondSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = { 0.5f, 0.5f, 0.5f, 0.5f };
		auto& secondSkillTrans = m_HUDSecondSkill.GetComponent<Stormlight::TransformComponent>();
		xm = (((secondSkillTrans.Translation.x - 0.25f) + 8.8888889f) * m_VieportSize_x) / 17.7777778f;
		ym = ((-((secondSkillTrans.Translation.y + 0.2f) - 5.0f)) * m_VieportSize_y) / 10.0f;

		ImGui::SetCursorPos(ImVec2{ xm, ym });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::Text("%.2f", (m_MainPlayer->m_SecondSkill->m_CooldownTimer.GetRemainingTimerDuration()));
		ImGui::PopStyleColor();
	}
	else
	{
		m_HUDSecondSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	if (m_MainPlayer->m_RaceSkill->m_IsActive)
	{
		m_HUDRaceSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = { 0.5f, 0.5f, 0.5f, 0.5f };
		auto& raceSkillTrans = m_HUDRaceSkill.GetComponent<Stormlight::TransformComponent>();
		xm = (((raceSkillTrans.Translation.x - 0.25f) + 8.8888889f) * m_VieportSize_x) / 17.7777778f;
		ym = ((-((raceSkillTrans.Translation.y + 0.2f) - 5.0f)) * m_VieportSize_y) / 10.0f;

		ImGui::SetCursorPos(ImVec2{ xm, ym });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::Text("%.2f", (m_MainPlayer->m_RaceSkill->m_CooldownTimer.GetRemainingTimerDuration()));
		ImGui::PopStyleColor();
	}
	else
	{
		m_HUDRaceSkill.GetComponent<Stormlight::SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

void HUDManager::InitSkillSlot(Stormlight::Entity skillSlot, int skillID)
{
	switch (skillID)
	{
	case 0:
		skillSlot.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDBlastSlot;
		break;
	case 1:
		skillSlot.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDHookSlot;
		break;
	case 2:
		skillSlot.GetComponent<Stormlight::SpriteRendererComponent>().Texture = m_TextureRepository->m_HUDKnifeSlot;
		break;
	default:
		break;
	}
}

void HUDManager::UpdateConsumables()
{
	auto m_VieportSize_x = Stormlight::Application::Get().GetWindow().GetWidth();
	auto m_VieportSize_y = Stormlight::Application::Get().GetWindow().GetHeight();
	glm::vec2 location;
	auto& trans = m_HUDHPpot.GetComponent<Stormlight::TransformComponent>();
	if (trans.Translation.y > 0.0f)
	{
		ImGui::SetWindowFontScale(0.8);
		location.x = ((trans.Translation.x + 8.8888889f + 0.1f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((trans.Translation.y - 0.15f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}
	else
	{
		ImGui::SetWindowFontScale(0.6);
		location.x = ((trans.Translation.x + 8.8888889f - 0.05f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((trans.Translation.y + 0.02f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}

	ImGui::SetCursorPos(ImVec2{ location.x, location.y });

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

	ImGui::Text("x%d", m_MainPlayer->GetComponent<Stormlight::PlayerComponent>().NumOfHPpot);
	
	auto& spyglassTrans = m_HUDSpyglass.GetComponent<Stormlight::TransformComponent>();
	if (spyglassTrans.Translation.y > 0.0f)
	{
		ImGui::SetWindowFontScale(0.8);
		location.x = ((spyglassTrans.Translation.x + 8.8888889f + 0.1f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((spyglassTrans.Translation.y - 0.15f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}
	else
	{
		ImGui::SetWindowFontScale(0.6);
		location.x = ((spyglassTrans.Translation.x + 8.8888889f - 0.05f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((spyglassTrans.Translation.y + 0.02f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}

	ImGui::SetCursorPos(ImVec2{ location.x, location.y });

	ImGui::Text("x%d", m_MainPlayer->GetComponent<Stormlight::PlayerComponent>().NumOfSpyglass);
	
	auto& warpStoneTrans = m_HUDWarpStone.GetComponent<Stormlight::TransformComponent>();
	if (warpStoneTrans.Translation.y > 0.0f)
	{
		ImGui::SetWindowFontScale(0.8);
		location.x = ((warpStoneTrans.Translation.x + 8.8888889f + 0.1f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((warpStoneTrans.Translation.y - 0.15f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}
	else
	{
		ImGui::SetWindowFontScale(0.6);
		location.x = ((warpStoneTrans.Translation.x + 8.8888889f - 0.05f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((warpStoneTrans.Translation.y + 0.02f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}

	ImGui::SetCursorPos(ImVec2{ location.x, location.y });

	ImGui::Text("x%d", m_MainPlayer->GetComponent<Stormlight::PlayerComponent>().NumOfWarpStone);

	auto& wanshingPowderTrans = m_HUDWanishingPowder.GetComponent<Stormlight::TransformComponent>();
	if (wanshingPowderTrans.Translation.y > 0.0f)
	{
		ImGui::SetWindowFontScale(0.8);
		location.x = ((wanshingPowderTrans.Translation.x + 8.8888889f + 0.1f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((wanshingPowderTrans.Translation.y - 0.15f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}
	else
	{
		ImGui::SetWindowFontScale(0.6);
		location.x = ((wanshingPowderTrans.Translation.x + 8.8888889f - 0.05f) * m_VieportSize_x) / 17.7777778f;
		location.y = ((-((wanshingPowderTrans.Translation.y + 0.02f) - 5.0f)) * m_VieportSize_y) / 10.0f;
	}

	ImGui::SetCursorPos(ImVec2{ location.x, location.y });

	ImGui::Text("x%d", 1);

	ImGui::PopStyleColor();
}
