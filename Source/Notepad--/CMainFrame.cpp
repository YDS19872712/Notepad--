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
            HWND_BOTTOM,
            0, 0,
            size.cx, size.cy,
            SWP_NOMOVE);
    }
    return 0;
}

LRESULT CMainFrame::OnFileNew(WORD, WORD, HWND, BOOL& bHandled)
{
    SetWindowText(TEXT("OnFileNew"));
    bHandled = TRUE;
    return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD, WORD, HWND, BOOL& bHandled)
{
    SetWindowText(TEXT("OnFileOpen"));
    bHandled = TRUE;
    return 0;
}
