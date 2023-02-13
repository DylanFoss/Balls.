#include "Balls.h"
#include "Engine/pch.h"
#include <fstream>

#include "Engine/Core/MouseCodes.h"
#include "Engine/Core/KeyCodes.h"

#include "Engine/GLErrorHandler.h"

#include "glm/common.hpp"

Balls::Balls(const std::string& name, uint32_t width, uint32_t height)
	:Application(name, width, height), m_WindowHalfHeight(m_Window->GetHeight() * 0.5f), m_WindowHalfWidth(m_Window->GetWidth() * 0.5f)
{
	float windowAspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
	//float multiplier = m_GameAspect / windowAspect;

	camera = OrthographicCameraController(m_WindowHalfWidth, m_WindowHalfHeight);
    //camera.SetMaxZoom(10.0f / static_cast<float>(gameHeight));

	Init();
	glLineWidth(1.0);
}

Balls::~Balls()
{
	Shutdown();
}

Shader shader;
glm::mat4 model;

void Balls::Init()
{
	//intialise openGL
	Renderer2D::Init();

	m_Window->SetVsync(false);

	m_Balls.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		m_Balls.push_back(Ball(rand() % 800 - 399, rand() % 800 - 399, rand() % 40 + 21, i));
	}
}

void Balls::Shutdown()
{
	Renderer2D::Shutdown();
}

void Balls::Update(float deltaTime)
{
	//TODO: move lambda functions into their own ubiquitous methods
	//TODO: use glm::vectors over single points.

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
		return distanceSqr(ballA.Position(), ballB.Position()) <= (ballA.Radius() + ballB.Radius()) * (ballA.Radius() + ballB.Radius());
	};

	camera.Update(deltaTime);

	if (Input::Get().IsMouseHeld(KC_MOUSE_BUTTON_LEFT))
	{
		if(m_SelectedBall)
		{
			glm::vec2 pos = camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second });

			auto mag = pos - m_SelectedBall->Position();
			m_SelectedBall->SetVelocity(mag * 20.0f);
		}
	}

	if (Input::Get().IsMousePressed(KC_MOUSE_BUTTON_RIGHT) || Input::Get().IsMousePressed(KC_MOUSE_BUTTON_LEFT))
	{
		glm::vec2 pos = camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second });

		for (Ball& ball : m_Balls)
		{
			if (distance(pos, ball.Position()) < ball.Radius())
			{
				m_SelectedBall = &ball;
				break;
			}
		}
	}

	if (Input::Get().IsMouseReleased(KC_MOUSE_BUTTON_RIGHT))
	{
		if (m_SelectedBall)
		{
			glm::vec2 pos = camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second });
			m_SelectedBall->SetVelocity(m_SelectedBall->Position() - pos);

			m_SelectedBall = nullptr;
		}
	}

	if (Input::Get().IsMouseReleased(KC_MOUSE_BUTTON_LEFT))
	{
		if (m_SelectedBall)
		{
			m_SelectedBall = nullptr;
		}
	}

	for (int i = 0; i < m_Balls.size(); i++)
	{
		m_Balls[i].Update(deltaTime);
	}

	// check collisions
	m_Pairs.clear();
	for (int i = 0; i < m_Balls.size(); i++)
	{
		for (int j = 0; j < m_Balls.size(); j++)
		{
			if (m_Balls[i].ID() != m_Balls[j].ID())
			{
				if (circleOverlap(m_Balls[i], m_Balls[j]))
				{
					m_Pairs.insert(std::pair<Ball&, Ball&>(m_Balls[i], m_Balls[j]));
				}
			}
		}
	}

	for (auto& pair : m_Pairs)
	{
		Ball& ball = pair.first;
		Ball& target = pair.second;

		//static resolution
		if (circleOverlap(m_Balls[ball.ID()], m_Balls[target.ID()]))
		{
			float fDistance = distance(m_Balls[ball.ID()].Position(), m_Balls[target.ID()].Position());
			float fOverlap = 0.5f * (fDistance - m_Balls[ball.ID()].Radius() - m_Balls[target.ID()].Radius());

			m_Balls[ball.ID()].SetPosX(m_Balls[ball.ID()].PosX() - fOverlap * (m_Balls[ball.ID()].PosX() - m_Balls[target.ID()].PosX()) / fDistance);
			m_Balls[ball.ID()].SetPosY(m_Balls[ball.ID()].PosY() - fOverlap * (m_Balls[ball.ID()].PosY() - m_Balls[target.ID()].PosY()) / fDistance);

			m_Balls[target.ID()].SetPosX(m_Balls[target.ID()].PosX() + fOverlap * (m_Balls[ball.ID()].PosX() - m_Balls[target.ID()].PosX()) / fDistance);
			m_Balls[target.ID()].SetPosY(m_Balls[target.ID()].PosY() + fOverlap * (m_Balls[ball.ID()].PosY() - target.PosY()) / fDistance);
		}
	}

	for (auto& pair : m_Pairs)
	{
		Ball& ball = pair.first;
		Ball& target = pair.second;

		//dynamic resolution

		glm::vec2 normal = glm::normalize(target.Position()- m_Balls[ball.ID()].Position());
		glm::vec2 tangent = { -normal.y, normal.x };

		auto ballTan = glm::dot(m_Balls[ball.ID()].Velocity(), tangent);
		auto targetTan = glm::dot(m_Balls[target.ID()].Velocity(), tangent);

		auto ballNorm = glm::dot(m_Balls[ball.ID()].Velocity(), normal);
		auto targetNorm = glm::dot(m_Balls[target.ID()].Velocity(), normal);


		m_Balls[ball.ID()].SetVelocity(ballTan* tangent);
		m_Balls[target.ID()].SetVelocity(targetTan*tangent);

		auto m1 = (ballNorm   * (m_Balls[ball.ID()].Mass() - m_Balls[target.ID()].Mass()) + 2.0f * m_Balls[target.ID()].Mass() * targetNorm)  / (ball.Mass() + m_Balls[target.ID()].Mass());
		auto m2 = (targetNorm * (m_Balls[target.ID()].Mass() - m_Balls[ball.ID()].Mass()) + 2.0f * m_Balls[ball.ID()].Mass()   * ballNorm)    / (m_Balls[ball.ID()].Mass() + m_Balls[target.ID()].Mass());

		m_Balls[ball.ID()].SetVelocity(ballTan * tangent + normal * m1);
		m_Balls[target.ID()].SetVelocity(targetTan * tangent + normal * m2);
	}

}

void Balls::Draw(float deltaTime)
{
	Renderer2D::StartFrame(camera.GetCamera());

	glClear(GL_COLOR_BUFFER_BIT);

	for (Ball& ball : m_Balls)
	{
		Renderer2D::DrawQuad(ball.Position(), { ball.Radius() * 2, ball.Radius() * 2 });
		Renderer2D::DrawLine(ball.Position(), ball.Position() + glm::normalize(ball.Velocity()) * ball.Radius(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	for (auto& pair : m_Pairs)
		Renderer2D::DrawLine(pair.first.Position(), pair.second.Position(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	if(m_SelectedBall)
		Renderer2D::DrawLine(m_SelectedBall->Position(), camera.ScreenToWorldSpace({ Input::Get().GetMousePos().first, Input::Get().GetMousePos().second }), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	Renderer2D::EndFrame();
}