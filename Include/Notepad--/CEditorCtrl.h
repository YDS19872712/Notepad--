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

/**
 * An editor control.
 * Encapsulates Scintilla component.
 */
class CEditorCtrl
    : public CWindowImpl<CEditorCtrl>
    , public CIdleHandler
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

    /**
     * Gets the path of currently edited file.
     * @returns A pointer to zero-terminated string.
     */
    PCTSTR GetPath() const;

    /**
     * Checks whether current path is empty, which means
     * that no file is open.
     * @returns true if current path is empty.
     */
    bool PathIsEmpty() const;

    /**
     * Checks if current text was modified in the editor.
     * @returns true if text was modified.
     */
    bool IsModified() const;

    /**
     * Initializes Scintilla's window classes.
     */
    static void Init();

    /**
     * Sets the font of the editor.
     * @param fd A font dialog that has been already presented to a user.
     */
    void SetFont(CFontDialog& fd);

    /**
     * Gets the font used in the editor.
     * @param fd A font dialog not yet presented to a user.
     */
    void GetFont(CFontDialog& fd) const;

private:

    /**
     * Holds the pointers by which Scintilla's API
     * can be reached.
     */
    struct SScintilla
    {
        /**
         * A window handle to Scintilla's control.
         */
        CWindow m_wnd;

        /**
         * Internal pointer to Scintilla's instance.
         */
        void* m_ptr;

        /**
         * A pointer to the function by which
         * all API is provided.
         */
        int (* m_fn)(void*, int, int, int);

        /**
         * Mimics message sending to Scintilla control.
         * Actually no messages are sent, this is a direct
         * call to Scintilla API.
         */
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

    /**
     * Replaces curretly edited file with a blank.
     */
    void DoFileNew();

    /**
     * Opens a file whose path is stored in m_path.
     */
    bool DoFileOpen();

    /**
     * Saves a file by the path that is stored in m_path.
     */
    bool DoFileSave();

    /**
     * Resets Scintilla control's state.
     */
    void ResetScintilla();

    /**
     * A path to currently edited file.
     */
    TCHAR m_path[MAX_PATH];

    /**
     * A buffer that holds all the changes made to the file.
     */
    std::unique_ptr<Core::CChangeBuffer> m_changeBuffer;

    /**
     * An access point to Scintilla API.
     */
    mutable SScintilla m_scintilla;
 
    /**
     * Amount of bytes already read from the source file.
     */
    unsigned long long m_bytesRead;
    
    /**
     * Amount of bytes needed to be read.
     */
    unsigned long long m_bytesToRead;

    /**
     * Total file size.
     */
    unsigned long long m_bytesTotal;

    /**
     * A tracker used to check the readiness of reading operations.
     */
    std::unique_ptr<Core::ITracker> m_tracker;

    /**
     * A buffer to be used by reading operations.
     */
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
