#include "FieldTrap.h"
#include "Animation.h"

void Musketeers::OnCreate()
{
	m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0];
	m_BulletTexture = Stormlight::Texture2D::Create("assets/game/textures/Bullet.png");
	m_FireTimer.StartTimer();
}

void Musketeers::OnUpdate(Stormlight::Timestep ts)
{
	if (m_FireTimer.OnUpdate(ts))
	{
		m_FireTimer.RestartTimer();
		OnDestroy();
		CreateBullets();
	}

	for (auto& bullet : m_GameScene->GetAllWithTag("Bullet"))
	{
		auto moveXY = bullet.GetComponent<Stormlight::MovementComponent>().XYSpeed;
		auto& bulletTrans = bullet.GetComponent<Stormlight::TransformComponent>();

		bulletTrans.Translation.x += moveXY.x * ts * 5.0f;
		bulletTrans.Translation.y += moveXY.y * ts * 5.0f;

		if (m_GameScene->CheckCollision(bullet, m_MainPlayer))
		{
			auto& hp = m_MainPlayer.GetComponent<Stormlight::HealthComponent>();
			hp.health -= 10.0f;
			hp.UpdateFullHealthBar();
			m_GameScene->DestroyEntity(bullet);
		}
	}
}

void Musketeers::OnDestroy()
{
	for (auto& bullet : m_GameScene->GetAllWithTag("Bullet"))
	{
		m_GameScene->DestroyEntity(bullet);
	}
}

void Musketeers::CreateBullets()
{
	glm::vec2 location;
	for (int i = 0; i < 5; i++)
	{
		int limit = Stormlight::GetRandomInt(2, 3);
		for (int j = 0; j < limit; j++)
		{
			location.x = 8.0f + i * 2.0f + Stormlight::GetRandomFloat();
			int offset = j * (limit == 2 ? 5 : 3);
			float intervalBound = limit == 2 ? 0.0f : -2.0f;
			location.y = Stormlight::GetRandomFloat(-5.0f + offset, intervalBound + offset);
			CreateBullet(location);
		}
	}
}

void Musketeers::CreateBullet(glm::vec2 location)
{
	auto& bullet = m_GameScene->CreateEntity("Bullet");

	auto& mc = bullet.AddComponent<Stormlight::MovementComponent>();
	mc.XYSpeed = { -1.0f, 0.0f };
	bullet.AddComponent<Stormlight::BoxCollider2DComponent>();
	auto& rb2dBullet = bullet.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dBullet.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	bullet.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_BulletTexture;

	auto& bulletTrans = bullet.GetComponent<Stormlight::TransformComponent>();
	bulletTrans.Translation.x = location.x;
	bulletTrans.Translation.y = location.y;

	bulletTrans.Scale.x = 0.3f;
	bulletTrans.Scale.y = 0.3f;

	m_GameScene->OnRuntimeAddEntity(bullet);
}

void ArcaneTrap::OnCreate()
{
	m_MainPlayer = m_GameScene->GetAllWithTag("MainCharacter")[0];
	auto headSpriteSheet = Stormlight::Texture2D::Create("assets/game/textures/ArcaneTrapHead.png");
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.0f , 0.0f }, { 0.125f, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 1, 0.0f }, { 0.125f * 2, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 2, 0.0f }, { 0.125f * 3, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 3, 0.0f }, { 0.125f * 4, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 4, 0.0f }, { 0.125f * 5, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 5, 0.0f }, { 0.125f * 6, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 6, 0.0f }, { 0.125f * 7, 1.0f }));
	m_HeadSubTextures.push_back(Stormlight::SubTexture2D::CreateFromRectangle(headSpriteSheet, { 0.125f * 7, 0.0f }, { 0.125f * 8, 1.0f }));
	

	m_BodyTexture = Stormlight::Texture2D::Create("assets/game/textures/ArcaneTrapBody.png");

	m_FireTimer.SetTimerDuration(20.0f);
	m_FireTimer.StartTimer();
	CreateArcaneTrap();
}

void ArcaneTrap::OnUpdate(Stormlight::Timestep ts)
{
	if (m_FireTimer.OnUpdate(ts))
	{
		m_FireTimer.RestartTimer();
		OnDestroy();
		CreateArcaneTrap();
	}

	for (auto& arcaneTendril : m_ArcaneTendrils)
	{
		arcaneTendril->OnUpdate(ts);

		if (arcaneTendril->m_TickDamageReady && m_GameScene->CheckCollision(arcaneTendril->Body, m_MainPlayer))
		{
			arcaneTendril->m_TickDamageReady = false;
			auto& hp = m_MainPlayer.GetComponent<Stormlight::HealthComponent>();
			hp.health -= 10.0f;
			hp.UpdateFullHealthBar();
		}
	}
}

void ArcaneTrap::OnDestroy()
{
	for (auto arcaneTendril : m_ArcaneTendrils)
	{
		arcaneTendril->OnDestroy();
	}
	m_ArcaneTendrils.clear();
}

