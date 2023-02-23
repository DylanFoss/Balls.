#include "Engine/GLRenderer.h"
#include "Engine/pch.h"
#include "Engine/GL.h"

#include "Engine/GLErrorHandler.h"

#include "glm/gtc/matrix_transform.hpp"

struct Vertex
{
	glm::vec2 Position;
	glm::vec2 TexCoord;
};

struct LineVertex
{
	glm::vec2 Position;
	glm::vec4 Color;
};

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

	glm::vec4 QuadVertexPositions[4];

	GLuint LineVA = 0;
	GLuint LineVB = 0;
	std::shared_ptr<Shader> LineShader;

	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferArray = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;

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

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f};
	s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f};
	s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f};
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f};

	s_Data.QuadShader = std::make_shared<Shader>();
	s_Data.QuadShader->CreateShader("basic.vert.shader", "basic.frag.shader");

	//set up line

	s_Data.LineVertexBufferArray = new LineVertex[s_Data.MaxVertices];

	GLCall(glGenVertexArrays(1, &s_Data.LineVA));
	GLCall(glBindVertexArray(s_Data.LineVA));

	GLCall(glCreateBuffers(1, &s_Data.LineVB));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.LineVB));
	GLCall(glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(s_Data.LineVA, 0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position)));

	GLCall(glEnableVertexArrayAttrib(s_Data.LineVA, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color)));

	s_Data.LineShader = std::make_shared<Shader>();
	s_Data.LineShader->CreateShader("line.vert.shader", "line.frag.shader");
}

void Renderer2D::Shutdown()
{
	delete[] s_Data.QuadVertexBufferArray;
	delete[] s_Data.LineVertexBufferArray;
}

void Renderer2D::StartFrame(const OrthographicCamera& camera)
{
	s_Data.ViewProjection = camera.GetViewProjectionMatrix();
	BeginBatch();
}

void Renderer2D::EndFrame()
{
	Flush();
}

void Renderer2D::BeginBatch()
{
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferArray;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferArray;
}

void Renderer2D::EndBatch()
{
	// copy CPU side memory to GPU
	GLsizeiptr size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferArray;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadVertexBufferArray));

	// copy CPU side memory to GPU
	size = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferArray;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.LineVB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.LineVertexBufferArray));
}

void Renderer2D::Flush()
{
	if (s_Data.QuadIndexCount)
	{
		GLsizeiptr size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferArray;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadVertexBufferArray));

		s_Data.QuadShader->Bind();
		glm::mat4 mvp = s_Data.ViewProjection;
		GLCall(glUniformMatrix4fv(glGetUniformLocation(s_Data.QuadShader->ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));

		GLCall(glBindVertexArray(s_Data.QuadVA));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
		GLCall(glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr));

		s_Data.QuadIndexCount = 0;
	}

	if (s_Data.LineVertexCount)
	{
		GLsizeiptr size = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferArray;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.LineVB));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.LineVertexBufferArray));

		s_Data.LineShader->Bind();
		glm::mat4 mvp = s_Data.ViewProjection;
		GLCall(glUniformMatrix4fv(glGetUniformLocation(s_Data.LineShader->ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));
		GLCall(glBindVertexArray(s_Data.LineVA));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.LineVB));
		GLCall(glDrawArrays(GL_LINES, 0, s_Data.LineVertexCount));

		s_Data.LineVertexCount = 0;
	}
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform);
}

void Renderer2D::DrawQuad(const glm::mat4& transform)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
	{
		Flush();
		BeginBatch();
	}

	constexpr glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	constexpr int numVerts = 4;

	for (int i = 0; i < numVerts; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

void Renderer2D::DrawLine(const glm::vec2& point0, const glm::vec2& point1, const glm::vec4& color)
{
	if (s_Data.LineVertexCount >= s_Data.MaxVertices)
	{
		Flush();
		BeginBatch();
	}

	s_Data.LineVertexBufferPtr->Position = point0;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->Position = point1;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexCount += 2;
}

void Renderer2D::ResetStats()
{
}

Renderer2D::PerformanceData Renderer2D::GetStats()
{
	return s_Data.stats;
}
