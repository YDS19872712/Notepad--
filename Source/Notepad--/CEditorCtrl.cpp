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
    dc.GradientFillRect(rc, RGB(255,255,0), RGB(100,100,255), FALSE);

    SetScrollSize(600, 600, TRUE, false);
}
