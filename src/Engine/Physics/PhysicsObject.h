#include "Engine/pch.h"
#include "glm/glm.hpp"

typedef size_t EntityID;
typedef int Flags;

template <typename T>
using Component = std::vector<T>;

struct VerletBody
{
	glm::vec2 m_Position;
	glm::vec2 m_OldPosition;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Velocity;
};

struct BodyMassData
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
		kFlagAABBCollider = 1 << 2
	};

	std::vector<EntityID> m_Entities;
	std::vector<VerletBody> m_VerletBodies;
	std::vector<BodyMassData> m_MassData;
	std::vector<BallCollider> m_BallColliders;
	std::vector<Flags> m_Flags;

	EntityID CreatePhysicsObject()
	{
		m_Entities.emplace_back(m_Entities.size());
		m_BallColliders.emplace_back(BallCollider());
		m_VerletBodies.emplace_back(VerletBody());
		m_Flags.emplace_back(0);

		return static_cast<EntityID>(m_Entities.size() - 1);
	}

	void Reserve(size_t n)
	{
		//m_PhysicsObjects.m_Positions.reserve(n);
		m_Entities.reserve(n);
		m_BallColliders.reserve(n);
		m_VerletBodies.reserve(n);
		m_VerletBodies.reserve(n);
	}
};