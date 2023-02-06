#pragma once
class TimeStep
{
public:
	TimeStep(float time = 0.0f)
		: m_Time(time)
	{
	}

	float Seconds() const { return m_Time; };
	float Milliseconds() const { return m_Time * 1000.0f; }

private:
	float m_Time;
};

