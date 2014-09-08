#ifndef CBATCHTRACKER_H
#define CBATCHTRACKER_H

#include <memory>
#include <vector>
#include <Core/ITracker.h>

namespace Core {
    /**
     * A batch of trackers.
     */
    class CBatchTracker : public ITracker
    {
    public:

        explicit CBatchTracker(
                std::vector<std::unique_ptr<ITracker>>&& trackers)
            : m_trackers(std::move(trackers)) {}

        virtual State GetState() const;

    private:

        std::vector<std::unique_ptr<ITracker>> m_trackers;
    };
}

#endif // CBATCHTRACKER_H
