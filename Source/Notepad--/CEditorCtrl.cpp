#include <Core/CFile.h>
#include <Core/CNullStorage.h>
#include <Notepad--/CEditorCtrl.h>

using namespace std;
using namespace Core;

LRESULT CEditorCtrl::OnCreate(LPCREATESTRUCT)
{
    SetClassLongPtr(
        m_hWnd, GCLP_HCURSOR,
        reinterpret_cast<LONG_PTR>(
            ::LoadCursor(NULL, IDC_IBEAM)));

    m_changeBuffer = unique_ptr<CChangeBuffer>(new CChangeBuffer(
        unique_ptr<IDataStorage>(new CNullStorage())));

    SetMsgHandled(false);
    return 0;
}

void CEditorCtrl::DoPaint(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);

    int p = GetScrollPos(SB_VERT);
    wchar_t b[100];
    wsprintfW(b, L"%i", p);

    dc.TextOutW(10, p + 10, b, lstrlenW(b));

    SetScrollSize(100, 100600, TRUE, false);
}
