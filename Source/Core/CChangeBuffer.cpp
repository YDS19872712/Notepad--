#include <limits>
#include <Core/CChangeBuffer.h>

using namespace std;
using namespace Core;

CChangeBuffer::CChangeBuffer(unique_ptr<IDataStorage> storage)
    : m_storage(std::move(storage))
{
    const unsigned long long MAX_BLOCK_SIZE =
        std::numeric_limits<std::size_t>::max();
}

CChangeBuffer::~CChangeBuffer()
{
    ;
}
