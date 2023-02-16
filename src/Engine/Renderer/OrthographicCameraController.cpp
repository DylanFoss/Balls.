#include "Engine/Renderer/OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

OrthographicCameraController::OrthographicCameraController(float orthoX, float orthoY)
	:m_OrthoX(orthoX), m_OrthoY(orthoY), m_Camera(-orthoX * m_Zoom, orthoX* m_Zoom, -orthoY * m_Zoom, orthoY* m_Zoom)
{
}

void OrthographicCameraController::Update(float deltaTime)
{
	if (Input::Get().IsKeyHeld(KC_KEY_A))
	{
		m_CameraPosition.x -= cos(glm::radians(0.0f)) * m_CameraTranslationSpeed;
		m_CameraPosition.y -= sin(glm::radians(0.0f)) * m_CameraTranslationSpeed;
	}
	else if  (Input::Get().IsKeyHeld(KC_KEY_D))
	{
		m_CameraPosition.x += cos(glm::radians(0.0f)) * m_CameraTranslationSpeed;
		m_CameraPosition.y += sin(glm::radians(0.0f)) * m_CameraTranslationSpeed;
	}

	if (Input::Get().IsKeyHeld(KC_KEY_W))
	{
		m_CameraPosition.x += -sin(glm::radians(0.0f)) * m_CameraTranslationSpeed;
		m_CameraPosition.y += cos(glm::radians(0.0f)) * m_CameraTranslationSpeed;
	}
	else if (Input::Get().IsKeyHeld(KC_KEY_S))
	{
		m_CameraPosition.x -= -sin(glm::radians(0.0f)) * m_CameraTranslationSpeed;
		m_CameraPosition.y -= cos(glm::radians(0.0f)) * m_CameraTranslationSpeed;
	}

	if (Input::Get().IsMousePressed(KC_MOUSE_BUTTON_MIDDLE))
	{
		m_LastHeldMousePosition = glm::vec2{ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second };
	}

	if (Input::Get().IsMouseHeld(KC_MOUSE_BUTTON_MIDDLE))
	{
		glm::vec2 LatestHeldMousePosition = glm::vec2(Input::Get().GetMousePos().first, Input::Get().GetMousePos().second);
		glm::vec2 ScreenPointVector = ScreenToWorldSpace(m_LastHeldMousePosition) - ScreenToWorldSpace(LatestHeldMousePosition);

		m_CameraPosition.x += ScreenPointVector.x;
		m_CameraPosition.y += ScreenPointVector.y;

		m_LastHeldMousePosition = glm::vec2{ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second };
	}

	if (Input::Get().IsMousePressed(KC_MOUSE_BUTTON_LEFT))
	{
		std::pair<int, int> pair = Input::Get().GetMousePos();
		glm::vec2 cursorPos = { pair.first, pair.second };

		glm::vec2 worldPos = ScreenToWorldSpace(cursorPos);

		std::cout << worldPos.x << ", " << worldPos.y << '\n';
	}

	if (Input::Get().GetMouseScroll() != 0)
	{

		auto pos = Input::Get().GetMousePos();
		glm::vec2 preZoomMousePosition = ScreenToWorldSpace(glm::vec2(pos.first, pos.second));

		//actual zoom performed here

		m_Zoom -= Input::Get().GetMouseScroll() * 0.01;
		m_Zoom = std::max(m_Zoom, m_MaxZoom);

		pos = Input::Get().GetMousePos();
		glm::vec2 postZoomMousePosition = ScreenToWorldSpace(glm::vec2(pos.first, pos.second));

		m_CameraPosition.x -= postZoomMousePosition.x - preZoomMousePosition.x;
		m_CameraPosition.y -= postZoomMousePosition.y - preZoomMousePosition.y;

		m_Camera.SetProjection(-m_OrthoX * m_Zoom, m_OrthoX * m_Zoom, -m_OrthoY * m_Zoom, m_OrthoY * m_Zoom);
	}

	if (Input::Get().IsKeyPressed(KC_KEY_R))
	{
		m_CameraPosition.x = 0.0f;
		m_CameraPosition.y = 0.0f;
		m_Zoom = 1;
		m_Camera.SetProjection(-m_OrthoX * m_Zoom, m_OrthoX * m_Zoom, -m_OrthoY * m_Zoom, m_OrthoY * m_Zoom);
	}

	m_Camera.SetPosition(m_CameraPosition);
	m_CameraTranslationSpeed = m_Zoom;
}

glm::vec2 OrthographicCameraController::ScreenToWorldSpace(glm::vec2 screenPoint)
{
	return glm::vec2(
		((screenPoint.x - m_OrthoX)) * m_Zoom + m_Camera.GetPosition().x,
		((-screenPoint.y + m_OrthoY)) * m_Zoom + m_Camera.GetPosition().y
	);
}

glm::vec2 OrthographicCameraController::WorldToScreenSpace(glm::vec2 worldPoint)
{
	return glm::vec2(
		static_cast<int>(((worldPoint.x + m_OrthoX) - m_Camera.GetPosition().x) / m_Zoom),
		static_cast<int>(((worldPoint.y + m_OrthoY) - m_Camera.GetPosition().y) / m_Zoom)
	);
}
