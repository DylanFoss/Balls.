#include "Ball.h"

Ball::Ball(float positionX, float positionY, float radius, int ID)
	:m_Position({ positionX, positionY }), m_Velocity({ 0, 0 }), m_Acceleration({ 0, 0 }), m_Radius(radius), m_Mass(radius* radius), m_InvMass(1.0f / m_Mass), m_Force({0,0}), m_ID(ID)
{
	//m_Velocity.x -= rand() % 600 - 300;
	//m_Velocity.y -= rand() % 600 - 30 
}

void Ball::Update(float deltaTime)
{
	//AddImpulse(-m_Velocity * m_Mass, deltaTime);
	m_Acceleration += -m_Velocity * 0.8f;

	if (glm::abs(m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y) < 0.005f)
		m_Velocity = {0.0f, 0.0f};

	//m_Acceleration = m_Force * m_InvMass;
	m_Velocity += m_Acceleration * deltaTime;
	//m_Velocity += m_Acceleration;
	m_Position += m_Velocity * deltaTime;
	m_Acceleration = { 0.0f, 0.0f };

	if (m_Position.x >  500 + m_Radius)
		m_Position.x = -500 - m_Radius;
	if (m_Position.y >  500 + m_Radius)
		m_Position.y = -500 - m_Radius;
	if (m_Position.x < -500 - m_Radius)
		m_Position.x =  500 + m_Radius;
	if (m_Position.y < -500 - m_Radius)
		m_Position.y =  500 + m_Radius;
}