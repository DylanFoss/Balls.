#include "Ball.h"

Ball::Ball(float positionX, float positionY, float radius)
	:m_Position({ positionX, positionY }), m_Velocity({ 0, 0 }), m_Acceleration({ 0, 0 }), m_Radius(radius)
{
}

void Ball::Update(float deltaTime)
{

	if (m_Position.x > 400 + m_Radius)
		m_Position.x = -400 - m_Radius;
	if (m_Position.y > 400 + m_Radius)
		m_Position.y = -400 - m_Radius;
	if (m_Position.x < -400 - m_Radius)
		m_Position.x = 400 + m_Radius;
	if (m_Position.y < -400 - m_Radius)
		m_Position.y = 400 + m_Radius;
}