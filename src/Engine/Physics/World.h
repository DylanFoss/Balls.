#pragma once
#include "glm/glm.hpp"
#include "Engine/pch.h"

//#include "Engine/Physics/Collision/Collision.h"

typedef size_t EntityID;

struct Manifold
{
	EntityID a;
	EntityID b;

	glm::vec2 collisionNormal;
	float overlap;

	bool collision = true;
};

//struct Transform
//{
//	glm::vec2 m_Position;
//};

struct VerletBody
{
	glm::vec2 m_Position;
	glm::vec2 m_OldPosition;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Velocity;
};

struct BodyData
{
	float m_Mass;
	float m_InvMass;
};

struct BallCollider
{
	float m_Radius;
};


struct PhysicsObjects
{
	enum
	{
		kFlagVerletObject = 1 << 0,
		kFlagBallCollider = 1 << 1,
	};

	std::vector<EntityID> m_Entities;
	std::vector<VerletBody> m_VerletBodies;
	std::vector<BallCollider> m_BallColliders;

	std::vector<int> m_Flags;

	EntityID CreatePhysicsObject()
	{
		//m_Positions.emplace_back(Transform());
		m_BallColliders.emplace_back(BallCollider());
		m_VerletBodies.emplace_back(VerletBody());

		m_Entities.emplace_back(0);

		return static_cast<EntityID>(m_Entities.size() - 1);
	}

	void Reserve(size_t n)
	{
		//m_PhysicsObjects.m_Positions.reserve(n);
		m_Entities.reserve(n);
		m_BallColliders.reserve(n);
		m_VerletBodies.reserve(n);
	}
};

//struct VerletSystem
//{
//	std::vector<EntityID> entities;
//
//	void AddObjectToSystem(EntityID id)
//	{
//		entities.emplace_back(id);
//	}
//
//	void UpdateSystem(float deltaTime)
//	{
//		for (EntityID id : entities)
//		{
//			Transform& transform = m_PhysicsObjects.m_Positions[id];
//			BallCollider& ballCollider = m_PhysicsObjects.m_BallColliders[id];
//			VerletBody& verletBody = m_PhysicsObjects.m_Bodies[id];
//
//			verletBody.m_Velocity = transform.m_Position - verletBody.m_OldPosition;
//			verletBody.m_OldPosition = transform.m_Position;
//			transform.m_Position = transform.m_Position + verletBody.m_Velocity + verletBody.m_Acceleration * deltaTime * deltaTime;
//			verletBody.m_Acceleration = { 0.0f, 0.0f };
//		}
//	}
//
//};
//
//struct BroadPhaseSystem
//{
//	std::vector<EntityID> entities;
//
//	void AddObjectToSystem(EntityID id)
//	{
//		entities.emplace_back(id);
//	}
//
//	void UpdateSystem()
//	{
//		EntityID lastId;
//		for (EntityID id : entities)
//		{
//			
//			for (int j = i + 1; j < m_Physics.size(); j++)
//			{
//				PhysicsObject* rhs = m_Physics[j];
//
//				Manifold m = Collisions::CircleVSCircle();
//
//				if (m.collision)
//				{
//					m.a = lhs;
//					m.b = rhs;
//					m_Collisions.emplace_back(m);
//				}
//
//			}
//		}
//	}
//};
//
//struct NarrowPhaseSystem
//{
//	std::vector<EntityID> entities;
//
//	void AddObjectToSystem(EntityID id)
//	{
//		entities.emplace_back(id);
//	}
//
//	void UpdateSystem()
//	{
//		for (Manifold& pair : m_Collisions)
//		{
//			//divide each body's individual mass by the combined mass
//			//to work out which should be offset more.
//			float combinedMass = pair.a->GetMass() + pair.b->GetMass();
//
//			//glm::vec2 offset = 0.5f * overlap * collisionNormal;
//			glm::vec2 offset = pair.overlap * pair.collisionNormal;
//
//			pair.a->OffsetPosition((pair.b->GetMass() / combinedMass) * offset);
//			pair.b->OffsetPosition((pair.a->GetMass() / combinedMass) * -offset);
//
//			//let's not delete the ptrs
//			pair.a = nullptr;
//			pair.b = nullptr;
//		}
//	}
//};

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

	void BroadPhase();
	void NarrowPhase();

	EntityID CreateBall(const glm::vec2 pos, float radius);


	void SetGravity(const glm::vec2& gravity) { m_Gravity = gravity; }

	PhysicsObjects m_PhysicsObjects;
	std::vector<Manifold> m_Collisions;

private:
	glm::vec2 m_Gravity;

	float m_StepDuration;
	float m_StepTime;
	float m_SubSteps;

	//this is strictly for fun purposes only
	float BallCannonDelay = 0.2;
	float BallCannonCounter = 0;
	float BallCannonShots = 3000;
	void BallCannon(float deltaTime);
};