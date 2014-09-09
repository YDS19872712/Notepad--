#include <Notepad--/CEditorCtrl.h>

void CEditorCtrl::DoPaint(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);
    dc.GradientFillRect(rc, RGB(255,255,0), RGB(100,100,255), FALSE);

    SetScrollSize(600, 600, TRUE, false);
}
