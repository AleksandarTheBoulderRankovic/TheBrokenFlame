#include "slpch.h"
#include "SubTexture2D.h"

namespace Stormlight {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
	Ref<SubTexture2D> SubTexture2D::CreateFromCoordinates(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{

		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth() , (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth() , ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
	Ref<SubTexture2D> SubTexture2D::CreateFromRectangle(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
	{
		glm::vec2 min1 = min;
		glm::vec2 max1 = max;
		return CreateRef<SubTexture2D>(texture, min1, max1);
	}
}
