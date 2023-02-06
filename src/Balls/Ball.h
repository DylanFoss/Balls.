#include "pch.h";
#include "glm/glm.hpp";

class Ball 
{
public:

	Ball(float positionX, float positionY, float radius);

private:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
	float m_Radius;
};