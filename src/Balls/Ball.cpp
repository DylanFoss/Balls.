#include "Ball.h"

Ball::Ball(float positionX, float positionY, float radius)
	:m_Position(positionX, positionY), m_Radius(radius)
{
	glm::vec2 radian = { rand() % 2 + 1, rand() % 2 + 1};
	m_Velocity.x -= cos(glm::radians(radian.x));
	m_Velocity.y -= sin(glm::radians(radian.y));
}

void Ball::Update(float deltaTime)
{
	m_Position += glm::vec2{ 0.05, 0.01 }  *m_Velocity;

	if (m_Position.x > 400 + m_Radius)
		while (m_Position.x > -400 - m_Radius) m_Position.x -= 800;
	if (m_Position.y > 400 + m_Radius)
		while (m_Position.y > -400 - m_Radius) m_Position.y -= 800;
	if (m_Position.x < -400 - m_Radius)
		while (m_Position.x < 400 + m_Radius) m_Position.x += 800;
	if (m_Position.y < -400 - m_Radius)
		while (m_Position.y < 400 + m_Radius) m_Position.y += 800;
}