#include <d3d11.h>

static double g_countsPerSecond;
static double g_timeStart;
static double g_currentTime;
static double g_previousTime;

static double C_GetTimeSinceStart_Internal()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );
	return ( currentTime.QuadPart - g_timeStart ) / g_countsPerSecond;
}

void C_InitTime()
{
	LARGE_INTEGER freqCount;
	QueryPerformanceFrequency( &freqCount );

	g_countsPerSecond = ( double ) freqCount.QuadPart;

	g_timeStart = 0;
	g_timeStart = C_GetTimeSinceStart_Internal();

	g_previousTime = g_currentTime = g_timeStart;
}

void C_UpdateTime()
{
	g_previousTime = g_currentTime;
	g_currentTime = C_GetTimeSinceStart_Internal();
}

double C_GetFrameTime()
{
	return g_currentTime - g_previousTime;
}

double C_GetTimeSinceStart()
{
	return g_currentTime;
}