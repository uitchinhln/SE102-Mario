#pragma once
#include <chrono>

class Stopwatch
{
public:
	Stopwatch();

	virtual void Start();
	virtual void Stop();
	virtual void Reset();
	virtual void Restart();

	virtual bool IsRunning();

	virtual long Elapsed();

protected:
	long elapsed;
	std::chrono::steady_clock::time_point startTimeStamp;
	bool isRunning;

	long GetElapsedTime();
};