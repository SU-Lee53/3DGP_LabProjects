#pragma once

// Accumulate and average 50 frame times
const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	// Update time
	void Tick(float fLockFPS = 0.0f);

	// return framerate in string
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);

	// return time elapsed
	float GetTimeElapsed();

private:
	double m_fTimeScale = 0.0;
	float m_fTimeElapsed = 0.0f;

	__int64 m_nBasePerformanceCounter = 0;
	__int64 m_nPausedPerformanceCounter = 0;
	__int64 m_nStopPerformanceCounter = 0;
	__int64 m_CurrentPerformanceCounter = 0;
	__int64 m_nLastPerformanceCounter = 0;

	__int64 m_PerformanceFrequencyPerSec = 0;

	float m_fFrameTime[MAX_SAMPLE_COUNT] = {};
	ULONG m_nSampleCount = 0;

	ULONG m_nCurrentFrameRate = 0;
	ULONG m_FramePerSecond = 0;
	float m_fFPSTimeElapsed = 0.0f;

};

