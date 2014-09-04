#include <Core/CTrackerFailed.h>

using Core::CTrackerFailed;

CTrackerFailed::State CTrackerFailed::GetState() const
{
    return STATE_FAILED;
}
