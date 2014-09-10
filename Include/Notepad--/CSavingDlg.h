#ifndef CSAVINGDLG_H
#define CSAVINGDLG_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlcrack.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <Notepad--/resource.h>
#include <Core/CFile.h>
#include <Core/CChangeBuffer.h>

class CSavingDlg : public CDialogImpl<CSavingDlg>
{
public:

    enum { IDD = IDD_SAVING_DIALOG };

private:

    BEGIN_MSG_MAP(CSavingDlg)

        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY   (OnDestroy)
        MSG_WM_TIMER     (OnTimer)

    END_MSG_MAP()

    bool OnInitDialog(CWindow, LPARAM);

    void OnDestroy();

    void OnTimer(UINT_PTR);

    Core::CFile* m_destination;

    Core::CChangeBuffer* m_source;

    CProgressBarCtrl m_progress;

    unsigned long long m_bytesTotal;

    unsigned long long m_bytesWritten;
};

#endif // CSAVINGDLG_H
