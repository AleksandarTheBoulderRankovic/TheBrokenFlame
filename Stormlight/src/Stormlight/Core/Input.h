#pragma once
#include "Stormlight/Core/Core.h"
#include <glm/glm.hpp>

namespace Stormlight {
	class Input 
	{
	public:
		
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static glm::vec2 GetMousePositionVec();
		static std::pair<float, float> GetRelativeMousePosition();
		static glm::vec2 GetRelativeMousePositionVec(glm::vec3& relevantObject);
		static glm::vec3 GetRelativeMousePositionVec3(glm::vec3& relevantObject);
		static float GetMouseX();
		static float GetMouseY();
	};
}
