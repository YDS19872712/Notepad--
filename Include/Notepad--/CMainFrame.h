#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlframe.h>

/**
 * The main window of Notepad--.
 */
class CMainFrame
    : public CFrameWindowImpl<CMainFrame>
//  , public CUpdateUI<CMainFrame>
{
public:

	DECLARE_FRAME_WND_CLASS(NULL, 0);

private:

	BEGIN_MSG_MAP(CMainFrame)

		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

    END_MSG_MAP();

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
};

#endif // CMAINFRAME_H
