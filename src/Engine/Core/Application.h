#pragma once
#include <string>
#include <memory>
#include "Window.h"
#include "Input.h"

#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"

class Application
{
public:
	Application(const std::string& name = "OpenGL Sandbox", uint32_t width = 1280, uint32_t height = 720);
	virtual ~Application() = default;

	int Run();

	void OnEvent(Event& e);

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(float deltaTime) = 0;

	inline static Application& Get() { return *s_Instance; }
	inline Window& GetWindow() { return *m_Window; }

protected:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Input> m_Input;

private:

	bool m_Running = true;
	float m_LastFrameTime = 0.0f;

	static Application* s_Instance;

	bool OnWindowClose(WindowCloseEvent& e);

};

