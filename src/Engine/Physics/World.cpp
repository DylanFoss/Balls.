#include "World.h"

#include "Engine/Physics/PhysicsObject.h"

#include "Engine/Physics/Collision/BallCollider.h"
#include "Engine/Physics/Body.h"
#include "Engine/Physics/Collision/ColliderType.h"

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
		BallCollider* circle = static_cast<BallCollider*>(obj->GetShape());

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

			Manifold m;
			m.objectOne = lhs;
			m.objectTwo = rhs;

			if (Collisions::CircleVsCircle(m))
				m_Collisions.push_back(m);
		}
	}
}

void World::NarrowPhase()
{
	for(Manifold pair : m_Collisions)
	{
		//divide each body's individual mass by the combined mass
		//to work out which should be offset more.
		float combinedMass = pair.objectOne->GetMass() + pair.objectTwo->GetMass();

		//glm::vec2 offset = 0.5f * overlap * collisionNormal;
		glm::vec2 offset = pair.overlap * pair.collisionNormal;

		pair.objectOne->OffsetPosition((pair.objectTwo->GetMass() / combinedMass) * offset);
		pair.objectTwo->OffsetPosition((pair.objectOne->GetMass() / combinedMass) * -offset);

		//let's not delete the ptrs
		pair.objectOne = nullptr;
		pair.objectTwo = nullptr;
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
