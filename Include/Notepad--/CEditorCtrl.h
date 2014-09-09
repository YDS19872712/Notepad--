#ifndef CEDITORCTRL_H
#define CEDITORCTRL_H

#include <memory>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlscrl.h>
#include <Core/CChangeBuffer.h>

#define ECM_FILE_NEW     WM_USER + 1
#define ECM_FILE_OPEN    WM_USER + 2
#define ECM_FILE_SAVE    WM_USER + 3
#define ECM_FILE_SAVE_AS WM_USER + 4

class CEditorCtrl : public CScrollWindowImpl<CEditorCtrl>
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

    PCTSTR GetPath() const;

    bool PathIsEmpty() const;

    bool IsModified() const;

private:

    BEGIN_MSG_MAP(CEditorCtrl)

        MSG_WM_CREATE     (OnCreate)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_KEYDOWN    (OnKeyDown)

        MESSAGE_HANDLER(ECM_FILE_NEW,     OnFileNew)
        MESSAGE_HANDLER(ECM_FILE_OPEN,    OnFileOpen)
        MESSAGE_HANDLER(ECM_FILE_SAVE,    OnFileSave)
        MESSAGE_HANDLER(ECM_FILE_SAVE_AS, OnFileSaveAs)

        CHAIN_MSG_MAP(CScrollWindowImpl<CEditorCtrl>)

    END_MSG_MAP();

    LRESULT OnCreate(LPCREATESTRUCT);

    LRESULT OnLButtonDown(UINT, CPoint);

    LRESULT OnKeyDown(TCHAR, UINT, UINT);

    LRESULT OnFileNew(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileOpen(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileSave(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileSaveAs(UINT, WPARAM, LPARAM, BOOL&);

    void DoPaint(CDCHandle);

    void DoFileNew();

    bool DoFileOpen();

    void DoFileSave();

    TCHAR m_path[MAX_PATH];

    bool m_modified;

    std::unique_ptr<Core::CChangeBuffer> m_changeBuffer;
};

inline PCTSTR CEditorCtrl::GetPath() const
{
    return m_path;
}

inline bool CEditorCtrl::PathIsEmpty() const
{
    return 0 == ::lstrlen(m_path);
}

inline bool CEditorCtrl::IsModified() const
{
    return m_modified;
}

#endif // CEDITORCTRL_H
