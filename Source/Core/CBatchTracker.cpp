#include <algorithm>
#include <Core/CBatchTracker.h>

using namespace std;
using namespace Core;

CBatchTracker::State CBatchTracker::GetState() const
{
    if (any_of(m_trackers.cbegin(), m_trackers.cend(),
            [](const std::unique_ptr<ITracker>& tracker)
            {
                return tracker->GetState() == ITracker::STATE_PENDING;
            })) {
        return ITracker::STATE_PENDING;
    }

    if (all_of(m_trackers.cbegin(), m_trackers.cend(),
            [](const std::unique_ptr<ITracker>& tracker)
            {
                return tracker->GetState() == ITracker::STATE_COMPLETE;
            })) {
        return ITracker::STATE_COMPLETE;
    }

    return ITracker::STATE_FAILED;
}
