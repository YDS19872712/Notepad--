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
#define PRELOAD_SIZE        READ_BUFFER_SIZE * 30
#define BYTES_TO_READ_COEFF 0.5

extern CAppModule _Module;

void CEditorCtrl::Init()
{
    Scintilla_RegisterClasses(_Module.m_hInst);
}

bool CEditorCtrl::IsModified() const
{
    return m_scintilla.Send(SCI_GETMODIFY) != 0;
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
        if (pos > m_bytesToRead * BYTES_TO_READ_COEFF) {
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
        m_bytesRead   = 0;
        m_bytesTotal  = m_changeBuffer->GetSize();
        m_bytesToRead = min(PRELOAD_SIZE, m_bytesTotal);
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

    unique_ptr<CFile> dst(new CFile(tmpPath, CFile::MODE_WRITE));

    dlg.m_source = m_changeBuffer.get();
    dlg.m_destination = dst.get();

    dlg.DoModal(m_hWnd, 0);

    return true;
}

void CEditorCtrl::ResetScintilla()
{
    m_scintilla.Send(SCI_SETSCROLLWIDTH, 1);
    m_scintilla.Send(SCI_CLEARALL);
    m_scintilla.Send(SCI_EMPTYUNDOBUFFER);
    m_scintilla.Send(SCI_SETSAVEPOINT);
}
