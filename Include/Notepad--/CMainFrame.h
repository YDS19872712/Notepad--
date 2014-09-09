#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlframe.h>
#include <Notepad--/CEditorCtrl.h>
#include <Notepad--/resource.h>

/**
 * The main window of Notepad--.
 */
class CMainFrame
    : public CFrameWindowImpl<CMainFrame>
//  , public CUpdateUI<CMainFrame>
{
public:

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME);

private:

	BEGIN_MSG_MAP(CMainFrame)

        MSG_WM_CREATE (OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE   (OnSize)

        COMMAND_ID_HANDLER(ID_FILE_NEW,  OnFileNew)
        COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)

        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
//      CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    void OnDestroy();

    LRESULT OnSize(UINT, CSize);

    LRESULT OnFileNew(WORD, WORD, HWND, BOOL&);

    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);

    CEditorCtrl m_editor;
};

#endif // CMAINFRAME_H
