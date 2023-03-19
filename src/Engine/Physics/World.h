#pragma once
#include "glm/glm.hpp"
#include "Engine/pch.h"

#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/Collision/Collision.h"


constexpr int s_NumBalls = 1000;

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

	void BruteForce();
	void SpatialGrid();

	EntityID CreateBall(const glm::vec2 pos, float radius, const glm::vec4 color);

	void SetGravity(const glm::vec2& gravity) { m_Gravity = gravity; }

	PhysicsObjects m_PhysicsObjects;

private:
	glm::vec2 m_Gravity;

	float m_StepDuration;
	float m_StepTime;
	float m_SubSteps;

	//this is strictly for fun purposes only
	float BallCannonDelay = 0.2;
	float BallCannonCounter = 0;
	float BallCannonShots = s_NumBalls;
	void BallCannon(float deltaTime);
};