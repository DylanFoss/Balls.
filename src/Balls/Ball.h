#include "Engine/pch.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Ball 
{
public:

	Ball(float positionX, float positionY, float radius, int ID);

	const inline glm::vec2 Position() { return m_Position; }
	const inline glm::vec2 Velocity() { return m_Velocity; }
	const inline float PosX() { return m_Position.x; }
	const inline float PosY() { return m_Position.y; }
	const inline float Radius() { return m_Radius; }
	const inline float Mass() { return m_Mass; }
	const inline float ID() { return m_ID; }

	inline void SetPosition(glm::vec2 position) { m_Position = position; }
	inline void SetVelocity(glm::vec2 velocity) { m_Velocity = velocity; }
	inline void SetAcceleration(glm::vec2 acc) { m_Acceleration = acc; }
	inline void SetPosX(float x) { m_Position.x = x; }
	inline void SetPosY(float y) { m_Position.y = y; }

	//inline void AddImpulse(glm::vec2 force, float dt) { m_Acceleration += force * dt * m_InvMass; }
	//inline void AddForce(glm::vec2 force, float dt) { m_Acceleration += force * dt*dt * m_InvMass; }
	//inline void AddAcceleration(glm::vec2 force, float dt) { m_Acceleration += force * dt * dt; }
	//inline void AddVelocity(glm::vec2 force, float dt) { m_Acceleration += force * dt; }

	void Update(float deltatime);

private:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Force;
	float m_Radius;
	float m_Mass;
	float m_InvMass;

	int m_ID;
};