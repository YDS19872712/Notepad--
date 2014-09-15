#include <Notepad--/CSavingDlg.h>

using namespace std;
using namespace Core;

#define TIMER_ID          1
#define TIMER_ELAPSE      10
#define PROGRESS_RANGE    100
#define WRITE_BUFFER_SIZE 4096   

bool CSavingDlg::OnInitDialog(CWindow, LPARAM)
{
    m_bytesTotal = m_source->GetSize();
    m_bytesWritten = 0;

    m_progress = GetDlgItem(IDC_SAVING_PROGRESS);

    m_progress.SetRange(0, PROGRESS_RANGE);
    m_progress.SetPos(0);

    m_buffers[0].resize(WRITE_BUFFER_SIZE);
    m_buffers[1].resize(WRITE_BUFFER_SIZE);
    m_readBufferIndex = 0;

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

    if (m_trackers[m_readBufferIndex] &&
        (ITracker::STATE_PENDING == m_trackers[m_readBufferIndex]->GetState())) {
        return;
    }

    size_t size = min(
        WRITE_BUFFER_SIZE,
        static_cast<size_t>(m_bytesTotal - m_bytesWritten));

    m_trackers[m_readBufferIndex] = m_destination->Write(
        m_bytesWritten,
        &m_buffers[m_readBufferIndex][0], size);

    m_bytesWritten += size;

    if (m_bytesTotal > 0) {
        m_progress.SetPos(
            static_cast<int>((
                static_cast<double>(m_bytesWritten) / m_bytesTotal) *
            PROGRESS_RANGE));
        m_progress.RedrawWindow();
    }
}
LRESULT CSavingDlg::OnCancel(WORD, WORD, HWND, BOOL& handled)
{
    EndDialog(0);
    handled = TRUE;
    return 0;
}
