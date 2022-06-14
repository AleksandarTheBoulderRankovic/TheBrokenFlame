#include "Player.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Animation.h"

void Player::OnCreate()
{
	auto& transform = this->GetComponent<Stormlight::TransformComponent>();
	transform.Translation.x = -6.0f;
	transform.Translation.y =  4.0f;
	m_Speed = this->GetComponent<Stormlight::MovementComponent>().GetSpeed();

	auto& src = this->GetComponent<Stormlight::SpriteRendererComponent>();
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	src.subTexture = ac.m_IdleSubTextures[0];

	auto pc = this->GetComponent<Stormlight::PlayerComponent>();
	InitFirstSkill(pc.FirstSkill);
	InitSecondSkill(pc.SecondSkill);
	m_RaceSkill = new Peasant(this->GetScene(), m_AudioManager);
	m_Weapon = new PlayerCrossbow(this->GetScene(), m_AudioManager);
}

void Player::OnUpdate(Stormlight::Timestep ts)
{
	
	auto& transform = this->GetComponent<Stormlight::TransformComponent>();
	auto mouseRelativePosition = Stormlight::Input::GetRelativeMousePosition();
	glm::vec3 mousePosVec = glm::vec3(mouseRelativePosition.first, mouseRelativePosition.second, 0.0f);

	m_Weapon->UpdateAttack(ts);
	if (m_AttackCooldown.OnUpdate(ts))
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
		{
			Hazel::Audio::Play(m_AudioManager->m_ReleseArrow);
			m_AttackCooldown.RestartTimer();
			m_ShootingAnimation = true;
		}
		else
		{
			m_AttackCooldown.ResetTimer();
		}
		m_AttackOnCooldown = false;
	}
	if (m_ShootingAnimation)
	{
		m_ShootingAnimationTimeElapsed += ts;
		if (!m_AttackOnCooldown && m_ShootingAnimationTimeElapsed > m_ShootingAnimationTime / 3.5f)
		{
			m_AttackOnCooldown = true;
			m_Weapon->StartAttack();
		}
		if (m_ShootingAnimationTimeElapsed > m_ShootingAnimationTime)
		{	
			m_ShootingAnimation = false;
			m_ShootingAnimationTimeElapsed = 0.0f;
		}
	}

	if (m_WalkingDirection.x != 0 && m_WalkingDirection.y != 0)
	{
		transform.Translation.x += m_WalkingDirection.x * m_Speed * ts * 0.7f;
		transform.Translation.y += m_WalkingDirection.y * m_Speed * ts * 0.7f;
	}
	else
	{
		transform.Translation.x += m_WalkingDirection.x * m_Speed * ts;
		transform.Translation.y += m_WalkingDirection.y * m_Speed * ts;
	}

	auto& hc = this->GetComponent<Stormlight::HealthComponent>();
	hc.UpdateHealthBars();
	/*
	if (hc.health <= 0.0f)
	{
		m_GameOver = true;
		Hazel::Audio::Play(m_AudioManager->m_GameOverKilledByEnemy);
	}
	*/
	glm::vec3 playerCenter = { transform.Translation.x, transform.Translation.y, 0.0f };
	auto delta = mousePosVec - playerCenter;
	delta = glm::normalize(delta);

	auto rotationAngle = std::atan2(delta.y, delta.x);
	transform.Rotation.z = rotationAngle;

	if (m_WalkingDirection.x == 0 && m_WalkingDirection.y == 0)
	{
		IdleAnimation(ts);
	}
	else
	{
		auto rotationWalkAngle = std::atan2(m_WalkingDirection.y, m_WalkingDirection.x);
		rotationAngle = glm::degrees(rotationAngle);
		rotationWalkAngle = glm::degrees(rotationWalkAngle);

		if ((rotationAngle <= (rotationWalkAngle + 45.0f) && rotationAngle >= (rotationWalkAngle - 45.0f))
			|| (rotationAngle < ((rotationWalkAngle > 0 ? (rotationWalkAngle - 180.0f) : (rotationWalkAngle + 180.0f)) + 45.0f)
				&& rotationAngle > ((rotationWalkAngle > 0 ? (rotationWalkAngle - 180.0f) : (rotationWalkAngle + 180.0f)) - 45.0f))
			|| (rotationAngle < -135.0f && (rotationWalkAngle == 180.0f || rotationWalkAngle == 0.0f)))
			WalkForwardAnimation(ts);
		else
			WalkSidewaysAnimation(ts);
	}

	if (m_FirstSkill->m_IsActive)
	{
		m_FirstSkill->Update(ts);
	}
	if (m_SecondSkill->m_IsActive)
	{
		m_SecondSkill->Update(ts);
	}

	m_RaceSkill->Update(ts);

	for (auto ent : this->GetScene()->GetAllWithTag("Border"))
	{
		if (this->GetScene()->CheckCollision(ent, *this))
		{
			OnOverlap(ent);
		}
	}
}

