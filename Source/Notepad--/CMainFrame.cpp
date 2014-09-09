#include <Notepad--/CMainFrame.h>

extern CAppModule _Module;

LRESULT CMainFrame::OnCreate(LPCREATESTRUCT)
{
    SetMenu(::LoadMenu(_Module.m_hInst, MAKEINTRESOURCE(IDR_MAINFRAME)));

    RECT rc;
    GetClientRect(&rc);
    m_editor.Create(m_hWnd, rc);

    SetMsgHandled(false);
    return 0;
}

void CMainFrame::OnDestroy()
{
    ::PostQuitMessage(0);
}

LRESULT CMainFrame::OnSize(UINT nType, CSize size)
{
    if (m_editor.IsWindow()) {
        m_editor.SetWindowPos(
            NULL, 0, 0, size.cx, size.cy,
            SWP_NOMOVE | SWP_NOZORDER);
    }
    return 0;
}
