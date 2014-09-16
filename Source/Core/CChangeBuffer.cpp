#include <algorithm>
#include <iterator>
#include <Core/CTrackerComplete.h>
#include <Core/CTrackerFailed.h>
#include <Core/CBatchTracker.h>
#include <Core/CChangeBuffer.h>

using namespace std;
using namespace Core;

CChangeBuffer::CChangeBuffer(unique_ptr<IDataStorage> storage)
    : m_totalSize(storage->GetSize())
    , m_storage(move(storage))
{
    if (m_totalSize > 0) {
        m_blocks.push_back(SBlock(0, 0));
    }
}

unique_ptr<ITracker> CChangeBuffer::Read(
    unsigned long long offset,
    void* buffer,
    size_t size) const
{
    if ((offset >= m_totalSize) || (offset + size > m_totalSize)) {
        return unique_ptr<ITracker>(new CTrackerFailed);
    }

    if (0 == size) {
        return unique_ptr<ITracker>(new CTrackerComplete);
    }

    vector<unique_ptr<ITracker>> trackers;

    char* out = reinterpret_cast<char*>(buffer);
    size_t left = size;

    auto first = upper_bound(
        m_blocks.cbegin(), m_blocks.cend(), SBlock(offset, 0)) - 1;

    for (auto it = first; left > 0; ++it) {
        size_t offsetInner = (offset > it->m_offsetVirtual)
            ? static_cast<size_t>(offset - it->m_offsetVirtual) : 0;
        auto ahead = it + 1;

        size_t sizeOfChunk = min(
            left, 
            static_cast<size_t>(
                (m_blocks.cend() == ahead
                    ? m_totalSize
                    : ahead->m_offsetVirtual) -
                        it->m_offsetVirtual - offsetInner));

        if (it->IsProjection()) {
            trackers.push_back(m_storage->Read(
                it->m_offsetReal + offsetInner, out, sizeOfChunk));
        } else {
            ::memcpy(out, &it->m_data[offsetInner], sizeOfChunk);
        }

        out += sizeOfChunk;
        left -= sizeOfChunk;
    }

    return unique_ptr<ITracker>(new CBatchTracker(std::move(trackers)));
}

bool CChangeBuffer::Insert(
    unsigned long long offset,
    const void* data,
    size_t size)
{
    if (offset > m_totalSize) {
        return false;
    }

    if (0 == size) {
        return true;
    }

    m_totalSize += size;

    if (m_blocks.empty()) {
        m_blocks.push_back(SBlock(offset, data, size));
    } else {
        auto next = lower_bound(
            m_blocks.begin(), m_blocks.end(), SBlock(offset, 0));

        bool inserted = false;

        if (m_blocks.end() != next) {
            if ((next->m_offsetVirtual == offset) && !next->IsProjection()) {
                next->InsertData(0, data, size);
                inserted = true;
            }
            for (auto it = inserted ? next + 1 : next;
                    m_blocks.end() != it; ++it) {
                it->m_offsetVirtual += size;
            }
        }

        if (!inserted) {
            if (m_blocks.begin() == next) {
                m_blocks.insert(next, SBlock(offset, data, size));
            } else {
                auto block = next - 1;
                size_t offsetInner = static_cast<size_t>(
                    offset - block->m_offsetVirtual);
                if (block->IsProjection()) {
                    SBlock blocks[] = {
                        SBlock(offset, data, size),
                        SBlock(offset + size,
                            block->m_offsetReal + offsetInner) };
                    const size_t SIZE = sizeof(blocks) / sizeof(SBlock);
                    m_blocks.insert(next, 
                        make_move_iterator(&blocks[0]),
                        make_move_iterator(&blocks[SIZE]));
                } else {
                    block->InsertData(offsetInner, data, size);
                }
            }
        }
    }

    return true;
}

bool CChangeBuffer::Delete(unsigned long long offset, size_t size)
{
    size_t offsetEnd = static_cast<size_t>(offset + size);

    if ((offset >= m_totalSize) || (offsetEnd > m_totalSize)) {
        return false;
    }

    if (0 == size) {
        return true;
    }

    auto first = upper_bound(
        m_blocks.begin(), m_blocks.end(), SBlock(offset, 0));

    auto last = lower_bound(
        m_blocks.begin(), m_blocks.end(), SBlock(offsetEnd, 0));

    if (first == last) {
        auto block = last - 1;
        size_t offsetInner = static_cast<size_t>(
            offset - block->m_offsetVirtual);

        size_t sizeOfBlock = static_cast<size_t>(
            (m_blocks.end() == last)
                ? m_totalSize - block->m_offsetVirtual
                : last->m_offsetVirtual - block->m_offsetVirtual);

        for (auto it = last; it != m_blocks.end(); ++it) {
            it->m_offsetVirtual -= size;
        }

        if ((0 == offsetInner) && (size == sizeOfBlock)) {
            m_blocks.erase(block);
        }
        else if (block->IsProjection()) {
            if (size + offsetInner != sizeOfBlock) {
                if (0 == offsetInner) {
                    block->m_offsetReal += size;
                } else {
                    m_blocks.insert(last, SBlock(offset,
                        block->m_offsetReal + offsetInner + size));
                }
            }
        } else {
            block->m_data.erase(
                block->m_data.begin() + offsetInner,
                block->m_data.begin() + offsetInner + size);
        }
    } else {
        auto block = first - 1;
        std::vector<SBlock>::iterator start, stop;

        if (offset == block->m_offsetVirtual) {
            start = block;
        } else {
            start = first;
            if (!block->IsProjection()) {
                block->m_data.erase(
                    block->m_data.begin() +
                        static_cast<size_t>(offset - block->m_offsetVirtual),
                    block->m_data.end());
            }
        }

        if ((m_blocks.end() == last)
                ? offsetEnd == m_totalSize
                : offsetEnd == last->m_offsetVirtual) {
            stop = last;
        } else {
            stop = last - 1;
            size_t rest = static_cast<size_t>(
                offsetEnd - stop->m_offsetVirtual);

            if (!stop->IsProjection()) {
                stop->m_data.erase(
                    stop->m_data.begin(),
                    stop->m_data.begin() + rest);
            } else {
                stop->m_offsetReal += rest;
            }
        }

        for (auto it = stop; it != m_blocks.end(); ++it) {
            it->m_offsetVirtual -= size;
        }

        if (start != stop) {
            m_blocks.erase(start, stop);
        }
    }

    m_totalSize -= size;

    return true;
}