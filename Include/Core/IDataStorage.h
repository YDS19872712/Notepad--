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
        virtual unsigned long long GetSize() const = 0;

        /**
         * Reads the specified number of bytes from the storage.
         * @param offset Position in bytes from which to read.
         * @param buffer A pointer to a buffer that is capable of storing
         * data chunk of requested size.
         * @param size Number of bytes to read.
         * @returns Unique pointer to an object that indicates the
         * state of reading operation initiated by the call.
         */
        virtual std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            void* buffer,
            std::size_t size) const = 0;

        /**
         * Write the specified number of bytes to the storage.
         * @param offset Position at which the writing will occure.
         * @param buffer A pointer to a buffer that contains the data.
         * @param size Number of bytes to write.
         * @returns Unique pointer to an object that tracks the state
         * of the writing operation.
         */
        virtual std::unique_ptr<ITracker> Write(
            unsigned long long offset,
            const void* buffer,
            std::size_t size) = 0;
    };
}

#endif // CORE_IDATASTORAGE_H
