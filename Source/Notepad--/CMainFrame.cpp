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

    CEditorCtrl::Init();
    m_editor.Create(m_hWnd, rc);

    UISetCheck(ID_FORMAT_WORDWRAP, TRUE);
    m_editor.SendMessage(ECM_FORMAT_WORDWRAP, TRUE);

    SetMsgHandled(false);
    return 0;
}

void CMainFrame::OnClose()
{
    DoExit();
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

LRESULT CMainFrame::OnFormatWordWrap(WORD, WORD, HWND, BOOL& handled)
{
    handled = TRUE;
    DWORD state = UIGetState(ID_FORMAT_WORDWRAP);
    bool checked = !((UPDUI_CHECKED & state) || (UPDUI_CHECKED2 & state));
    m_editor.SendMessage(ECM_FORMAT_WORDWRAP, checked);
    UISetCheck(ID_FORMAT_WORDWRAP, checked);
    return 0;
}

LRESULT CMainFrame::OnFormatFont(WORD, WORD, HWND, BOOL& handled)
{
    CFontDialog fd;    
    m_editor.GetFont(fd);
    if (IDOK == fd.DoModal()) {
        m_editor.SetFont(fd);
    }
    return 0;
}

LRESULT CMainFrame::OnHelpAbout(WORD, WORD, HWND, BOOL& handled)
{
    if (m_about.IsWindow()) {
        m_about.SetActiveWindow();
    } else {
        m_about.Create(
            ::GetDesktopWindow(),
            CWindow::rcDefault,
            TEXT("README.md"),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            WS_EX_TOOLWINDOW);
    }
    handled = TRUE;
    return 0;
}

void CMainFrame::UpdateLayout(BOOL)
{
    if (m_editor.IsWindow()) {
        RECT rc;
        GetClientRect(&rc);
        m_editor.SetWindowPos(NULL, &rc, SWP_NOMOVE | SWP_NOZORDER);
    }
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
        m_editor.m_hWnd = NULL;
        if (m_about.IsWindow()) {
            m_about.DestroyWindow();
            m_about.m_hWnd = NULL;
        }
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
        return false;
    }
    return true;
}
