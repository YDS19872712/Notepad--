#include <Notepad--/CMainFrame.h>

LRESULT CMainFrame::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
    ::PostQuitMessage(0);
    bHandled = TRUE;
    return 0;
}
