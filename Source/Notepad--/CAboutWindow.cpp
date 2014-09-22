#include <Notepad--/resource.h>
#include <Notepad--/CAboutWindow.h>

#define WIDTH  600
#define HEIGHT 700

extern CAppModule _Module;

LRESULT CAboutWindow::OnCreate(LPCREATESTRUCT)
{
    SetScrollSize(WIDTH, HEIGHT, TRUE, false);

    m_hResource = ::FindResource(
        _Module.m_hInst,
        MAKEINTRESOURCE(IDR_README_MD),
        RT_RCDATA);

    m_sizeOfResource = static_cast<int>(
        ::SizeofResource(_Module.m_hInst, m_hResource));

    m_hGlobal = ::LoadResource(_Module.m_hInst, m_hResource);
    m_text = reinterpret_cast<char*>(::LockResource(m_hGlobal));

    SetMsgHandled(false);
    return 0;
}

void CAboutWindow::OnDestroy()
{
    ::FreeResource(m_hGlobal);
}

void CAboutWindow::OnClose()
{
    DestroyWindow();
}

BOOL CAboutWindow::OnEraseBkgnd(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);
    dc.FillRect(&rc, COLOR_WINDOW);
    return TRUE;
}

void CAboutWindow::DoPaint(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);
    ::DrawTextA(dc, m_text, m_sizeOfResource, &rc, 0);
}
