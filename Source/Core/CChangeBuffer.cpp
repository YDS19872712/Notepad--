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

CChangeBuffer::~CChangeBuffer()
{
    ;
}

unique_ptr<ITracker> CChangeBuffer::Read(
    unsigned long long offset,
    void* buffer,
    size_t size) const
{
    if (0 == size) {
        return unique_ptr<ITracker>(new CTrackerComplete);
    }

    if ((offset > m_totalSize) && (offset + size > m_totalSize)) {
        return unique_ptr<ITracker>(new CTrackerFailed);
    }

    vector<unique_ptr<ITracker>> trackers;

    auto first = upper_bound(
        m_blocks.cbegin(), m_blocks.cend(), SBlock(offset, 0));

    auto last = upper_bound(
        m_blocks.cbegin(), m_blocks.cend(), SBlock(offset + size, 0));

    char* out = reinterpret_cast<char*>(buffer);
    size_t left = size;

    for (auto it = first - 1; it != last; ++it) {
        size_t offsetInner = (offset > it->m_offsetVirtual)
            ? static_cast<size_t>(offset - it->m_offsetVirtual) : 0;

        auto ahead = it + 1;

        size_t sizeOfChunk = min(
            (static_cast<size_t>((m_blocks.cend() == ahead ? m_totalSize : ahead->m_offsetVirtual) -
                (it->m_offsetVirtual + offsetInner))),
            left);

        if (it->IsProjection()) {
            trackers.push_back(m_storage->Read(
                it->m_offsetReal + offsetInner, out, sizeOfChunk));
        } else {
            ::memcpy(out, &it->m_data[0], sizeOfChunk);
        }
        out += sizeOfChunk;
        size -= sizeOfChunk;
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
