#include <Notepad--/CSavingDlg.h>

#define TIMER_ID       1
#define TIMER_ELAPSE   50
#define PROGRESS_RANGE 100

bool CSavingDlg::OnInitDialog(CWindow, LPARAM)
{
    m_bytesTotal = m_source->GetSize();
    m_bytesWritten = 0;

    m_progress = GetDlgItem(IDC_SAVING_PROGRESS);

    m_progress.SetRange(0, PROGRESS_RANGE);
    m_progress.SetPos(0);

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
    
    // ---
    m_bytesTotal = 1000;
    m_bytesWritten += 10;

    if (m_bytesTotal > 0) {
        m_progress.SetPos(
            static_cast<int>((
                static_cast<double>(m_bytesWritten) / m_bytesTotal) *
            PROGRESS_RANGE));
        m_progress.RedrawWindow();
    }
}
