#include <Notepad--/CEditorCtrl.h>

void CEditorCtrl::DoPaint(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);
    dc.GradientFillRect(rc, RGB(255,0,0), RGB(100,100,0), FALSE);

    //SetScrollSize(1000000, 1000000, TRUE, false);
}
