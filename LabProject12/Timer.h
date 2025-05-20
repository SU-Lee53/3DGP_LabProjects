#pragma once

const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

public:
	void Start() {}
	void Stop() {}
	void Reset();
	
	// Updates Timer
	void Tick(float fLockFPS = 0.0f);
	
	// Returns Framerate
	ULONG GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);

	// Returns average frame time(aka delta time)
	float GetTimeElapsed();

private:
	BOOL	m_bHardwareHasPerformanceCounter = FALSE;	// Does this computer has Performace Counter?
	float	m_fTimeScale = 0.f;							// Amount of Scale Counter
	float	m_fTimeElapsed = 0.f;						// Time spent after last frame
	__int64 m_nCurrentTime = 0;							// Current time
	__int64 m_nLastTime = 0;							// Last frame time
	__int64 m_nPerformanceFrequency = 0;				// This computer's Performance Frequency

	float m_fFrameTime[MAX_SAMPLE_COUNT];				// Array for accumulate frame time
	ULONG m_nSampleCount = 0;							// Accumulated frames count

	ULONG m_nCurrentFrameRate = 0;						// Current Framerate
	ULONG m_nFramesPerSecond = 0;						// F(rames) P(er) S(econd)
	float m_fFPSTimeElapsed = 0.f;						// Time spent while computing framerate

	BOOL m_bStopped = FALSE;



};

