#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atldlgs.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlframe.h>
#include <Notepad--/CEditorCtrl.h>
#include <Notepad--/CAboutWindow.h>
#include <Notepad--/resource.h>

/**
 * The main window of Notepad--.
 */
class CMainFrame
    : public CFrameWindowImpl<CMainFrame>
    , public CUpdateUI<CMainFrame>
{
public:

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME);

    BEGIN_UPDATE_UI_MAP(CMainFrame)

        UPDATE_ELEMENT(ID_FORMAT_WORDWRAP, UPDUI_MENUPOPUP)

    END_UPDATE_UI_MAP()

private:

	BEGIN_MSG_MAP(CMainFrame)

        MSG_WM_CREATE (OnCreate)
        MSG_WM_CLOSE  (OnClose)

        COMMAND_ID_HANDLER(ID_FILE_NEW,        OnFileNew)
        COMMAND_ID_HANDLER(ID_FILE_OPEN,       OnFileOpen)
        COMMAND_ID_HANDLER(ID_FILE_SAVE,       OnFileSave)
        COMMAND_ID_HANDLER(ID_FILE_SAVE_AS,    OnFileSaveAs)
        COMMAND_ID_HANDLER(ID_APP_EXIT,        OnAppExit)
        COMMAND_ID_HANDLER(ID_FORMAT_WORDWRAP, OnFormatWordWrap)
        COMMAND_ID_HANDLER(ID_FORMAT_FONT,     OnFormatFont)
        COMMAND_ID_HANDLER(ID_HELP_ABOUT,      OnHelpAbout)

        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    void OnClose();

    LRESULT OnSize(UINT, CSize);

    LRESULT OnFileNew(WORD, WORD, HWND, BOOL&);

    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);

    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&);

    LRESULT OnFileSaveAs(WORD, WORD, HWND, BOOL&);

    LRESULT OnAppExit(WORD, WORD, HWND, BOOL&);

    LRESULT OnFormatWordWrap(WORD, WORD, HWND, BOOL&);

    LRESULT OnFormatFont(WORD, WORD, HWND, BOOL&);

    LRESULT OnHelpAbout(WORD, WORD, HWND, BOOL&);

    /**
     * Resizes inner editor control.
     */
    void UpdateLayout(BOOL = FALSE);

    /**
     * Shows a file dialog to a user and 
     * asks editor to open chosen file.
     */
    LRESULT DoFileSave(bool);

    /**
     * A routine to exit the application.
     */
    bool DoExit();

    /**
     * Sets the title of the window with appending
     * application's name at the end.
     */
    void SetTitle(PCTSTR);

    /**
     * Checks if the data in the editor where modified
     * and prososes to a user to save them.
     */
    bool PreventDataLoss();

    /**
     * An editor child control.
     */
    CEditorCtrl m_editor;

    /**
     * A window that shows README.md
     */
    CAboutWindow m_about;
};

#endif // CMAINFRAME_H
