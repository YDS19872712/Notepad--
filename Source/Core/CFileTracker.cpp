#include <Core/CFileTracker.h>

using Core::CFileTracker;

CFileTracker::CFileTracker()
    : m_overlapped()
{
    m_overlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

CFileTracker::~CFileTracker()
{
    if (NULL != m_overlapped.hEvent) {
        ::CloseHandle(m_overlapped.hEvent);
        m_overlapped.hEvent = NULL;
    }
}

CFileTracker::State CFileTracker::GetState() const
{
    return HasOverlappedIoCompleted(&m_overlapped)
        ? STATE_COMPLETE
        : STATE_PENDING;
}
