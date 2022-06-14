#pragma once

#include "Stormlight.h"

static void Animation(Stormlight::Entity* entity, Stormlight::Timestep ts, float& timeElapsed, float frameDuration, size_t& index, std::vector<Stormlight::Ref<Stormlight::SubTexture2D>>& subTextures)
{
	timeElapsed += ts;

	if (timeElapsed > frameDuration)
	{
		timeElapsed = 0.0f;
		index = (index + 1) % subTextures.size();
		auto& src = entity->GetComponent<Stormlight::SpriteRendererComponent>();
		src.subTexture = subTextures[index];
	}
}