#ifndef CORE_IDATASTORAGE_H
#define CORE_IDATASTORAGE_H

#include <memory>
#include <cstddef>
#include <Core/ITracker.h>

namespace Core {
    /**
     * An interface suitable for asynchronous data access.
     * @author Dmytro Yurchenko.
     */
    struct __declspec(novtable) IDataStorage
    {
        /// Ensures virtual destructor.
        virtual ~IDataStorage() {}

        /**
         * Gets the size of contained data.
         * @returns Size in bytes.
         */
        virtual long long GetSize() const = 0;

        /**
         * Reads the specified number of bytes from the storage.
         * @param offset Position in bytes from which to read.
         * @param size Number of bytes to read.
         * @param buffer A pointer to a buffer that is capable of storing
         * data chunk of requested size.
         * @returns Unique pointer to an object that indicates the
         * state of reading operation initiated by the call.
         */
        virtual std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            std::size_t size,
            void* buffer) const = 0;

        /**
         * Write the specified number of bytes to the storage.
         * @param offset Position at which the writing will occure.
         * @param size Number of bytes to write.
         * @param buffer A pointer to a buffer that contains the data.
         * @returns Unique pointer to an object that tracks the state
         * of the writing operation.
         */
        virtual std::unique_ptr<ITracker> Write(
            unsigned long long offset,
            std::size_t size,
            const void* buffer) = 0;
    };
}

#endif // CORE_IDATASTORAGE_H
