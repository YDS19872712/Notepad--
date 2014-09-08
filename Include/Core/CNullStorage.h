#ifndef CORE_CNULLSTORAGE_H
#define CORE_CNULLSTORAGE_H

#include <Core/IDataStorage.h>

namespace Core {
    /**
     * A storage implementation that contains nothing.
     */
    struct CNullStorage : public IDataStorage
    {
        virtual unsigned long long GetSize() const;

        virtual std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            void* buffer,
            std::size_t size) const;

        virtual std::unique_ptr<ITracker> Write(
            unsigned long long offset,
            const void* buffer,
            std::size_t size);
    };
}

#endif // CORE_CNULLSTORAGE_H
