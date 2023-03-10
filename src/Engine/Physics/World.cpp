#include "World.h"

#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/Body.h"
#include "Engine/Physics/Collision/Collisionpch.h"

World::World(const glm::vec2& gravity)
	:m_Gravity(gravity), m_StepDuration(0.00694f), m_StepTime(0), m_SubSteps(8)
{
}

World::~World()
{
	for (PhysicsObject* obj : m_Physics)
		delete obj;
	m_Physics.clear();
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
			else
			{
				m_Physics.clear();
				BallCannonShots = 200;
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
	for (PhysicsObject* obj : m_Physics)
		obj->Update(deltaTime);
}

void World::ApplyGravity()
{
	for (PhysicsObject* obj : m_Physics)
		obj->Accelerate(m_Gravity);
}

void World::ApplyConstraints()
{
	for (PhysicsObject* obj : m_Physics)
	{
		BallCollider* circle = static_cast<BallCollider*>(obj->GetCollider());

		const glm::vec2 toObj = obj->GetPosition() - glm::vec2{ 0.f, 0.f };
		const float dist = glm::length(toObj);
		if (dist > 300.f - circle->GetRadius())
		{
			obj->SetPosition((toObj/dist) * (300.f - circle->GetRadius()));
		}
	}
}

//to be seperated into their own classes, but fun to mess around with now!
void World::SolveCollisions()
{
	BroadPhase();
	NarrowPhase();
	m_Collisions.clear();
}

void World::BroadPhase()
{
	for (int i = 0; i < m_Physics.size(); i++)
	{
		PhysicsObject* lhs = m_Physics[i];
		for (int j = i + 1; j < m_Physics.size(); j++)
		{
			PhysicsObject* rhs = m_Physics[j];

			Manifold m = lhs->GetCollider()->TestCollision(lhs->GetTransform(), rhs->GetCollider(), rhs->GetTransform());

			if (m.collision)
			{
				m.a = lhs;
				m.b = rhs;
				m_Collisions.emplace_back(m);
			}
			
		}
	}
}

void World::NarrowPhase()
{
	for (Manifold& pair : m_Collisions)
	{
		//divide each body's individual mass by the combined mass
		//to work out which should be offset more.
		float combinedMass = pair.a->GetMass() + pair.b->GetMass();

		//glm::vec2 offset = 0.5f * overlap * collisionNormal;
		glm::vec2 offset = pair.overlap * pair.collisionNormal;

		pair.a->OffsetPosition((pair.b->GetMass() / combinedMass) * offset);
		pair.b->OffsetPosition((pair.a->GetMass() / combinedMass) * -offset);

		//let's not delete the ptrs
		pair.a = nullptr;
		pair.b = nullptr;
	}
}

void World::CreatePhysicsObject(const glm::vec2 pos, Collider* shape)
{
	Collider* concreteShape;
	switch (shape->GetType())
	{
	case(ColliderType::BALL):
		concreteShape = static_cast<BallCollider*>(shape);
		break;
	case(ColliderType::AABB):
		break;
	default:
		std::cout << "No valid type provided." << '\n';
		break;
	}

	BodyDefinition bd;
	bd.m_Position = pos;

	m_Physics.emplace_back(new PhysicsObject(shape, bd));
}

void World::CreateBall(const glm::vec2 pos, float radius)
{
	BodyDefinition bd;
	bd.m_Position = pos;

	BallCollider* c = new BallCollider(radius);
	m_Physics.emplace_back(new PhysicsObject(c, bd));
}

void World::BallCannon(float deltaTime)
{
	BallCannonCounter += deltaTime;
	if (BallCannonCounter > BallCannonDelay)
	{
		CreateBall({ 0,260 }, 10);
		m_Physics.back()->SetVelocity({ -(rand() % 6 - 3), (rand() % 2 + 1) } );

		BallCannonCounter = 0;
		BallCannonShots--;
	}
}
