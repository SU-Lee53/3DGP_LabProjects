#include "stdafx.h"
#include "Timer.h"


CGameTimer::CGameTimer()
{
	// If this computer has performance counter, Set unit time using performance counter and performance frequency.
	// If not, Use multimedia timer. This will make unit time to 0.001 seconds.(1 ms)

	if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPerformanceFrequency)) 
	{
		m_bHardwareHasPerformanceCounter = TRUE;
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
		m_fTimeScale = 1.0f / m_nPerformanceFrequency;	
	}
	else 
	{
		__debugbreak();
		m_bHardwareHasPerformanceCounter = FALSE;
		// m_nLastTime = ::timeGetTime();
		m_fTimeScale = 0.001f;
	}
}

CGameTimer::~CGameTimer()
{
}

void CGameTimer::Tick(float fLockFPS)
{
	if (m_bHardwareHasPerformanceCounter)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	}
	else
	{
		// m_nCurrentTime = ::timeGetTime()
	}

	// Compute time spent after this function last called.
	float fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	if (fLockFPS > 0.0f)
	{
		// if fLockFPS > 0, wait for fLockFPS time.
		while (fTimeElapsed < (1.0f / fLockFPS))
		{
			if (m_bHardwareHasPerformanceCounter)
			{
				::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
			}
			else
			{
				// m_nCurrentTime = ::timeGetTime()
			}

			float fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
		}
	}

	// Store current time.
	m_nLastTime = m_nCurrentTime;

	// if difference between last frame time and current frame time are less then 1, save current frametime in m_fFrameTime[0]
	if (fabsf(fTimeElapsed - m_fTimeElapsed < 1.f))
	{
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapsed;
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}

	// Increase FPS and save frame time accumulated.
	m_nFramesPerSecond++;
	m_fFPSTimeElapsed += fTimeElapsed;
	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	// Get average of accumulated frame time.
	m_fTimeElapsed = 0.0f;
	for (ULONG i = 0; i < m_nSampleCount; ++i)
		m_fTimeElapsed += m_fFrameTime[i];

	if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;
}

ULONG CGameTimer::GetFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		// Convert current framerate to string and write lpszString buffer with " FPS".
		::_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);
		::wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return m_nCurrentFrameRate;
}

float CGameTimer::GetTimeElapsed()
{
	return m_fTimeElapsed;
}

void CGameTimer::Reset()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	m_nLastTime = nPerformanceCounter;
	m_nCurrentTime = nPerformanceCounter;

	m_bStopped = false;
}