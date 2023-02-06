#pragma once
#include "GL.h"
#include "GLUtils.h"

class Balls:
    public Application
{
public:
    Balls(const std::string& name, uint32_t width, uint32_t height);
    ~Balls();

    void Init();
    void Shutdown();

    void Update(float deltatime);
    void Draw(float deltatime);

private:

    int32_t m_WindowHalfWidth;
    int32_t m_WindowHalfHeight;

    Utils::OrthographicCameraController camera = Utils::OrthographicCameraController(400.0f);

    bool m_IsPaused = false;

    GLuint VA = 0;
    GLuint VB = 0;
    GLuint IB = 0;

   // glm::vec2 GetGameCell(glm::vec2 worldPosition);
};


