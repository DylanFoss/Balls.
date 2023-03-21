#include "World.h"

//#include "Engine/Physics/Collision/Collisionpch.h"

#include "Engine/Physics/PartitionGrid.h"
#include "Engine/Utils/Timer.h"

World::World(const glm::vec2& gravity)
	:m_Gravity(gravity), m_StepDuration(1.f/60.f), m_StepTime(0), m_SubSteps(8)
{
	m_PhysicsObjects.Reserve(s_NumBalls);
	CreateKinematicBall({ 0.0f, 0.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });
	CreateKinematicBall({ -100.0f, -130.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });
	CreateKinematicBall({ 100.0f, -130.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });

	CreateKinematicBall({ 50.0f, 70.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });
	CreateKinematicBall({ -50.0f, 70.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });

	CreateKinematicBall({ 130.0f, 0.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });
	CreateKinematicBall({ -130.0f, 0.0f }, 30.f, { 1.0f, 0.0f, 1.0f, 1.0f });
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
			SolveCollisions();
			ApplyConstraints();
			UpdatePositions(m_StepDuration / m_SubSteps);
		}
		m_StepTime = 0;
	}
}

void World::UpdatePositions(float deltaTime)
{
	for (EntityID id : m_ActiveObjects)
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
	for (EntityID id : m_ActiveObjects)
	{
		VerletBody& verletBody = m_PhysicsObjects.m_VerletBodies[id];

		verletBody.m_Acceleration = verletBody.m_Acceleration + m_Gravity;
	}
}

void World::ApplyConstraints()
{
	constexpr float constraintRadius = 300.f;

	for (EntityID id : m_ActiveObjects)
	{
		VerletBody& verletBody = m_PhysicsObjects.m_VerletBodies[id];
		BallCollider& collider = m_PhysicsObjects.m_BallColliders[id];

		const glm::vec2 toObj = verletBody.m_Position - glm::vec2{ 0.f, 0.f };
		const float dist = glm::length(toObj);
		if (dist > constraintRadius - collider.m_Radius)
		{
			verletBody.m_Position = (toObj/dist) * (constraintRadius - collider.m_Radius);
		}
	}
}

//to be seperated into their own classes, but fun to mess around with now!
void World::SolveCollisions()
{
	BruteForce();
	//SpatialGrid();
}

void World::BruteForce()
{
	for (int i = 0; i < m_LiveObjects.size(); i++)
	{
		EntityID lhs = m_PhysicsObjects.m_Entities[i];
		VerletBody& verletBodyA = m_PhysicsObjects.m_VerletBodies[lhs];

		for (int j = i+1; j < m_LiveObjects.size(); j++)
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
				float combinedMass = m_PhysicsObjects.m_MassData[lhs].m_Mass + m_PhysicsObjects.m_MassData[rhs].m_Mass;

				glm::vec2 offset = 0.5f * overlap * collisionNormal * restitution;

				bool lhsNotKinematic =  1 - m_PhysicsObjects.m_Flags[lhs] & PhysicsObjects::kFlagKinematic;
				bool rhsNotKinematic = 1 - m_PhysicsObjects.m_Flags[rhs] & PhysicsObjects::kFlagKinematic;

				m_PhysicsObjects.m_VerletBodies[lhs].m_Position += (m_PhysicsObjects.m_MassData[rhs].m_Mass / combinedMass) * lhsNotKinematic * offset;
				m_PhysicsObjects.m_VerletBodies[rhs].m_Position += (m_PhysicsObjects.m_MassData[lhs].m_Mass / combinedMass) * rhsNotKinematic * -offset;
				
			}
		}
	}

}

