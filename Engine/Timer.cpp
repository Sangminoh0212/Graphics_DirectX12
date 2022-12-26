#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	// CPU 주파수에 따른 1초당 진행되는 틱수
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// 현재 CPU 틱
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void Timer::Update()
{
	// DeltaTime : 한 프레임이 넘어가는데 몇초가 걸리는가?
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;
	// CurrentCount - PrevCount = 프레임 당 진동수
	// Frequency = 초당 진동수
	// 프레임당 진동수 / 초당 진동수 = 프레임당 진동수가 몇초간 진행되었나 알 수 있음

	// Fps : 1초에 몇 프레임이 실행되는가?
	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime > 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}