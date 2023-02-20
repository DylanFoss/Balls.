#pragma once
#include "glm/glm.hpp"
#include "Engine/pch.h"

#include "Engine/Physics/Shapes/ShapePCH.h"

class PhysicsObject;


class World
{
public:

	World(const glm::vec2& gravity);
	~World();

	void Update(float deltaTime);
	void UpdatePositions(float deltaTime);
	void ApplyGravity();
	void ApplyConstraints();
	void SolveCollisions();
	
	void CreatePhysicsObject(const glm::vec2 pos, Shape* shape);
	void CreateBall(const glm::vec2 pos, float radius);

	std::vector<PhysicsObject*> m_Physics;

private:
	glm::vec2 m_Gravity;

	//float m_NumSteps;
	float m_StepDuration;
	float m_StepTime = 0;
};