void Player::OnDestroy()
{
	this->GetComponent<Stormlight::HealthComponent>().DestroyHealthBars();
	m_Weapon->OnDestroy();
	delete m_Weapon;
	m_FirstSkill->OnDestroy();
	delete m_FirstSkill;
	m_SecondSkill->OnDestroy();
	delete m_SecondSkill;
	m_RaceSkill->OnDestroy();
	delete m_RaceSkill;
}

void Player::OnEvent(Stormlight::Event& e)
{
	Stormlight::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Stormlight::KeyPressedEvent>(SL_BIND_EVENT_FN(Player::OnKeyPressed));
	dispatcher.Dispatch <Stormlight::KeyReleasedEvent>(SL_BIND_EVENT_FN(Player::OnKeyReleased));
	dispatcher.Dispatch <Stormlight::MouseButtonPressedEvent>(SL_BIND_EVENT_FN(Player::OnMouseButtonPressed));
}

void Player::OnStageChange()
{
	m_WalkingDirection = { 0.0f, 0.0f };
	m_Weapon->OnDestroy();
	m_FirstSkill->OnDestroy();
	m_SecondSkill->OnDestroy();
	m_RaceSkill->OnDestroy();
}

void Player::OnOverlap(Stormlight::Entity& otherEntity)
{
	auto bounds = otherEntity.GetComponent<Stormlight::TransformComponent>().GetBounds();

	auto& transform = this->GetComponent<Stormlight::TransformComponent>();

	if (m_WalkingDirection.x == 1.0f && transform.Translation.x < bounds.z)
		transform.Translation.x = bounds.z - transform.Scale.x / 2;
	else if (m_WalkingDirection.x == -1.0f && transform.Translation.x > bounds.x)
		transform.Translation.x = bounds.x + transform.Scale.x / 2;
	if (m_WalkingDirection.y == 1.0f && transform.Translation.y < bounds.w)
		transform.Translation.y = bounds.w - transform.Scale.y / 2;
	else if (m_WalkingDirection.y == -1.0f && transform.Translation.y > bounds.y)
		transform.Translation.y = bounds.y + transform.Scale.y / 2;
}

void Player::IdleAnimation(Stormlight::Timestep ts)
{
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	ac.m_FrameDuration = 0.3f;
	if(m_ShootingAnimation)
		Animation(this, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleFireSubTextures);
	else
		Animation(this, ts, ac.m_IdleTimeElapsed, ac.m_FrameDuration, ac.m_IdleIndex, ac.m_IdleSubTextures);
}

void Player::WalkForwardAnimation(Stormlight::Timestep ts)
{
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	ac.m_FrameDuration = 0.08f;
	if (m_ShootingAnimation)
		Animation(this, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkForwardFireSubTextures);
	else
		Animation(this, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkForwardSubTextures);
}

