#include "Balls.h"
#include "Engine/pch.h"
#include <fstream>

#include "Engine/Core/MouseCodes.h"
#include "Engine/Core/KeyCodes.h"

#include "Engine/GLErrorHandler.h"

#include "Engine/Physics/PhysicsObject.h"

#include "Engine/Physics/Shapes/ShapeType.h"

#include "Engine/Physics/Shapes/Circle.h"

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

		if (glm::length(pos) + 40. < 300.)
		{
			m_World.CreateBall(pos, 40.);
		}
	}

	m_World.Update(deltaTime);
}

void Balls::Draw(float deltaTime)
{
	Renderer2D::StartFrame(camera.GetCamera());

	glClear(GL_COLOR_BUFFER_BIT);

	Renderer2D::DrawQuad({ 0,0 }, { 300 * 2, 300 * 2 });

	for (PhysicsObject* item : m_World.m_Physics)
	{
		Renderer2D::DrawQuad(item->GetPosition(), { 40 * 2, 40 * 2 });
		Renderer2D::DrawLine(item->GetPosition(), item->GetPosition() + glm::normalize(item->GetVelocity())*40.f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	Renderer2D::EndFrame();
}