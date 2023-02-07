#include "GLRenderer.h"
#include "pch.h"
#include "GL.h"

#include "GLErrorHandler.h"

struct RenderData
{
	static const uint32_t MaxQuads = 1000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;

	GLuint QuadVA = 0;
	GLuint QuadVB = 0;
	GLuint QuadIB = 0;
	std::shared_ptr<Shader> QuadShader;

	uint32_t QuadIndexCount = 0;
	Vertex* QuadVertexBufferArray = nullptr;
	Vertex* QuadVertexBufferPtr = nullptr;

	glm::vec2 QuadVertexPositions[4];

	Renderer2D::PerformanceData stats;
	glm::mat4 ViewProjection;
};

static RenderData s_Data;

void Renderer2D::Init()
{
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << '\n';

	{
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
		std::cout << "Max texture size : " << size << '\n';
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.20f, 1.0f);

	//ASSERT(s_Data.QuadVertexBufferArray == nullptr) // VertexBuffer already assigned, Init was already called.
	s_Data.QuadVertexBufferArray = new Vertex[s_Data.MaxVertices];

	GLCall(glGenVertexArrays(1, &s_Data.QuadVA));
	GLCall(glBindVertexArray(s_Data.QuadVA));

	GLCall(glCreateBuffers(1, &s_Data.QuadVB));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	GLCall(glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoord)));

	std::vector<unsigned int> indices;
	indices.reserve(s_Data.MaxIndices);
	int offset = 0;
	for (int i = 0; i < s_Data.MaxIndices; i += 6)
	{
		indices.emplace_back(0 + offset);
		indices.emplace_back(1 + offset);
		indices.emplace_back(2 + offset);
		indices.emplace_back(2 + offset);
		indices.emplace_back(3 + offset);
		indices.emplace_back(0 + offset);
		offset += 4;
	}

	GLCall(glCreateBuffers(1, &s_Data.QuadIB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f};
	s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f};
	s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f};
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f};

	s_Data.QuadShader = std::make_shared<Shader>();
	s_Data.QuadShader->CreateShader("basic.vert.shader", "basic.frag.shader");
}

void Renderer2D::Shutdown()
{
	delete[] s_Data.QuadVertexBufferArray;
}

void Renderer2D::StartFrame(OrthographicCamera& camera)
{
	s_Data.ViewProjection = camera.GetViewProjectionMatrix();
	BeginBatch();
}

void Renderer2D::EndFrame()
{
	EndBatch();
	Flush();
}

void Renderer2D::BeginBatch()
{
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferArray;
}

void Renderer2D::EndBatch()
{
	// copy CPU side memory to GPU
	GLsizeiptr size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferArray;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadVertexBufferArray));
}

void Renderer2D::Flush()
{
	//Draw
	s_Data.QuadShader->Bind();
	glm::mat4 mvp = s_Data.ViewProjection;
	GLCall(glUniformMatrix4fv(glGetUniformLocation(s_Data.QuadShader->ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));

	GLCall(glBindVertexArray(s_Data.QuadVA));
	GLCall(glBindVertexArray(s_Data.QuadVB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
	GLCall(glDrawElements(GL_TRIANGLES, s_Data.MaxIndices, GL_UNSIGNED_INT, nullptr));

	s_Data.QuadIndexCount = 0;
}

void Renderer2D::DrawQuad(const glm::vec2 position, glm::vec2 size)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices)//|| do texture slot checks here)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	constexpr glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	constexpr int numVerts = 4;

	for (int i = 0; i < numVerts; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = position + (size*s_Data.QuadVertexPositions[i]);
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

void Renderer2D::DrawQuad(const glm::mat4 position, glm::vec2 size)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices)//|| do texture slot checks here)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	constexpr glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	constexpr int numVerts = 4;

	for (int i = 0; i < numVerts; i++)
	{
		/*s_Data.QuadVertexBufferPtr->Position = position + (size * s_Data.QuadVertexPositions[i]);*/
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

void Renderer2D::ResetStats()
{
}

Renderer2D::PerformanceData Renderer2D::GetStats()
{
	return s_Data.stats;
}
