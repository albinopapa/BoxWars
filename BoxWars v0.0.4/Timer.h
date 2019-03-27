
#pragma once

#include <chrono>

class Timer
{
public:
	void StartWatch()noexcept;
	void StopWatch()noexcept;
	float GetTimeMilli() const noexcept;

private:
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point current;
	bool watchStopped = false;
};

class FrameTimer
{
public:
	float Mark()noexcept;
private:
	Timer timer;
};