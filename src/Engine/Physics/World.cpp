#include "World.h"

//#include "Engine/Physics/Collision/Collisionpch.h"

#include "Engine/Physics/PartitionGrid.h"

World::World(const glm::vec2& gravity)
	:m_Gravity(gravity), m_StepDuration(1.f/60.f), m_StepTime(0), m_SubSteps(8)
{
	m_PhysicsObjects.Reserve(3000); 
}

World::~World()
{
}

void World::Update(float deltaTime)
{
	m_StepTime += deltaTime;

	if (m_StepTime > m_StepDuration)
	{
		for (int i = 0; i < m_SubSteps; i++)
		{
			if (BallCannonShots > 0)
			{
				BallCannon(m_StepTime);
			}

			ApplyGravity();
			ApplyConstraints();
			SolveCollisions();
			UpdatePositions(m_StepDuration / m_SubSteps);
		}
		m_StepTime = 0;
	}
}

void World::UpdatePositions(float deltaTime)
{
	for (EntityID id : m_PhysicsObjects.m_Entities)
	{
		VerletBody& verletBody = m_PhysicsObjects.m_VerletBodies[id];

		verletBody.m_Velocity = verletBody.m_Position - verletBody.m_OldPosition;
		verletBody.m_OldPosition = verletBody.m_Position;
		verletBody.m_Position = verletBody.m_Position + verletBody.m_Velocity + verletBody.m_Acceleration * deltaTime * deltaTime;
		verletBody.m_Acceleration = { 0.0f, 0.0f };
	}
}

void World::ApplyGravity()
{
	for (EntityID id : m_PhysicsObjects.m_Entities)
	{
		VerletBody& verletBody = m_PhysicsObjects.m_VerletBodies[id];

		verletBody.m_Acceleration = verletBody.m_Acceleration + m_Gravity;
	}
}

void World::ApplyConstraints()
{
	for (EntityID id : m_PhysicsObjects.m_Entities)
	{
		VerletBody& verletBody = m_PhysicsObjects.m_VerletBodies[id];
		BallCollider& collider = m_PhysicsObjects.m_BallColliders[id];

		const glm::vec2 toObj = verletBody.m_Position - glm::vec2{ 0.f, 0.f };
		const float dist = glm::length(toObj);
		if (dist > 300.f - collider.m_Radius)
		{
			verletBody.m_Position = (toObj/dist) * (300.f - collider.m_Radius);
		}
	}
}

//to be seperated into their own classes, but fun to mess around with now!
void World::SolveCollisions()
{
	//BroadPhase();
	NarrowPhase();
}

void World::BroadPhase()
{
}

void World::NarrowPhase()
{
	for (int i = 0; i < m_PhysicsObjects.m_Entities.size(); i++)
	{
		EntityID lhs = m_PhysicsObjects.m_Entities[i];
		VerletBody& verletBodyA = m_PhysicsObjects.m_VerletBodies[lhs];

		for (int j = i + 1; j < m_PhysicsObjects.m_Entities.size(); j++)
		{
			EntityID rhs = m_PhysicsObjects.m_Entities[j];
			VerletBody& verletBodyB = m_PhysicsObjects.m_VerletBodies[rhs];

			BallCollider& colliderA = m_PhysicsObjects.m_BallColliders[lhs];
			BallCollider& colliderB = m_PhysicsObjects.m_BallColliders[rhs];

			glm::vec2 collisionAxis = verletBodyA.m_Position - verletBodyB.m_Position;
			float radii = (colliderA.m_Radius + colliderB.m_Radius);

			//use cheaper squared distance first
			float sqrDistance = glm::dot(collisionAxis, collisionAxis);

			if (sqrDistance < radii * radii)
			{
				float distance = glm::length(collisionAxis);
				glm::vec2 collisionNormal = glm::normalize(collisionAxis);
				float overlap = radii - distance;

				constexpr float restitution = 0.75f;
				//divide each body's individual mass by the combined mass
				//to work out which should be offset more.
				//float combinedMass = pair.a + pair.b;

				glm::vec2 offset = 0.5f * overlap * collisionNormal * restitution;
				//glm::vec2 offset = pair.overlap * pair.collisionNormal;

				//pair.a->OffsetPosition((pair.b->GetMass() / combinedMass) * offset);
				//pair.b->OffsetPosition((pair.a->GetMass() / combinedMass) * -offset);

				m_PhysicsObjects.m_VerletBodies[lhs].m_Position += offset;
				m_PhysicsObjects.m_VerletBodies[rhs].m_Position += -offset;
			}
		}
	}
}


EntityID World::CreateBall(const glm::vec2 pos, float radius)
{
	EntityID id = m_PhysicsObjects.CreatePhysicsObject();

	m_PhysicsObjects.m_Entities[id]						=  id;

	m_PhysicsObjects.m_BallColliders[id].m_Radius		= radius;
	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagBallCollider;

	m_PhysicsObjects.m_VerletBodies[id].m_Position		= pos;
	m_PhysicsObjects.m_VerletBodies[id].m_OldPosition	= pos;
	m_PhysicsObjects.m_VerletBodies[id].m_Acceleration	= { 0.0f, 0.0f };
	m_PhysicsObjects.m_VerletBodies[id].m_Velocity		= { 0.0f, 0.0f };
	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagVerletObject;


	return id;
}

float t = 0;
void World::BallCannon(float deltaTime)
{
	BallCannonCounter += deltaTime;
	if (BallCannonCounter > BallCannonDelay)
	{
		EntityID id = CreateBall({ 0,260 }, 5);

		t += deltaTime;
		const float angle = 1.0f * sin(t) + 3.14159265358979323846 * 0.5f;
		m_PhysicsObjects.m_VerletBodies[id].m_OldPosition = glm::vec2{ cos(angle), sin(angle) } + m_PhysicsObjects.m_VerletBodies[id].m_Position;


		BallCannonCounter = 0;
		BallCannonShots--;
	}
}
