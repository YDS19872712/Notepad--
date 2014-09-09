#ifndef CORE_CCHANGEBUFFER_H
#define CORE_CCHANGEBUFFER_H

#include <limits>
#include <memory>
#include <vector>
#include <cassert>
#include <cstddef>
#include <Core/IDataStorage.h>

// Conflicts with windows.h
#undef max

namespace Core {
    /**
     * A class that remembers all changes made to the file.
     */
    class CChangeBuffer
    {
    public:

        /**
         * @param storage Pointer to CFile instance available for read access
         * or CNullStorage if the goal is to create a new file in the end.
         */
        explicit CChangeBuffer(std::unique_ptr<IDataStorage> storage);

        /**
         * Gets total size of the buffer.
         * @returns Size in bytes.
         */
        unsigned long long GetSize() const;

        std::unique_ptr<ITracker> Read(
            unsigned long long offset,
            void* buffer,
            std::size_t size) const;

        /**
         * Inserts a block of data into the buffer.
         * @param offset An offset at which the insertion takes place.
         * @param data Source data to be inserted into the buffer.
         * @returns true if insertion succeeded.
         */
        bool Insert(
            unsigned long long offset,
            const void* data,
            std::size_t size);

        /**
         * Deletes the data from the change buffer.
         * @param offset An offset from which to start the deletion.
         * @param size Number of bytes to delete.
         * @returns true if deletion succeeded.
         */
        bool Delete(
            unsigned long long offset,
            std::size_t size);

    private:

        struct SBlock
        {
            unsigned long long m_offsetVirtual;
            unsigned long long m_offsetReal;
            std::vector<char>  m_data;

            SBlock(
                    unsigned long long offsetVirtual,
                    unsigned long long offsetReal)
                : m_offsetVirtual(offsetVirtual)
                , m_offsetReal(offsetReal) {}

            SBlock(
                    unsigned long long offsetVirtual,
                    const void* data,
                    std::size_t size)
                : m_offsetReal(std::numeric_limits<unsigned long long>::max())
                , m_offsetVirtual(offsetVirtual)
                , m_data(
                    reinterpret_cast<const char*>(data),
                    reinterpret_cast<const char*>(data) + size) {}

            bool IsProjection() const;

            bool operator <(const SBlock& block) const;

            void InsertData(
                std::size_t position,
                const void* data,
                std::size_t size);
        };

        std::vector<SBlock> m_blocks;

        unsigned long long m_totalSize;

        std::unique_ptr<IDataStorage> m_storage;
    };

    inline unsigned long long CChangeBuffer::GetSize() const
    {
        return m_totalSize;
    }

    inline void CChangeBuffer::SBlock::InsertData(
        std::size_t position,
        const void* data,
        std::size_t size)
    {
        assert(!IsProjection());
        m_data.insert(
            m_data.begin() + position,
            reinterpret_cast<const char*>(data),
            reinterpret_cast<const char*>(data) + size);
    }

    inline bool CChangeBuffer::SBlock::IsProjection() const
    {
        return std::numeric_limits<unsigned long long>::max() != m_offsetReal;
    }

    inline bool CChangeBuffer::SBlock::operator <(const SBlock& block) const
    {
        return m_offsetVirtual < block.m_offsetVirtual;
    }
}

#endif // CORE_CCHANGEBUFFER_H
