#pragma once
#include <string>
#include "GLFW/glfw3.h"

#include <functional>
#include "Events/Event.h"


struct WindowProps
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;
	bool vsync = false;

	WindowProps(const std::string& title = "Test Application", uint32_t width = 1280, uint32_t height = 720)
		:Title(title), Width(width), Height(height)
	{
	}
};

struct WindowData
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;
	bool vsync;

	std::function<void(Event&)> EventCallback;
};

class Window
{
public:
	Window(const WindowProps& data);
	~Window();

	void OnUpdate();

	inline uint32_t GetWidth() const { return m_Data.Width; }
	inline uint32_t GetHeight() const { return m_Data.Height; }

	inline void SetEventCallback(const std::function<void(Event&)>& callback) { m_Data.EventCallback = callback; }
	void SetVsync(bool enable);
	bool IsVsync() const;

	static Window* Create(const WindowProps& data = WindowProps());

	inline void* GetNativeWindow() { return m_Window; }

private:
	void Init(const WindowProps& data);
	void Shutdown();

	WindowData m_Data;
	GLFWwindow* m_Window;
	bool glfwInitialised = false;
};