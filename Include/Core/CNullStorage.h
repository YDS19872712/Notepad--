#ifndef CORE_CNULLSTORAGE_H
#define CORE_CNULLSTORAGE_H

#include <Core/IDataStorage.h>

namespace Core {
    /**
     * A storage implementation that contains nothing.
     */
    struct CNullStorage : public IDataStorage
    {
        virtual long long GetSize() const;

        virtual std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            std::size_t size,
            void* buffer) const;

        virtual std::unique_ptr<ITracker> Write(
            unsigned long long offset,
            std::size_t size,
            const void* buffer);
    };
}

#endif // CORE_CNULLSTORAGE_H
