#ifndef CORE_CFILETRACKER_H
#define CORE_CFILETRACKER_H

#include <windows.h>
#include <Core/ITracker.h>

namespace Core {
    /**
     * A class that allows to track asynchronous file operations.
     */
    class CFileTracker : public ITracker
    {
    public:

        CFileTracker();

        virtual ~CFileTracker();

        virtual State GetState() const;

        void SetOffset(unsigned long long offset);

        OVERLAPPED* GetOverlapped();

    private:

        OVERLAPPED m_overlapped;
    };

    inline void CFileTracker::SetOffset(unsigned long long offset)
    {
        ULARGE_INTEGER fileOffset;
        fileOffset.QuadPart = offset;
        m_overlapped.Offset = fileOffset.LowPart;
        m_overlapped.OffsetHigh = fileOffset.HighPart;
    }

    inline OVERLAPPED* CFileTracker::GetOverlapped()
    {
        return &m_overlapped;
    }
}

#endif // CORE_CFILETRACKER_H
