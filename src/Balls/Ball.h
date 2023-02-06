#include "pch.h";
#include "glm/glm.hpp";
#include "glm/gtc/matrix_transform.hpp"

class Ball 
{
public:

	Ball(float positionX, float positionY, float radius);

	const inline glm::vec2 GetPosition() { return m_Position; }
	const inline float GetRadius() { return m_Radius; }

	void Update(float deltatime);

private:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
	float m_Radius;
};