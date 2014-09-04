#ifndef CORE_ITRACKER_H
#define CORE_ITRACKER_H

namespace Core {
    /**
     * An interface for tracking the state of an operation.
     * @author Dmytro Yurchenko.
     */
    struct __declspec(novtable) ITracker
    {
        /**
         * Represents available states of an operation.
         */
        enum State
        {
            STATE_COMPLETE,
            STATE_FAILED,
            STATE_PENDING,
        };

        /// Ensures virtual destructor.
        virtual ~ITracker() {}

        /**
         * Gets the current state of an operation that is tracked.
         * Since ITracker is designed for one-time use only, it is expected
         * that GetState() will never return STATE_PENDING once it has
         * already returned STATE_COMPLETE or STATE_FAILED.
         * A value of STATE_PENDING may never be returned if an operation
         * completed immediately, disregarding successfully or not.
         */
        virtual State GetState() const = 0;
    };
}

#endif // CORE_ITRACKER_H
