#include "Timer.h"

using steady_clock = std::chrono::steady_clock;

void Timer::StopWatch()noexcept
{
	if( !watchStopped )
	{
		current = steady_clock::now();
		watchStopped = true;
	}
}

void Timer::StartWatch()noexcept
{
	watchStopped = false;
	start = steady_clock::now();
}

float Timer::GetTimeMilli() const noexcept
{
	return  ( !watchStopped ) ?
		std::chrono::duration<float>( steady_clock::now() - start ).count():
		std::chrono::duration<float>( current - start ).count();
}

float FrameTimer::Mark()noexcept
{
	timer.StopWatch();
	const auto elapsed = timer.GetTimeMilli();
	timer.StartWatch();
	return elapsed;
}
