#include <Core/CTrackerComplete.h>
#include <Core/CNullStorage.h>

using namespace std;
using namespace Core;

long long CNullStorage::GetSize() const
{
    return 0;
}

unique_ptr<ITracker> CNullStorage::Read(
    unsigned long long offset,
    size_t size,
    void* buffer) const
{
    return unique_ptr<ITracker>(new CTrackerComplete);
}

unique_ptr<ITracker> CNullStorage::Write(
    unsigned long long offset,
    size_t size,
    const void* buffer)
{
    return unique_ptr<ITracker>(new CTrackerComplete);
}