void World::SpatialGrid()
{
	SpatialPartition::Grid grid = SpatialPartition::Grid({ 0,0 }, 60, 60, 10);

	for (size_t po : m_PhysicsObjects.m_Entities)
		grid.AddObject(po, m_PhysicsObjects.m_VerletBodies[po].m_Position);

	for (int y = 1; y < grid.Height() - 1; y++)
	{
		for (int x = 1; x < grid.Width() - 1; x++)
		{
			std::vector<size_t> neighbourhood = grid.GetNeighbourhoodContents({ x,y });

			for (int k = 0; k < neighbourhood.size(); k++)
			{
				EntityID lhs = m_PhysicsObjects.m_Entities[neighbourhood[k]];
				for (int l = k + 1; l < neighbourhood.size(); l++)
				{
					EntityID rhs = m_PhysicsObjects.m_Entities[neighbourhood[l]];

					if (lhs == rhs)
						break;

					VerletBody& verletBodyA = m_PhysicsObjects.m_VerletBodies[lhs];
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
						float combinedMass = m_PhysicsObjects.m_MassData[lhs].m_Mass + m_PhysicsObjects.m_MassData[rhs].m_Mass;

						glm::vec2 offset = 0.5f * overlap * collisionNormal * restitution;

						m_PhysicsObjects.m_VerletBodies[lhs].m_Position += (m_PhysicsObjects.m_MassData[rhs].m_Mass / combinedMass) * offset;
						m_PhysicsObjects.m_VerletBodies[rhs].m_Position += (m_PhysicsObjects.m_MassData[lhs].m_Mass / combinedMass) * -offset;
					}
				}
			}
		}
	}
}


EntityID World::CreateBall(const glm::vec2 pos, float radius, const glm::vec4 color)
{
	EntityID id = m_PhysicsObjects.CreatePhysicsObject();

	m_PhysicsObjects.m_Entities[id]						=  id;

	m_PhysicsObjects.m_BallColliders[id].m_Radius		= radius;

	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagBallCollider;

	m_PhysicsObjects.m_VerletBodies[id].m_Position		= pos;
	m_PhysicsObjects.m_VerletBodies[id].m_OldPosition	= pos;
	m_PhysicsObjects.m_VerletBodies[id].m_Acceleration	= { 0.0f, 0.0f };
	m_PhysicsObjects.m_VerletBodies[id].m_Velocity		= { 0.0f, 0.0f };

	m_PhysicsObjects.m_MassData[id].m_Mass = m_PhysicsObjects.m_BallColliders[id].m_Radius* m_PhysicsObjects.m_BallColliders[id].m_Radius; // should be PIr^2
	m_PhysicsObjects.m_MassData[id].m_InvMass = 1.0f/m_PhysicsObjects.m_MassData[id].m_Mass;

	m_PhysicsObjects.m_RenderData[id].m_Color = color;
	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagRenderable;

	m_ActiveObjects.push_back(id);
	m_LiveObjects.push_back(id);

	return id;
}

EntityID World::CreateKinematicBall(const glm::vec2 pos, float radius, const glm::vec4 color)
{
	EntityID id = m_PhysicsObjects.CreatePhysicsObject();

	m_PhysicsObjects.m_Entities[id] = id;

	m_PhysicsObjects.m_BallColliders[id].m_Radius = radius;

	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagBallCollider;

	m_PhysicsObjects.m_VerletBodies[id].m_Position = pos;
	m_PhysicsObjects.m_VerletBodies[id].m_OldPosition = pos;
	m_PhysicsObjects.m_VerletBodies[id].m_Acceleration = { 0.0f, 0.0f };
	m_PhysicsObjects.m_VerletBodies[id].m_Velocity = { 0.0f, 0.0f };

	m_PhysicsObjects.m_MassData[id].m_Mass = m_PhysicsObjects.m_BallColliders[id].m_Radius * m_PhysicsObjects.m_BallColliders[id].m_Radius; // should be PIr^2
	m_PhysicsObjects.m_MassData[id].m_InvMass = 1.0f / m_PhysicsObjects.m_MassData[id].m_Mass;

	m_PhysicsObjects.m_RenderData[id].m_Color = color;
	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagRenderable;

	m_PhysicsObjects.m_Flags[id] |= PhysicsObjects::kFlagKinematic;
	m_LiveObjects.push_back(id);

	return id;
}

float t = 0;
void World::BallCannon(float deltaTime)
{
	BallCannonCounter += deltaTime;
	if (BallCannonCounter > BallCannonDelay)
	{
		EntityID id = CreateBall({ 0,260 }, 5, { (rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f, 1.0f });

		t += deltaTime;
		const float angle = 1.0f * sin(t) + 3.14159265358979323846 * 0.5f;
		m_PhysicsObjects.m_VerletBodies[id].m_OldPosition = glm::vec2{ cos(angle), sin(angle) } + m_PhysicsObjects.m_VerletBodies[id].m_Position;


		BallCannonCounter = 0;
		BallCannonShots--;
	}
}
