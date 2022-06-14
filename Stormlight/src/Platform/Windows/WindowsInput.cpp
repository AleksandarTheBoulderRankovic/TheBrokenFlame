#include "slpch.h"
#include "Stormlight/Core/Input.h"

#include "Stormlight/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Stormlight {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return {(float)x, (float)y};
	}
	std::pair<float, float> Input::GetRelativeMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		float mousePixelPosition_x = (float)x - Application::Get().GetWindow().GetWidth() / 2.0f;
		float mousePixelPosition_y = -(float)y + Application::Get().GetWindow().GetHeight() / 2.0f;
		float mouseActualPosition_x = (17.7777778f * mousePixelPosition_x) / Application::Get().GetWindow().GetWidth();
		float mouseActualPosition_y = (10.0f * mousePixelPosition_y) / Application::Get().GetWindow().GetHeight();
		return {mouseActualPosition_x, mouseActualPosition_y};
	}

	glm::vec2 Input::GetMousePositionVec()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	glm::vec2 Input::GetRelativeMousePositionVec(glm::vec3& relevantObject)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		float posX = (float)x - Application::Get().GetWindow().GetWidth() / 2.0f - relevantObject.x * (Application::Get().GetWindow().GetWidth() / 20.0f);
		float posY = -(float)y + Application::Get().GetWindow().GetHeight() / 2.0f - relevantObject.y * (Application::Get().GetWindow().GetHeight() / 20.0f);
		return { posX, posY };
	}

	glm::vec3 Input::GetRelativeMousePositionVec3(glm::vec3& relevantObject)
	{
		glm::vec2 coords = GetRelativeMousePositionVec(relevantObject);
		return { coords.x, coords.y, 0.0f };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}