void Player::WalkSidewaysAnimation(Stormlight::Timestep ts)
{
	auto& ac = this->GetComponent<Stormlight::AnimationComponent>();
	ac.m_FrameDuration = 0.08f;
	if (m_ShootingAnimation)
		Animation(this, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkSidewaysFireSubTextures);
	else
		Animation(this, ts, ac.m_WalkTimeElapsed, ac.m_FrameDuration, ac.m_WalkIndex, ac.m_WalkSidewaysSubTextures);
}

bool Player::OnKeyPressed(Stormlight::KeyPressedEvent& e)
{
	this->GetComponent<Stormlight::PlayerComponent>().StartCombat = true;
	switch (e.GetKeyCode())
	{
	case SL_KEY_LEFT_SHIFT:
	{
		if (!m_FirstSkill->m_IsActive) m_FirstSkill->Use();
		break;
	}
	case SL_KEY_SPACE:
	{
		if (!m_SecondSkill->m_IsActive) m_SecondSkill->Use();
		break;
	}
	case SL_KEY_W:
	{
		m_WalkingDirection.y = 1;
		break;
	}
	case SL_KEY_S:
	{
		m_WalkingDirection.y = -1;
		break;
	}
	case SL_KEY_A:
	{
		m_WalkingDirection.x = -1;
		break;
	}
	case SL_KEY_D:
	{
		m_WalkingDirection.x = 1;
		break;
	}
	case SL_KEY_P:
	{
		Hazel::Audio::Play(m_AudioManager->m_UWU);
		break;
	}

	case SL_KEY_O:
	{
		Hazel::Audio::Pause(m_AudioManager->m_UWU);
		break;
	}

	case SL_KEY_I:
	{
		Hazel::Audio::Stop(m_AudioManager->m_UWU);
		break;
	}
	}

	return false;
}

bool Player::OnKeyReleased(Stormlight::KeyReleasedEvent& e)
{
	switch (e.GetKeyCode())
	{
	case SL_KEY_W:
	{
		m_WalkingDirection.y = 0;
		break;
	}
	case SL_KEY_S:
	{
		m_WalkingDirection.y = 0;
		break;
	}
	case SL_KEY_A:
	{
		m_WalkingDirection.x = 0;
		break;
	}
	case SL_KEY_D:
	{
		m_WalkingDirection.x = 0;
		break;
	}
	}
	return false;
}

bool Player::OnMouseButtonPressed(Stormlight::MouseButtonPressedEvent& e)
{
	this->GetComponent<Stormlight::PlayerComponent>().StartCombat = true;
	if (!m_AttackCooldown.IsTimerTicking() && e.GetMouseButton() == 0)
	{
		Hazel::Audio::Play(m_AudioManager->m_ReleseArrow);
		m_AttackCooldown.SetTimerDuration(1.1f * this->GetComponent<Stormlight::PlayerComponent>().AttackSpeed);
		m_AttackCooldown.StartTimer();
		m_ShootingAnimation = true;
	}
	if (!m_RaceSkill->m_IsActive && e.GetMouseButton() == 1)
	{
		m_RaceSkill->Use();
	}
	return false;
}

void Player::InitFirstSkill(int skillID)
{
	switch (skillID)
	{
	case 0:
		m_FirstSkill = new Blast(this->GetScene(), m_AudioManager);
		break;
	case 1:
		m_FirstSkill = new Hook(this->GetScene(), m_AudioManager);
		break;
	case 2:
		m_FirstSkill = new KnifeThrow(this->GetScene(), m_AudioManager);
		break;
	default:
		break;
	}
}

void Player::InitSecondSkill(int skillID)
{
	switch (skillID)
	{
	case 0:
		m_SecondSkill = new Blast(this->GetScene(), m_AudioManager);
		break;
	case 1:
		m_SecondSkill = new Hook(this->GetScene(), m_AudioManager);
		break;
	case 2:
		m_SecondSkill = new KnifeThrow(this->GetScene(), m_AudioManager);
		break;
	default:
		break;
	}
}