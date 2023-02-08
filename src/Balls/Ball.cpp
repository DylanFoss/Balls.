#include "Ball.h"

Ball::Ball(float positionX, float positionY, float radius)
	:m_Position({ positionX, positionY }), m_Velocity({ 0, 0 }), m_Acceleration({ 0, 0 }), m_Radius(radius)
{
	//m_Velocity.x -= rand() % 600 - 300;
	//m_Velocity.y -= rand() % 600 - 300;
}

void Ball::Update(float deltaTime)
{
	m_Velocity = m_Velocity + m_Acceleration * deltaTime;
	m_Position = m_Position + m_Velocity * deltaTime;


	if (m_Position.x >  500 + m_Radius)
		m_Position.x = -500 - m_Radius;
	if (m_Position.y >  500 + m_Radius)
		m_Position.y = -500 - m_Radius;
	if (m_Position.x < -500 - m_Radius)
		m_Position.x =  500 + m_Radius;
	if (m_Position.y < -500 - m_Radius)
		m_Position.y =  500 + m_Radius;
}