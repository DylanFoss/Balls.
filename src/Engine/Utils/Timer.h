#include <chrono>
#include <iostream>

namespace Utils
{

struct StackTimer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	StackTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~StackTimer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout << "Timer Took: " << ms << "ms " << std::endl;
	}

};

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	void Start() 
	{
		start = std::chrono::high_resolution_clock::now();
	};

	void End()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout << "Timer Took: " << ms << "ms " << std::endl;
	}

	Timer()
	{
	}

	~Timer()
	{
	}

};

}