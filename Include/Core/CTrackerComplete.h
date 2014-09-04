#ifndef CORE_CTRACKERCOMPLETE_H
#define CORE_CTRACKERCOMPLETE_H

#include <Core/ITracker.h>

namespace Core {
    /**
     * A tracker that always reports complete state.
     * @author Dmytro Yurchenko.
     */
    struct CTrackerComplete : public ITracker
    {
        /// @returns Always STATE_COMPLETE.
        virtual State GetState() const;
    };
}

#endif // CORE_CTRACKERCOMPLETE_H
