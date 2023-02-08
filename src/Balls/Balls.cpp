#include "Balls.h"
#include "pch.h"
#include <fstream>

#include "Core/MouseCodes.h"
#include "Core/KeyCodes.h"

#include "GLErrorHandler.h"

Balls::Balls(const std::string& name, uint32_t width, uint32_t height)
	:Application(name, width, height), m_WindowHalfHeight(m_Window->GetHeight() * 0.5f), m_WindowHalfWidth(m_Window->GetWidth() * 0.5f)
{
	float windowAspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
	//float multiplier = m_GameAspect / windowAspect;

	camera = OrthographicCameraController(m_WindowHalfWidth, m_WindowHalfHeight);
    //camera.SetMaxZoom(10.0f / static_cast<float>(gameHeight));

	Init();
}

Balls::~Balls()
{
	Shutdown();
}

Shader shader;
glm::mat4 model;

void Balls::Init()
{
	Renderer2D::Init();
	m_Window->SetVsync(false);

	// end of opengl

	m_Balls.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		m_Balls.push_back(Ball(rand() % 800 - 399, rand() % 800 - 399, rand() % 40 + 21));
	}
}

void Balls::Shutdown()
{
	Renderer2D::Shutdown();
}

void Balls::Update(float deltaTime)
{
	auto distance = [](glm::vec2 point1, glm::vec2 point2)
	{
		return glm::sqrt(((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y)));
	};

	auto distanceSqr = [](glm::vec2 point1, glm::vec2 point2)
	{
		return (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
	};


	auto circleOverlap = [&](Ball& ballA, Ball& ballB)
	{
		return fabs(distanceSqr(ballA.Position(), ballB.Position()) <= (ballA.Radius() + ballB.Radius()) * (ballA.Radius() + ballB.Radius()));
	};

	camera.Update(deltaTime);

	if (Input::Get().IsMouseHeld(KC_MOUSE_BUTTON_LEFT))
	{
		glm::vec2 pos = { Input::Get().GetMousePos().first, Input::Get().GetMousePos().second };
		m_Balls[0].SetPosition(camera.ScreenToWorldSpace(pos));
	}

	for (int i = 0; i < m_Balls.size(); i++)
	{
		m_Balls[i].Update(deltaTime);
	}

	// check collisions
	m_CollidingBalls.clear();
	for (int i = 0; i < m_Balls.size(); i++)
	{
		for (int j = 0; j < m_Balls.size(); j++)
		{
			if (j != i)
			{
				if (circleOverlap(m_Balls[i], m_Balls[j]))
					m_CollidingBalls.push_back(std::pair<Ball*, Ball*>(&m_Balls[i], &m_Balls[j]));
			}
		}
	}

	// resolve collisions

	for (auto pair : m_CollidingBalls)
	{
		Ball* ball = pair.first;
		Ball* target = pair.second;

		float fDistance = distance(ball->Position(), target->Position());
		float fOverlap = 0.5f * (fDistance - ball->Radius() - target->Radius());

		ball->SetPosX(ball->PosX() - fOverlap * (ball->PosX() - target->PosX()) / fDistance);
		ball->SetPosY(ball->PosY() - fOverlap * (ball->PosY() - target->PosY()) / fDistance);

		target->SetPosX(target->PosX() + fOverlap * (ball->PosX() - target->PosX()) / fDistance);
		target->SetPosY(target->PosY() + fOverlap * (ball->PosY() - target->PosY()) / fDistance);
	}

}

void Balls::Draw(float deltaTime)
{
	Renderer2D::StartFrame(camera.GetCamera());

	glClear(GL_COLOR_BUFFER_BIT);

	for (Ball &ball : m_Balls)
		Renderer2D::DrawQuad(ball.Position(), { ball.Radius() * 2, ball.Radius() * 2 });


	Renderer2D::EndFrame();
}