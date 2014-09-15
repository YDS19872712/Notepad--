#ifndef CSAVINGDLG_H
#define CSAVINGDLG_H

#include <memory>
#include <vector>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlcrack.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <Core/ITracker.h>
#include <Core/CFile.h>
#include <Core/CChangeBuffer.h>
#include <Notepad--/resource.h>

class CSavingDlg : public CDialogImpl<CSavingDlg>
{
public:

    enum { IDD = IDD_SAVING_DIALOG };

private:

    BEGIN_MSG_MAP(CSavingDlg)

        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY   (OnDestroy)
        MSG_WM_TIMER     (OnTimer)

        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)

    END_MSG_MAP()

    bool OnInitDialog(CWindow, LPARAM);

    void OnDestroy();

    void OnTimer(UINT_PTR);

    LRESULT OnCancel(WORD, WORD, HWND, BOOL&);

    Core::CFile* m_destination;

    Core::CChangeBuffer* m_source;

    CProgressBarCtrl m_progress;

    std::unique_ptr<Core::ITracker> m_readTracker, m_writeTracker;

    std::vector<BYTE> m_readBuffer, m_writeBuffer;

    unsigned long long m_bytesTotal;

    unsigned long long m_bytesWritten;

    unsigned long long m_bytesRead;
};

#endif // CSAVINGDLG_H