void ArcaneTrap::CreateArcaneTrap()
{
	for (int i = 0; i < 5; i++)
	{
		ArcaneTendril* at = new ArcaneTendril();
		at->OnCreate(this, glm::vec2(6.0f - i * 3.0f, 4.0f));
		at->m_HoldTimer.SetTimerDuration(5.0f + i / 2.0f);
		at->m_AppearTimer.SetTimerDuration(1.0f + i / 2.0f);
		m_ArcaneTendrils.push_back(at);
	}
}

void ArcaneTrap::ArcaneTendril::OnCreate(ArcaneTrap* arcaneTrap, glm::vec2 location)
{
	m_ArcaneTrap = arcaneTrap;
	Head = m_ArcaneTrap->m_GameScene->CreateEntity("ArcaneTrap");

	auto& mc = Head.AddComponent<Stormlight::MovementComponent>();
	mc.XYSpeed = { 0.0f, -1.0f };
	Head.AddComponent<Stormlight::BoxCollider2DComponent>();
	auto& rb2dHead = Head.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dHead.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	Head.AddComponent<Stormlight::SpriteRendererComponent>().subTexture = (m_ArcaneTrap->m_HeadSubTextures)[0];
	auto& acHead = Head.AddComponent<Stormlight::AnimationComponent>();
	acHead.m_AttackSubTextures = m_ArcaneTrap->m_HeadSubTextures;
	acHead.m_FrameDuration = 0.05f;

	auto& headTrans = Head.GetComponent<Stormlight::TransformComponent>();
	headTrans.Translation.x = location.x;
	headTrans.Translation.y = location.y + 2.0f;

	m_ArcaneTrap->m_GameScene->OnRuntimeAddEntity(Head);

	Body = m_ArcaneTrap->m_GameScene->CreateEntity("ArcaneTrap");

	auto& mcBody = Body.AddComponent<Stormlight::MovementComponent>();
	Body.AddComponent<Stormlight::BoxCollider2DComponent>();
	auto& rb2dBody = Body.AddComponent<Stormlight::Rigidbody2DComponent>();
	rb2dBody.Type = Stormlight::Rigidbody2DComponent::BodyType::Dynamic;
	Body.AddComponent<Stormlight::SpriteRendererComponent>().Texture = m_ArcaneTrap->m_BodyTexture;

	auto& bodyTrans = Body.GetComponent<Stormlight::TransformComponent>();
	bodyTrans.Translation.x = location.x;
	bodyTrans.Translation.y = location.y + 50.0f + 2.0f;

	bodyTrans.Scale.x = 0.7f;
	bodyTrans.Scale.y = 100.0f;
	
	m_ArcaneTrap->m_GameScene->OnRuntimeAddEntity(Body);

	m_Sign = 1.0f;
	m_StartAttack = false;
	m_AppearTimer.StartTimer();
	m_DamageTickTimer.StartTimer();
}

void ArcaneTrap::ArcaneTendril::OnUpdate(Stormlight::Timestep ts)
{
	if (m_DamageTickTimer.OnUpdate(ts))
	{
		m_DamageTickTimer.RestartTimer();
		m_TickDamageReady = true;
	}
	if (m_AppearTimer.OnUpdate(ts))
	{
		m_AppearTimer.ResetTimer();
		auto& headTrans = Head.GetComponent<Stormlight::TransformComponent>();
		headTrans.Translation.y -= 3.0f;
		auto& bodyTrans = Body.GetComponent<Stormlight::TransformComponent>();
		bodyTrans.Translation.y -= 3.0f;
		m_HoldTimer.StartTimer();
	}
	if (m_HoldTimer.OnUpdate(ts))
	{
		m_HoldTimer.ResetTimer();
		m_MoveTimer.StartTimer();
		m_StartAttack = true;
	}

	if (m_StartAttack)
	{
		if (m_MoveTimer.OnUpdate(ts))
		{
			m_Sign = -m_Sign;
			m_Speed = 30.0f;
			m_MoveTimer.ResetTimer();
		}

		auto moveXY = Head.GetComponent<Stormlight::MovementComponent>().XYSpeed;
		auto& headTrans = Head.GetComponent<Stormlight::TransformComponent>();
		headTrans.Translation.x += moveXY.x * ts * m_Speed * m_Sign;
		headTrans.Translation.y += moveXY.y * ts * m_Speed * m_Sign;

		auto& bodyTrans = Body.GetComponent<Stormlight::TransformComponent>();

		bodyTrans.Translation.x += moveXY.x * ts * m_Speed * m_Sign;
		bodyTrans.Translation.y += moveXY.y * ts * m_Speed * m_Sign;
	}

	auto& acHead = Head.GetComponent<Stormlight::AnimationComponent>();
	Animation(&Head, ts, acHead.m_AttackTimeElapsed, acHead.m_FrameDuration, acHead.m_AttackIndex, acHead.m_AttackSubTextures);
}

void ArcaneTrap::ArcaneTendril::OnDestroy()
{
	m_ArcaneTrap->m_GameScene->DestroyEntity(Head);
	m_ArcaneTrap->m_GameScene->DestroyEntity(Body);
}
