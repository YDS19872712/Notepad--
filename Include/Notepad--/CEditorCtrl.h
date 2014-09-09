#ifndef CEDITORCTRL_H
#define CEDITORCTRL_H

#include <memory>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlcrack.h>
#include <atlscrl.h>
#include <Core/CChangeBuffer.h>

#define WM_OPEN_FILE WM_USER + 1

class CEditorCtrl : public CScrollWindowImpl<CEditorCtrl>
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

private:

    BEGIN_MSG_MAP(CEditorCtrl)

        MSG_WM_CREATE(OnCreate)

        CHAIN_MSG_MAP(CScrollWindowImpl<CEditorCtrl>)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    void DoPaint(CDCHandle);

    std::unique_ptr<Core::CChangeBuffer> m_changeBuffer;
};

#endif // CEDITORCTRL_H
