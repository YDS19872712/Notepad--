#include <Notepad--/CMainFrame.h>

#define MAX_TITLE 300

extern CAppModule _Module;

static const TCHAR FILTER[] = TEXT(
    "Text files (*.txt)\0*.txt\0All files\0*.*\0\0");

static const TCHAR DEFEXT[] = TEXT("txt");

LRESULT CMainFrame::OnCreate(LPCREATESTRUCT)
{
    SetTitle(NULL);
    SetMenu(::LoadMenu(_Module.m_hInst, MAKEINTRESOURCE(IDR_MAINFRAME)));

    RECT rc;
    GetClientRect(&rc);
    
    m_editor.Create(m_hWnd, rc);

    SetMsgHandled(false);
    return 0;
}

void CMainFrame::OnClose()
{
    DoExit();
}

LRESULT CMainFrame::OnSize(UINT nType, CSize size)
{
    if (m_editor.IsWindow()) {
        m_editor.SetWindowPos(
            HWND_BOTTOM,
            0, 0,
            size.cx, size.cy,
            SWP_NOMOVE);
    }
    return 0;
}

LRESULT CMainFrame::OnFileNew(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    HRESULT result = 0;

    if (PreventDataLoss()) {
        SetTitle(NULL);
        result = m_editor.SendMessage(ECM_FILE_NEW);
    }
    return result;
}

LRESULT CMainFrame::OnFileOpen(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    LRESULT result = 0;

    if (PreventDataLoss()) {
        CFileDialog fileDialog(TRUE, DEFEXT, NULL,
            OFN_FILEMUSTEXIST | OFN_EXPLORER, FILTER, m_hWnd);

        if (IDOK == fileDialog.DoModal()) {
            result = m_editor.SendMessage(ECM_FILE_OPEN, 0,
                reinterpret_cast<LPARAM>(fileDialog.m_szFileName));
            if (0 == result) {
                SetTitle(fileDialog.m_szFileTitle);
            }
        }
    }
    return result;
}

LRESULT CMainFrame::OnFileSave(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    return DoFileSave(false);
}

LRESULT CMainFrame::OnFileSaveAs(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    return DoFileSave(true);
}

LRESULT CMainFrame::OnAppExit(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    return DoExit() ? 0 : -1;
}

LRESULT CMainFrame::DoFileSave(bool askPath)
{
    HRESULT result = 0;

    if (askPath || m_editor.PathIsEmpty()) {
        CFileDialog fileDialog(FALSE, DEFEXT, NULL,
            OFN_OVERWRITEPROMPT | OFN_EXPLORER, FILTER, m_hWnd);

        ::lstrcpyn(fileDialog.m_szFileName, m_editor.GetPath(), MAX_PATH);

        if (IDOK == fileDialog.DoModal()) {
            result = m_editor.SendMessage(ECM_FILE_SAVE_AS, 0,
                reinterpret_cast<LPARAM>(fileDialog.m_szFileName));
            if (0 == result) {
                SetTitle(fileDialog.m_szFileTitle);
            }
        } else {
            result = -1;
        }
    } else {
        result = m_editor.SendMessage(ECM_FILE_SAVE);
    }

    return result;
}

bool CMainFrame::DoExit()
{
    if (PreventDataLoss()) {
        m_editor.DestroyWindow();
        DestroyWindow();
        ::PostQuitMessage(0);
        return true;
    }
    return false;
}

void CMainFrame::SetTitle(PCTSTR title)
{
    static TCHAR buffer[MAX_TITLE];

    if (!title) {
        title = TEXT("Untitled");
    }
    ::wnsprintf(buffer, MAX_TITLE, TEXT("%s - Notepad--"), title);
    SetWindowText(buffer);
}

bool CMainFrame::PreventDataLoss()
{
    if (m_editor.IsModified()) {
        int button = MessageBox(
            TEXT("The text has changed!\nDo you want to save it?"),
            TEXT("Attention!"),
            MB_YESNOCANCEL | MB_ICONEXCLAMATION);

        if ((IDNO == button) || ((IDYES == button) &&
                (0 == DoFileSave(false)))) {
            return true;
        }
    }
    return false;
}
