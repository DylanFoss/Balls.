#include "Renderer/OrthographicCamera.h"

class Renderer2D
{
public:

	static void Init();
	static void Shutdown();

	static void StartFrame(OrthographicCamera& camera);
	static void EndFrame();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawQuad(const glm::vec2 position, glm::vec2 size);
	static void DrawQuad(const glm::mat4 transform);

	struct PerformanceData
	{
		uint32_t numDrawsThisFrame;
		uint32_t numQuadsThisFrame;

		uint32_t GetTotalVertexCount() const { return numQuadsThisFrame * 4; }
		uint32_t GetTotalIndexCount() const { return numQuadsThisFrame * 6; }
	};
	static void ResetStats();
	static PerformanceData GetStats();

};

