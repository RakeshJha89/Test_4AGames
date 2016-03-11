#ifndef GLOBALS_H
#define GLOBALS_H
#include <iostream>
#include <sstream>

#define DEBUGOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

#define ONEOVERSIX 0.1666f
#define MIN_FLOAT_THRESHOLD 0.0001f

class Globals
{
public:

	void SetCurrentTime(double val) { currTime = val; }
	void SetDeltaTime(double val){ deltaTime = val; }
	void SetFixedDelta(double val) { fixedDeltaTime = val; }
	void UpdatePrevTime(){ prevTime = currTime; }

	double GetCurrTime() { return currTime; }
	double GetPrevTime() { return prevTime; }
	double GetDeltaTime(){ return deltaTime; }
	double GetFixedDelta(){ return fixedDeltaTime; }

	double inline GetSecondsPerTick()
	{
		if (secondsPerTick <= 0)
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			secondsPerTick = 1.0 / frequency.QuadPart;
		}
		return secondsPerTick;
	}

	double inline GetCurrentTimeInSeconds()
	{
		LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);

		return Cycles.QuadPart * secondsPerTick;
	}
	

private:

	double secondsPerTick;
	double currTime;
	double prevTime;
	double deltaTime;
	double fixedDeltaTime;
};

#endif