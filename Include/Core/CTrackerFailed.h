#ifndef CORE_CTRACKERFAILED_H
#define CORE_CTRACKERFAILED_H

#include <Core/ITracker.h>

namespace Core {
    /**
     * A tracker that always reports failed state.
     * @author Dmytro Yurchenko.
     */
    struct CTrackerFailed : public ITracker
    {
        /// @returns Always STATE_FAILED.
        virtual State GetState() const;
    };
}

#endif // CORE_CTRACKERFAILED_H
