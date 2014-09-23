#include <shlwapi.h>
#include <Scintilla.h>
#include <Core/CFile.h>
#include <Core/CNullStorage.h>
#include <Notepad--/CSavingDlg.h>
#include <Notepad--/CEditorCtrl.h>

using namespace std;
using namespace Scintilla;
using namespace Core;

#define IDLE_TIMER_ID       1
#define IDLE_TIMER_ELAPSE   50
#define READ_BUFFER_SIZE    4096
#define PRELOAD_SIZE        READ_BUFFER_SIZE * 1000

extern CAppModule _Module;

void CEditorCtrl::Init()
{
    Scintilla_RegisterClasses(_Module.m_hInst);
}

bool CEditorCtrl::IsModified() const
{
    return m_scintilla.Send(SCI_GETMODIFY) != 0;
}

void CEditorCtrl::SetFont(CFontDialog& fd)
{
#ifdef _UNICODE

    char faceName[LF_FACESIZE];

    ::WideCharToMultiByte(
        CP_UTF8, 0, fd.GetFaceName(), -1,
        faceName, LF_FACESIZE, NULL, NULL);

#define FONT_FACE_NAME faceName

#else

#define FONT_FACE_NAME fd.GetFaceName()

#endif

    m_scintilla.Send(
        SCI_STYLESETFONT,
        STYLE_DEFAULT,
        reinterpret_cast<int>(FONT_FACE_NAME));

    m_scintilla.Send(
        SCI_STYLESETFORE,
        STYLE_DEFAULT,
        fd.GetColor());

    m_scintilla.Send(
        SCI_STYLESETUNDERLINE,
        STYLE_DEFAULT,
        fd.IsUnderline());

    m_scintilla.Send(
        SCI_STYLESETITALIC,
        STYLE_DEFAULT,
        fd.IsItalic());

    m_scintilla.Send(
        SCI_STYLESETWEIGHT,
        STYLE_DEFAULT,
        fd.GetWeight());

    CDC dc(GetDC());

    int fontSize = -::MulDiv(
        fd.m_cf.lpLogFont->lfHeight, 72,
        dc.GetDeviceCaps(LOGPIXELSY));

    m_scintilla.Send(
        SCI_STYLESETSIZE,
        STYLE_DEFAULT,
        fontSize);

    m_scintilla.Send(SCI_STYLECLEARALL);
}

void CEditorCtrl::GetFont(CFontDialog& fd) const
{
    LOGFONT lf;

#ifdef _UNICODE

    char faceName[LF_FACESIZE];

    m_scintilla.Send(
        SCI_STYLEGETFONT,
        STYLE_DEFAULT,
        reinterpret_cast<int>(faceName));

    ::MultiByteToWideChar(
        CP_UTF8, 0, faceName, -1,
        lf.lfFaceName, LF_FACESIZE);

#else

    m_scintilla.Send(
        SCI_STYLEGETFONT,
        STYLE_DEFAULT,
        reinterpret_cast<int>(lf.lfFaceName));

#endif

    lf.lfUnderline = m_scintilla.Send(SCI_STYLEGETUNDERLINE, STYLE_DEFAULT);
    lf.lfItalic = m_scintilla.Send(SCI_STYLEGETITALIC, STYLE_DEFAULT);
    lf.lfWeight = m_scintilla.Send(SCI_STYLEGETWEIGHT, STYLE_DEFAULT);

    lf.lfStrikeOut = FALSE;

    CDC dc(::GetDC(m_hWnd));

    lf.lfHeight = -::MulDiv(
        m_scintilla.Send(SCI_STYLEGETSIZE, STYLE_DEFAULT),
        dc.GetDeviceCaps(LOGPIXELSY), 72);

    fd.SetLogFont(&lf);

    fd.m_cf.rgbColors = static_cast<COLORREF>(
        m_scintilla.Send(SCI_STYLEGETFORE, STYLE_DEFAULT));
}

