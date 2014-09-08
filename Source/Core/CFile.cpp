#include <Core/CTrackerComplete.h>
#include <Core/CTrackerFailed.h>
#include <Core/CFileTracker.h>
#include <Core/CFile.h>

using namespace std;
using namespace Core;

CFile::CFile(PCTSTR path, unsigned int mode)
{
    DWORD desiredAccess =
        (mode & MODE_READ  ? GENERIC_READ  : 0) |
        (mode & MODE_WRITE ? GENERIC_WRITE : 0);

    DWORD creationDisposition =
        mode & MODE_WRITE ? OPEN_ALWAYS : (
            mode & MODE_READ ? OPEN_EXISTING : 0);

    m_handle = ::CreateFile(
        path,
        desiredAccess,
        FILE_SHARE_READ,
        NULL,
        creationDisposition,
        FILE_FLAG_OVERLAPPED,
        NULL);
}

CFile::CFile(const CFile& file)
    : m_handle(INVALID_HANDLE_VALUE)
{
    *this = file;
}

CFile::CFile(CFile&& file)
    : m_handle(INVALID_HANDLE_VALUE)
{
    *this = file;
}

CFile::~CFile()
{
    Close();
    m_handle = INVALID_HANDLE_VALUE;
}

CFile& CFile::operator =(const CFile& file)
{
    Close();
    ::DuplicateHandle(
        ::GetCurrentProcess(),
        file.m_handle,
        ::GetCurrentProcess(),
        &m_handle,
        0,
        FALSE,
        DUPLICATE_SAME_ACCESS);
    return *this;
}

CFile& CFile::operator =(CFile&& file)
{
    Close();
    m_handle = file.m_handle;
    file.m_handle = INVALID_HANDLE_VALUE;
    return *this;
}

unsigned long long CFile::GetSize() const
{
    LARGE_INTEGER result;
    if (::GetFileSizeEx(m_handle, &result)) {
        return static_cast<unsigned long long>(result.QuadPart);
    }
    return 0;
}

unique_ptr<ITracker> CFile::Read(
    unsigned long long offset,
    void* buffer,
    size_t size) const
 {
    auto fileTracker = new CFileTracker;
    fileTracker->SetOffset(offset);

    unique_ptr<ITracker> tracker(fileTracker);

    BOOL done = ::ReadFile(m_handle, buffer, size,
        NULL, fileTracker->GetOverlapped());

    if (done) {
        tracker.reset(new CTrackerComplete);
    } else if (ERROR_IO_PENDING != ::GetLastError()) {
        tracker.reset(new CTrackerFailed);
    }

    return tracker;
}

unique_ptr<ITracker> CFile::Write(
    unsigned long long offset,
    const void* buffer,
    size_t size)
{
    auto fileTracker = new CFileTracker;
    fileTracker->SetOffset(offset);

    unique_ptr<ITracker> tracker(fileTracker);

    BOOL done = ::WriteFile(m_handle, buffer, size,
        NULL, fileTracker->GetOverlapped());

    if (done) {
        tracker.reset(new CTrackerComplete);
    } else if (ERROR_IO_PENDING != ::GetLastError()) {
        tracker.reset(new CTrackerFailed);
    }

    return tracker;
}
