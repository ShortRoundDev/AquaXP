#include "pch.h"
#include "StepTimer.h"

using namespace AquaXP;

Result<StepTimer> AquaXP::CreateStepTimer()
{
    LARGE_INTEGER qpcFrequency, qpcLastTime;
    if (!QueryPerformanceFrequency(&qpcFrequency))
    {
        return ErrorCode::QueryPerformanceCounterFailed;
    }

    if (!QueryPerformanceCounter(&qpcLastTime))
    {
        return ErrorCode::QueryPerformanceCounterFailed;
    }

    return StepTimer(qpcFrequency, qpcLastTime);
}