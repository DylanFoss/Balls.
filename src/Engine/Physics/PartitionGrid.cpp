#include "Engine/Physics/PartitionGrid.h"

SpatialPartition::Grid::Grid(glm::vec2 position, int width, int height, float cellSize)
	:m_Position(position), m_CellSize(cellSize), m_GridWidth(width), m_GridHeight(height), m_GridHalfWidth(width/2), m_GridHalfHeight(height/2)
{
	m_Grid.resize(m_GridWidth * m_GridHeight);
}

void SpatialPartition::Grid::AddObject(size_t physobj, const glm::vec2& pos)
{
	GetCell(pos)->m_CellBucket.push_back(physobj);
}

SpatialPartition::GridCell* SpatialPartition::Grid::GetCell(const glm::vec2& pos)
{
	glm::vec2 position = WorldToGrid(pos);

	return &m_Grid[static_cast<int>(position.x) + m_GridWidth * static_cast<int>(position.y)];
}

SpatialPartition::GridCell* SpatialPartition::Grid::GetRelativeCell(const glm::vec2& pos)
{
	glm::vec2 position = pos;

	if (position.x > m_GridWidth - 1) position.x = static_cast<float>(m_GridWidth - 1);
	else if (position.x < 0) position.x = 0.f;
	if (position.y > m_GridHeight - 1) position.y = static_cast<float>(m_GridHeight - 1);
	else if (position.y < 0) position.y = 0.f;

	return &m_Grid[static_cast<int>(position.x) + m_GridWidth * static_cast<int>(position.y)];
}

std::vector<SpatialPartition::GridCell*> SpatialPartition::Grid::GetNeighbourhoodCells(const glm::vec2& pos)
{
	std::vector<SpatialPartition::GridCell*> m_Neighborhood;
	m_Neighborhood.reserve(9);

	glm::vec2 middleCell = pos;

	if (middleCell.x == m_GridWidth - 1) middleCell.x--;
	else if (middleCell.x == 0) middleCell.x++;
	if (middleCell.x == m_GridHeight - 1) middleCell.y--;
	else if (middleCell.x == 0) middleCell.y++;

	for (float i = -1; i < 2; i++)
		for (float j = -1; j < 2; j++)
		{
			m_Neighborhood.emplace_back(GetRelativeCell(middleCell + glm::vec2{ j, i}));
		}

	return m_Neighborhood;
}

std::vector<size_t> SpatialPartition::Grid::GetNeighbourhoodContents(const glm::vec2& pos)
{
	std::vector<SpatialPartition::GridCell*> m_Neighborhood = GetNeighbourhoodCells(pos);

	std::vector<size_t> physicsObjects;
	for (SpatialPartition::GridCell* cell : m_Neighborhood)
	{
		for (auto obj : cell->m_CellBucket)
			physicsObjects.emplace_back(obj);
	}

	return physicsObjects;
}

glm::vec2 SpatialPartition::Grid::WorldToGrid(const glm::vec2& pos) const
{
	glm::vec2 WorldToGrid;
	WorldToGrid.x = m_GridHalfWidth + std::floor(pos.x / m_CellSize);
	WorldToGrid.y = m_GridHalfHeight - std::floor(pos.y / m_CellSize);

	if (WorldToGrid.x > m_GridWidth - 1) WorldToGrid.x = static_cast<float>(m_GridWidth - 1);
	else if (WorldToGrid.x < 0) WorldToGrid.x = 0.f;
	if (WorldToGrid.y > m_GridHeight - 1) WorldToGrid.y = static_cast<float>(m_GridHeight - 1);
	else if (WorldToGrid.y < 0) WorldToGrid.y = 0.f;

	return WorldToGrid;
}
