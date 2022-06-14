#include "slpch.h"
#include "Components.h"

void Stormlight::HealthComponent::CreateHealthBars(float maxHP)
{
	maxHealth = maxHP;
	health = maxHealth;
	auto& transform = owner.GetComponent<Stormlight::TransformComponent>();
	m_FullBar = owner.GetScene()->CreateEntity("HealthBar");
	m_FullBar.AddComponent<Stormlight::SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	m_FullBar.AddComponent<Stormlight::MovementComponent>();
	auto& transformFull = m_FullBar.GetComponent<Stormlight::TransformComponent>();
	transformFull.Translation.x = transform.Translation.x;
	transformFull.Translation.y = transform.Translation.y + 0.6f;
	transformFull.Translation.z = 0.91f;
	transformFull.Scale.y = 0.1f;

	m_EmptyBar = owner.GetScene()->CreateEntity("HealthBar");
	m_EmptyBar.AddComponent<Stormlight::SpriteRendererComponent>(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_EmptyBar.AddComponent<Stormlight::MovementComponent>();
	auto& transformEmpty = m_EmptyBar.GetComponent<Stormlight::TransformComponent>();
	transformEmpty.Translation.x = transform.Translation.x;
	transformEmpty.Translation.y = transform.Translation.y + 0.6f;
	transformEmpty.Translation.z = 0.9f;
	transformEmpty.Scale.y = 0.1f;

	
}

void Stormlight::HealthComponent::UpdateFullHealthBar()
{
	//probably dont need this anymore
	owner.GetComponent<Stormlight::BoxCollider2DComponent>().Collided = false;
	auto& transFull = m_FullBar.GetComponent<Stormlight::TransformComponent>();
	transFull.Scale.x = health / maxHealth;
	offset = (1.0f - health / maxHealth) / 2.0f;
}

void Stormlight::HealthComponent::UpdateHealthBars()
{
	auto& transform = owner.GetComponent<Stormlight::TransformComponent>();
	auto& transformEmpty = m_EmptyBar.GetComponent<Stormlight::TransformComponent>();
	transformEmpty.Translation.x = transform.Translation.x;
	transformEmpty.Translation.y = transform.Translation.y + 0.6f;

	auto& transformFull = m_FullBar.GetComponent<Stormlight::TransformComponent>();
	transformFull.Translation.x = transform.Translation.x - offset;
	transformFull.Translation.y = transform.Translation.y + 0.6f;
}

void Stormlight::HealthComponent::DestroyHealthBars()
{
	owner.GetScene()->DestroyEntity(m_FullBar);
	owner.GetScene()->DestroyEntity(m_EmptyBar);
}

void Stormlight::BoxCollider2DComponent::HandleCollision(Entity& myEntity, Timestep ts)
{
	if (Collided)
	{
		auto& enemyTrans = myEntity.GetComponent<Stormlight::TransformComponent>();
		auto& otherTrans = collisionEntity.GetComponent<Stormlight::TransformComponent>();
		glm::vec2 direction = otherTrans.Get2DTranslate() - enemyTrans.Get2DTranslate();
		direction = glm::normalize(direction);
		enemyTrans.Translation.x -= direction.x * ts * 0.5f;
		enemyTrans.Translation.y -= direction.y * ts * 0.5f;
	}
}

void Stormlight::BoxCollider2DComponent::FindCollision(Entity& myEntity, const std::string& tag)
{
	Collided = false;
	
	auto otherEntities = myEntity.GetScene()->GetAllWithTag(tag);
	for (auto otherEntity : otherEntities)
	{
		if (myEntity != otherEntity)
		{
			if (myEntity.GetScene()->CheckCollision(myEntity, otherEntity))
			{
				Collided = true;
				collisionEntity = otherEntity;
			}
		}
	}
}

void Stormlight::PlayerComponent::GainConsumable()
{
	float random = Stormlight::GetRandomFloat();

	if(random < 0.3333333f)
		NumOfSpyglass += 1;
	else if(random >= 0.3333333f && random < 0.6666666f)
		NumOfWarpStone += 1;
	else
		NumOfHPpot += 1;
}
