#pragma once
#include "Engine/GL.h"
#include "Engine/GLRenderer.h"
#include "Ball.h"

#include "Engine/Physics/World.h"

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

    OrthographicCameraController camera = OrthographicCameraController(400.0f);
    World m_World;
};


