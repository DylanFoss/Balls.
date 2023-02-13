#pragma once
#include "GL.h"
#include "GLRenderer.h"
#include "Ball.h"

class Balls:
    public Application
{
    struct CollisionPairComparator {
        bool operator()(std::pair<Ball&, Ball&> lhs, std::pair<Ball&, Ball&> rhs) const {
            return (std::minmax(lhs.first.ID(), lhs.second.ID()) > std::minmax(rhs.first.ID(), rhs.second.ID()));
        }
    };

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

    bool m_IsPaused = false;

    std::vector<Ball> m_Balls;
    std::set<std::pair<Ball&, Ball&>, CollisionPairComparator> m_Pairs;

    Ball* m_SelectedBall = nullptr;

    int m_ID;
};


