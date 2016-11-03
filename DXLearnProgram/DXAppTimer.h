#pragma once 
#include <windows.h>

class Timer
{
public:
	Timer();
	virtual ~Timer();

	float	DeltaTime()const;
	float	TotalTime()const;

	void	RestTimer();
	void	Start();
	void	Stop();
	void	Tick();
private:
	__int64 preTime;
	__int64 baseTime;
	__int64 pauseTime;
	__int64 currentTime;
	__int64 stopTime;

	double secondPerCount;
	double iDeltaTime;

	bool bStopped;
};