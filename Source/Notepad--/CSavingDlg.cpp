#include <Notepad--/CSavingDlg.h>

using namespace std;
using namespace Core;

#define TIMER_ID          1
#define TIMER_ELAPSE      1
#define PROGRESS_RANGE    100
#define BUFFER_SIZE       0x40000

bool CSavingDlg::OnInitDialog(CWindow, LPARAM)
{
    m_bytesTotal = m_source->GetSize();
    m_bytesWritten = 0;
    m_bytesRead = 0;

    m_progress = GetDlgItem(IDC_SAVING_PROGRESS);

    m_progress.SetRange(0, PROGRESS_RANGE);
    m_progress.SetPos(0);

    m_readBuffer.reserve(BUFFER_SIZE);
    m_writeBuffer.reserve(BUFFER_SIZE);

    SetTimer(TIMER_ID, TIMER_ELAPSE);

    return false;
}

void CSavingDlg::OnDestroy()
{
    KillTimer(TIMER_ID);
}

void CSavingDlg::OnTimer(UINT_PTR id)
{
    ATLASSERT(TIMER_ID == id);

    if (m_bytesWritten >= m_bytesTotal) {
        EndDialog(0);
        return;
    }

    bool readNext = false;

    if (m_readTracker &&
            (ITracker::STATE_COMPLETE == m_readTracker->GetState())) {
        if (!m_writeTracker ||
                (ITracker::STATE_COMPLETE == m_writeTracker->GetState())) {
            m_bytesWritten += m_writeBuffer.size();

            double value = static_cast<double>(m_bytesWritten) / m_bytesTotal;
            m_progress.SetPos(static_cast<int>(value * PROGRESS_RANGE));
            m_progress.RedrawWindow();

            if (m_bytesWritten >= m_bytesTotal) {
                return;
            }

            swap(m_readBuffer, m_writeBuffer);
            if (m_writeBuffer.size() > 0) {
                m_writeTracker = m_destination->Write(
                    m_bytesWritten, &m_writeBuffer[0], m_writeBuffer.size());
            }
            readNext = true;
        }
    }

    if (readNext || !m_readTracker) {
        size_t size = min(BUFFER_SIZE,
            static_cast<size_t>(m_bytesTotal - m_bytesRead));
        if (size > 0) {
            m_readBuffer.resize(size);
            m_readTracker = m_source->Read(
                m_bytesRead, &m_readBuffer[0], size);
            m_bytesRead += size;
        }
    }
}

LRESULT CSavingDlg::OnCancel(WORD, WORD, HWND, BOOL& handled)
{
    EndDialog(-1);
    handled = TRUE;
    return 0;
}
