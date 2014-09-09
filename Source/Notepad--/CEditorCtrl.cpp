#include <Core/CFile.h>
#include <Core/CNullStorage.h>
#include <Notepad--/CEditorCtrl.h>

using namespace std;
using namespace Core;

LRESULT CEditorCtrl::OnCreate(LPCREATESTRUCT)
{
    m_modified = false;

    SetClassLongPtr(
        m_hWnd, GCLP_HCURSOR,
        reinterpret_cast<LONG_PTR>(
            ::LoadCursor(NULL, IDC_IBEAM)));

    DoFileNew();

    SetScrollSize(2000, 2000, FALSE, false);

    SetMsgHandled(false);
    return 0;
}

LRESULT CEditorCtrl::OnLButtonDown(UINT flags, CPoint point)
{
    SetFocus();
    return 0;
}

LRESULT CEditorCtrl::OnKeyDown(TCHAR, UINT, UINT)
{
    m_modified = true;
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
    DoFileSave();
    handled = true;
    return 0;
}

LRESULT CEditorCtrl::OnFileSaveAs(UINT, WPARAM, LPARAM lParam, BOOL& handled)
{
    PCTSTR path = reinterpret_cast<PCTSTR>(lParam);
    ::lstrcpyn(m_path, path, MAX_PATH);
    DoFileSave();
    handled = true;
    return 0;
}

void CEditorCtrl::DoPaint(CDCHandle dc)
{
    RECT rc;
    GetClientRect(&rc);
    rc.right >>= 1;
    rc.bottom >>= 1;
    dc.Rectangle(&rc);
}

void CEditorCtrl::DoFileNew()
{
    m_path[0] = 0;
    m_changeBuffer.reset(
        new CChangeBuffer(
            unique_ptr<IDataStorage>(
                new CNullStorage())));
    m_modified = false;
}

bool CEditorCtrl::DoFileOpen()
{
    CFile* file = new CFile(m_path, CFile::MODE_READ);
    unique_ptr<IDataStorage> storage(file);

    if (file->IsOpen()) {
        m_changeBuffer.reset(new CChangeBuffer(std::move(storage)));
        m_modified = false;
        return true;
    }

    MessageBox(
        TEXT("Cannot open the file!"),
        TEXT("Ouch!"),
        MB_ICONEXCLAMATION);

    return false;
}

void CEditorCtrl::DoFileSave()
{
    ;
}
