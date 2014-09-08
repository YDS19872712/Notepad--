#include <Core/CTrackerComplete.h>
#include <Core/CNullStorage.h>

using namespace std;
using namespace Core;

unsigned long long CNullStorage::GetSize() const
{
    return 0;
}

unique_ptr<ITracker> CNullStorage::Read(
    unsigned long long offset,
    void* buffer,
    size_t size) const
{
    return unique_ptr<ITracker>(new CTrackerComplete);
}

unique_ptr<ITracker> CNullStorage::Write(
    unsigned long long offset,
    const void* buffer,
    size_t size)
{
    return unique_ptr<ITracker>(new CTrackerComplete);
}
