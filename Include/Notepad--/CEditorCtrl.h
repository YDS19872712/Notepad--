#ifndef CEDITORCTRL_H
#define CEDITORCTRL_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlscrl.h>

class CEditorCtrl :  public CMapScrollWindowImpl<CEditorCtrl>
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

private:

    BEGIN_MSG_MAP(CEditorCtrl)

        CHAIN_MSG_MAP(CMapScrollWindowImpl<CEditorCtrl>)

    END_MSG_MAP();

    void DoPaint(CDCHandle);
};

#endif // CEDITORCTRL_H
