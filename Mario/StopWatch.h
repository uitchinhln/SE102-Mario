#pragma once
#include <string>

class Stopwatch
{
public:
	Stopwatch();

	void Start();
	void Stop();
	void Reset();
	void Restart();

	bool IsRunning();

	long Elapsed();

private:
	long elapsed;
	long startTimeStamp;
	bool isRunning;

	long GetElapsedTime();
};