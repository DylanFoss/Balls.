#include "Balls.h"
#include "Engine/pch.h"
#include <fstream>

#include "Engine/Core/MouseCodes.h"
#include "Engine/Core/KeyCodes.h"

#include "Engine/GLErrorHandler.h"


Balls::Balls(const std::string& name, uint32_t width, uint32_t height)
	:Application(name, width, height), m_WindowHalfHeight(m_Window->GetHeight() * 0.5f), m_WindowHalfWidth(m_Window->GetWidth() * 0.5f), m_World(World({0., -1000.}))
{
	float windowAspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());

	camera = OrthographicCameraController(m_WindowHalfWidth, m_WindowHalfHeight);

	Init();
	glLineWidth(1.0);
}

Balls::~Balls()
{
	Shutdown();
}

void Balls::Init()
{
	//intialise openGL
	Renderer2D::Init();

	m_Window->SetVsync(false);
}

void Balls::Shutdown()
{
	Renderer2D::Shutdown();
}

void Balls::Update(float deltaTime)
{
	camera.Update(deltaTime);

	if (Input::Get().IsMousePressed(KC_MOUSE_BUTTON_LEFT))
	{
		glm::vec2 pos = camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second });

		if (glm::length(pos) < 300.)
		{
			m_World.CreateBall(pos, rand() % 30 + 5, { (rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f, 1.0f });
		}
	}

	if (Input::Get().IsMouseHeld(KC_MOUSE_BUTTON_RIGHT))
	{
		glm::vec2 pos = camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second });

		float scalar = glm::length(pos) < 300. ? 300. : glm::length(pos);
		glm::vec2 normalPos = glm::normalize(pos);
		m_World.SetGravity( normalPos*scalar );

	} else if (Input::Get().IsMouseReleased(KC_MOUSE_BUTTON_RIGHT))
	{
		m_World.SetGravity({ 0., -1000. });
	}

	m_World.Update(deltaTime);
}

void Balls::Draw(float deltaTime)
{
	Renderer2D::StartFrame(camera.GetCamera());

	glClear(GL_COLOR_BUFFER_BIT);

	Renderer2D::DrawQuad({ 0,0 }, { 300 * 2, 300 * 2 }, {0.0f, 0.0f, 0.0f, 1.0f});

	for (EntityID item : m_World.m_PhysicsObjects.m_Entities)
	{

		Renderer2D::DrawQuad(m_World.m_PhysicsObjects.m_VerletBodies[item].m_Position,
			{ m_World.m_PhysicsObjects.m_BallColliders[item].m_Radius * 2, m_World.m_PhysicsObjects.m_BallColliders[item].m_Radius * 2 },
			m_World.m_PhysicsObjects.m_RenderData[item].m_Color);
		Renderer2D::DrawLine(m_World.m_PhysicsObjects.m_VerletBodies[item].m_Position, m_World.m_PhysicsObjects.m_VerletBodies[item].m_Position + glm::normalize(m_World.m_PhysicsObjects.m_VerletBodies[item].m_Velocity)* m_World.m_PhysicsObjects.m_BallColliders[item].m_Radius, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	Renderer2D::EndFrame();
}