#ifndef CEDITORCTRL_H
#define CEDITORCTRL_H

#include <memory>
#include <vector>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <Scintilla.h>
#include <Core/CChangeBuffer.h>

#define ECM_FILE_NEW        WM_USER + 1
#define ECM_FILE_OPEN       WM_USER + 2
#define ECM_FILE_SAVE       WM_USER + 3
#define ECM_FILE_SAVE_AS    WM_USER + 4
#define ECM_FORMAT_WORDWRAP WM_USER + 5

#define SCINTILLA_CTRL_ID 1001

class CEditorCtrl
    : public CWindowImpl<CEditorCtrl>
    , public CIdleHandler
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

    PCTSTR GetPath() const;

    bool PathIsEmpty() const;

    bool IsModified() const;

    static void Init();

private:

    struct SScintilla
    {
        CWindow m_wnd;
        void* m_ptr;
        int (* m_fn)(void*, int, int, int);

        int Send(int, int = 0, int = 0);
    };

    BEGIN_MSG_MAP(CEditorCtrl)

        MSG_WM_CREATE (OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE   (OnSize)

        MESSAGE_HANDLER(ECM_FILE_NEW,        OnFileNew)
        MESSAGE_HANDLER(ECM_FILE_OPEN,       OnFileOpen)
        MESSAGE_HANDLER(ECM_FILE_SAVE,       OnFileSave)
        MESSAGE_HANDLER(ECM_FILE_SAVE_AS,    OnFileSaveAs)
        MESSAGE_HANDLER(ECM_FORMAT_WORDWRAP, OnFormatWordWrap)

        NOTIFY_HANDLER(SCINTILLA_CTRL_ID, SCN_UPDATEUI, OnScintillaUpdateUI)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    void OnDestroy();

    LRESULT OnSize(UINT, CSize);

    LRESULT OnFileNew(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileOpen(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileSave(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileSaveAs(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFormatWordWrap(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnScintillaUpdateUI(int, NMHDR*, BOOL&);

    BOOL OnIdle();

    void DoFileNew();

    bool DoFileOpen();

    bool DoFileSave();

    void ResetScintilla();

    TCHAR m_path[MAX_PATH];

    std::unique_ptr<Core::CChangeBuffer> m_changeBuffer;

    mutable SScintilla m_scintilla;

    unsigned long long m_bytesRead, m_bytesToRead, m_bytesTotal;

    std::unique_ptr<Core::ITracker> m_tracker;

    std::vector<BYTE> m_buffer;
};

inline PCTSTR CEditorCtrl::GetPath() const
{
    return m_path;
}

inline bool CEditorCtrl::PathIsEmpty() const
{
    return 0 == ::lstrlen(m_path);
}

inline int CEditorCtrl::SScintilla::Send(int message, int first, int second)
{
    return m_fn(m_ptr, message, first, second);
}

#endif // CEDITORCTRL_H
