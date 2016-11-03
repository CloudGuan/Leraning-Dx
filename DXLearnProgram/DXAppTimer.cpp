#include "DXAppTimer.h"

Timer::Timer()
	:preTime(0), baseTime(0)
	, pauseTime(0), currentTime(0)
	, stopTime(0)
	, secondPerCount(0.0)
	, bStopped(false)
{
	__int64 countPerSceond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSceond);
	secondPerCount = 1.0 / (double)countPerSceond;
}

Timer::~Timer()
{

}

float Timer::DeltaTime() const
{
	return (float)iDeltaTime;
}

float Timer::TotalTime() const
{
	double res;
	if (bStopped)
	{
		res = ((stopTime - pauseTime) - baseTime)*secondPerCount;
	}
	else
	{
		res = ((currentTime - pauseTime) - baseTime)*secondPerCount;
	}
	return (float)res;
}

void Timer::RestTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	baseTime = currentTime;
	preTime = currentTime;
	bStopped = false;
	pauseTime = 0;
}

void Timer::Start()
{
	if (bStopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		pauseTime += (currentTime-stopTime);
		preTime = currentTime;
		stopTime = 0;
		bStopped = false;
	}
}

void Timer::Stop()
{
	if (!bStopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		stopTime = currentTime;
		bStopped = true;
	}
}

void Timer::Tick()
{
	if (bStopped)
	{
		iDeltaTime = 0.0;
		return;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	
	iDeltaTime = (currentTime - preTime)*secondPerCount;
	preTime = currentTime;


	// ȷ����Ϊ��ֵ��DXSDK�е�CDXUTTimer�ᵽ����������������˽ڵ�ģʽ
	// ���л�����һ����������mDeltaTime���Ϊ��ֵ��
	if (iDeltaTime<0)
	{
		iDeltaTime = 0.0;
	}
}
