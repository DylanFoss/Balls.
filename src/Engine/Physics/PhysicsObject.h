#include "Engine/pch.h"
#include "glm/glm.hpp"

//typedef size_t EntityID;
typedef int Flags;

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

struct RenderData
{
	glm::vec4 m_Color;
};

const uint32_t ENTITY_INDEX_BITS = 24;
const uint32_t ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const uint32_t ENTITY_GENERATION_BITS = 8;
const uint32_t ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

struct EntityID
{
	uint32_t id;

	uint32_t index() const { return id & ENTITY_INDEX_MASK; }
	uint32_t generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
};

struct PhysicsObjects
{
	enum
	{
		kFlagRenderable = 1 << 0,
		kFlagBallCollider = 1 << 1,
		kFlagAABBCollider = 1 << 2,
		kFlagKinematic = 1 << 3
	};

	//entity
	std::vector<unsigned char> m_EntityGen;
	std::deque<size_t> m_FreeIndices;

	//component
	std::vector<VerletBody> m_VerletBodies;
	std::vector<BodyMassData> m_MassData;
	std::vector<BallCollider> m_BallColliders;
	std::vector<RenderData> m_RenderData;
	std::vector<Flags> m_Flags;

	EntityID CreatePhysicsObject()
	{
		uint32_t index = Allocate();

		if (index < m_EntityGen.size() - 1)
		{
			m_BallColliders[index] = BallCollider();
			m_MassData[index] = BodyMassData();
			m_VerletBodies[index] = VerletBody();
			m_RenderData[index] = RenderData();
			m_Flags[index] = 0;
		}
		else
		{
			m_BallColliders.emplace_back(BallCollider());
			m_MassData.emplace_back(BodyMassData());
			m_VerletBodies.emplace_back(VerletBody());
			m_RenderData.emplace_back(RenderData());
			m_Flags.emplace_back(0);
		}

		return EntityID{ index | (m_EntityGen[index] << ENTITY_INDEX_BITS) };
	}

	void RemovePhysicsObject(EntityID id)
	{
		Deallocate(id);
	}

	uint32_t Allocate()
	{
		uint32_t  index;
		if (m_FreeIndices.size() > 0)
		{
			index = m_FreeIndices.front();
			m_FreeIndices.pop_front();
		}
		else
		{
			m_EntityGen.push_back(0);
			index = m_EntityGen.size() - 1;
		}

		return index;
	}

	void Deallocate(EntityID id)
	{
		if (IsAlive(id))
		{
			const uint32_t index = id.index();
			++m_EntityGen[index];
			m_FreeIndices.push_back(index);
		}
	}

	bool IsAlive(EntityID id) const
	{
		return m_EntityGen[id.index()] == id.generation();
	}

	void Reserve(size_t n)
	{
		//m_PhysicsObjects.m_Positions.reserve(n);
		m_EntityGen.reserve(n);
		m_BallColliders.reserve(n);
		m_VerletBodies.reserve(n);
		m_MassData.reserve(n);
		m_RenderData.reserve(n);
		m_Flags.reserve(n);
	}
};