LRESULT CEditorCtrl::OnCreate(LPCREATESTRUCT)
{
    m_buffer.reserve(READ_BUFFER_SIZE);

    RECT rc;
    GetClientRect(&rc);

    m_scintilla.m_wnd.Create(
        TEXT("Scintilla"),
        m_hWnd,
        &rc,
        NULL,
        WS_CHILD | WS_VISIBLE,
        0,
        SCINTILLA_CTRL_ID);

    m_scintilla.m_fn = reinterpret_cast<decltype(m_scintilla.m_fn)>(
        m_scintilla.m_wnd.SendMessage(SCI_GETDIRECTFUNCTION, 0, 0));

    m_scintilla.m_ptr = reinterpret_cast<void*>(
        m_scintilla.m_wnd.SendMessage(SCI_GETDIRECTPOINTER, 0, 0));

    m_scintilla.Send(SCI_SETCODEPAGE, SC_CP_UTF8);
    m_scintilla.Send(SCI_SETMARGINWIDTHN, SC_MARGIN_NUMBER, 0);
    m_scintilla.Send(SCI_SETSCROLLWIDTHTRACKING, TRUE);
    m_scintilla.Send(SCI_SETHSCROLLBAR, TRUE);
    m_scintilla.Send(SCI_SETVSCROLLBAR, TRUE);

    DoFileNew();

    _Module.GetMessageLoop()->AddIdleHandler(this);

    SetTimer(IDLE_TIMER_ID, IDLE_TIMER_ELAPSE);

    SetMsgHandled(false);
    return 0;
}

void CEditorCtrl::OnDestroy()
{
    KillTimer(IDLE_TIMER_ID);
    _Module.GetMessageLoop()->RemoveIdleHandler(this);
}

LRESULT CEditorCtrl::OnSize(UINT nType, CSize size)
{
    if (m_scintilla.m_wnd.IsWindow()) {
        m_scintilla.m_wnd.SetWindowPos(
            NULL, 0, 0, size.cx, size.cy,
            SWP_NOMOVE | SWP_NOZORDER);
    }
    return 0;
}

LRESULT CEditorCtrl::OnFileNew(UINT, WPARAM, LPARAM , BOOL& handled)
{
    DoFileNew();
    handled = true;
    return 0;
}

LRESULT CEditorCtrl::OnFileOpen(UINT, WPARAM, LPARAM lParam, BOOL& handled)
{
    handled = true;

    PCTSTR path = reinterpret_cast<PCTSTR>(lParam);
    ::lstrcpyn(m_path, path, MAX_PATH);

    if (DoFileOpen()) {
        return 0;
    }
    m_path[0] = 0;
    return -1;
}

LRESULT CEditorCtrl::OnFileSave(UINT, WPARAM, LPARAM, BOOL& handled)
{
    handled = true;
    if (!IsModified() && (IDNO == MessageBox(
            TEXT("No changes were made! Do you really want to proceed?"),
            TEXT("Why Oh Why?"),
            MB_YESNO | MB_ICONEXCLAMATION))) {
        return 0;
    }
    return DoFileSave() ? 0 : -1;
}

LRESULT CEditorCtrl::OnFormatWordWrap(
    UINT, WPARAM wrap, LPARAM, BOOL& handled)
{
    handled = TRUE;
    m_scintilla.Send(SCI_SETWRAPMODE, wrap ? SC_WRAP_WORD : SC_WRAP_NONE);
    return 0;
}

LRESULT CEditorCtrl::OnFileSaveAs(UINT, WPARAM, LPARAM lParam, BOOL& handled)
{
    handled = true;
    PCTSTR path = reinterpret_cast<PCTSTR>(lParam);
    ::lstrcpyn(m_path, path, MAX_PATH);
    return DoFileSave() ? 0 : -1;
}

LRESULT CEditorCtrl::OnScintillaUpdateUI(int id, NMHDR* hdr, BOOL& handled)
{
    ATLASSERT(SCINTILLA_CTRL_ID == id);
    SCNotification* notification = reinterpret_cast<SCNotification*>(hdr);

    if (notification->updated & SC_UPDATE_V_SCROLL) {
        int lineNo = m_scintilla.Send(SCI_GETFIRSTVISIBLELINE) + 1;
        int totalLines = m_scintilla.Send(SCI_GETLINECOUNT);
        while (m_scintilla.Send(SCI_GETLINEVISIBLE, lineNo) &&
                (lineNo < totalLines)) {
            ++lineNo;
        }
        int pos = m_scintilla.Send(SCI_POSITIONFROMLINE, lineNo);
        if (pos > m_bytesToRead / 2) {
            m_bytesToRead += min(
                READ_BUFFER_SIZE,
                m_bytesTotal - m_bytesToRead);
        }
    }

    handled = TRUE;
    return 0;
}

