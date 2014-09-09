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
