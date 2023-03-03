#include "glm/glm.hpp"

#include "Engine/pch.h"

namespace SpatialPartition
{
	struct GridCell
	{
		std::vector<size_t> m_CellBucket;
	};

	class Grid
	{
	public:
		Grid(glm::vec2 position, int width, int height, float cellSize);

		void AddObject(size_t physobj, const glm::vec2& pos);

		GridCell* GetCell(const glm::vec2& pos);
		GridCell* GetRelativeCell(const glm::vec2& pos);
		std::vector<GridCell*> GetNeighbourhoodCells(const glm::vec2& pos);
		std::vector<size_t> GetNeighbourhoodContents(const glm::vec2& pos);

		inline int Width() const { return m_GridWidth; };
		inline int Height() const { return m_GridHeight; };

	private:

		glm::vec2 WorldToGrid(const glm::vec2& pos) const;

		glm::vec2 m_Position;
		float m_CellSize;
		int m_GridWidth;
		int m_GridHeight;

		//cache half values
		int m_GridHalfWidth;
		int m_GridHalfHeight;

		std::vector<GridCell> m_Grid;
	};

};