BOOL CEditorCtrl::OnIdle()
{
    if (m_bytesToRead > m_bytesRead) {
        if (m_tracker) {
            auto state = m_tracker->GetState();
            if (ITracker::STATE_COMPLETE == state) {
                bool modified = IsModified();
                m_scintilla.Send(
                    SCI_APPENDTEXT,
                    m_buffer.size(),
                    reinterpret_cast<int>(&m_buffer[0]));
                if (!modified) {
                    m_scintilla.Send(SCI_SETSAVEPOINT);
                }
                m_bytesRead += m_buffer.size();
            } else if (ITracker::STATE_PENDING == state) {
                return TRUE;
            }
        }
        size_t bytesLeft = static_cast<size_t>(m_bytesToRead - m_bytesRead);
        if (bytesLeft > 0) {
            m_buffer.resize(min(bytesLeft, READ_BUFFER_SIZE));
            m_tracker = m_changeBuffer->Read(
                m_bytesRead, &m_buffer[0], m_buffer.size());
        }
    } else if (m_tracker) {
        m_tracker.reset();
    }
    return TRUE;
}

void CEditorCtrl::DoFileNew()
{
    m_bytesRead   = 0;
    m_bytesToRead = 0;
    m_bytesTotal  = 0;
    m_path[0]     = 0;

    m_changeBuffer.reset(
        new CChangeBuffer(
            unique_ptr<IDataStorage>(
                new CNullStorage())));
    ResetScintilla();
}

bool CEditorCtrl::DoFileOpen()
{
    CFile* file = new CFile(m_path, CFile::MODE_READ);
    unique_ptr<IDataStorage> storage(file);

    if (file->IsOpen()) {
        m_changeBuffer.reset(new CChangeBuffer(std::move(storage)));
        m_bytesRead  = 0;
        m_bytesTotal = m_changeBuffer->GetSize();

        static const PTCHAR text =
            TEXT("The file is rather big!\n")
            TEXT("Do you want to load the whole of it?\n")
            TEXT("Otherwise it will be loaded part by part on scroll down.");

        if ((m_bytesTotal > PRELOAD_SIZE) &&
            (MessageBox(text, TEXT("You to decide!"),
                MB_ICONINFORMATION | MB_YESNO) == IDYES)) {
            m_bytesToRead = m_bytesTotal;
        } else {
            m_bytesToRead = min(m_bytesTotal, PRELOAD_SIZE);
        }
        ResetScintilla();
        return true;
    }

    MessageBox(
        TEXT("Cannot open the file!"),
        TEXT("Ouch!"),
        MB_ICONEXCLAMATION);

    return false;
}

bool CEditorCtrl::DoFileSave()
{
    CSavingDlg dlg;

    TCHAR tmpPath[MAX_PATH + 1];
    ::lstrcpy(tmpPath, m_path);
    ::lstrcat(tmpPath, TEXT("~"));

    unsigned int mode = CFile::MODE_WRITE;

    if(::PathFileExists(tmpPath)) {
        mode |= CFile::MODE_TRUNCATE;
    }

    // TODO: Rewrite. In the real world this will be
    // very time-consuming.
    m_changeBuffer->Delete(0, static_cast<size_t>(m_bytesRead));
    int len = m_scintilla.Send(SCI_GETLENGTH);
    vector<BYTE> tmp;
    tmp.resize(len + 1);
    m_scintilla.Send(SCI_GETTEXT, tmp.size(),
        reinterpret_cast<int>(&tmp[0]));
    m_changeBuffer->Insert(0, &tmp[0], len);

    unique_ptr<CFile> dst(new CFile(tmpPath, mode));

    dlg.m_source = m_changeBuffer.get();
    dlg.m_destination = dst.get();

    int result = dlg.DoModal(m_hWnd, 0);

    dst.reset();
    m_changeBuffer.reset();

    if (0 == result) {
        ::MoveFileEx(tmpPath, m_path, MOVEFILE_REPLACE_EXISTING);
        m_scintilla.Send(SCI_SETSAVEPOINT);
    } else {
        ::DeleteFile(tmpPath);
    }

    // TODO: Make it DRY
    CFile* file = new CFile(m_path, CFile::MODE_READ);
    unique_ptr<IDataStorage> storage(file);
    m_changeBuffer.reset(new CChangeBuffer(std::move(storage)));

    return true;
}

void CEditorCtrl::ResetScintilla()
{
    m_scintilla.Send(SCI_SETSCROLLWIDTH, 1);
    m_scintilla.Send(SCI_CLEARALL);
    m_scintilla.Send(SCI_EMPTYUNDOBUFFER);
    m_scintilla.Send(SCI_SETSAVEPOINT);
}
