#ifndef CABOUTWINDOW_H
#define CABOUTWINDOW_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlcrack.h>
#include <atlscrl.h>

class CAboutWindow : public CScrollWindowImpl<CAboutWindow>
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

private:

    BEGIN_MSG_MAP(CAboutWindow)
 
        MSG_WM_CREATE    (OnCreate)
        MSG_WM_DESTROY   (OnDestroy)
        MSG_WM_CLOSE     (OnClose)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)

        CHAIN_MSG_MAP(CScrollWindowImpl<CAboutWindow>)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    void OnDestroy();

    void OnClose();

    BOOL OnEraseBkgnd(CDCHandle);

    void DoPaint(CDCHandle);

    HRSRC m_hResource;

    int m_sizeOfResource;

    HGLOBAL m_hGlobal;

    char* m_text;
};

#endif // CABOUTWINODOW_H
