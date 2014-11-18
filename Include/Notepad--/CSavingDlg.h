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

/**
 * A dialog that displays a progess of saving.
 */
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

    /**
     * Destination file.
     */
    Core::CFile* m_destination;

    /**
     * Source change buffer that may be backed up by a file.
     */
    Core::CChangeBuffer* m_source;

    /**
     * A control which shows a progress of the writing operation.
     */
    CProgressBarCtrl m_progress;

    /**
     * A tracker used to check if the read buffer is ready.
     */
    std::unique_ptr<Core::ITracker> m_readTracker;

    /**
     * A tracker used to check if all the data are already written
     * from the write buffer.
     */
    std::unique_ptr<Core::ITracker> m_writeTracker;

    /**
     * A buffer that provides a place to store the data readed from
     * the source.
     */
    std::vector<BYTE> m_readBuffer;
    
    /**
     * A buffer which holds the data while they are being writing
     * to the destination.
     */
    std::vector<BYTE> m_writeBuffer;

    /**
     * Overall size of the source.
     */
    unsigned long long m_bytesTotal;

    /**
     * Number of bytes that are already written to the destination.
     */
    unsigned long long m_bytesWritten;

    /**
     * Number of bytes that are already read from the source.
     */
    unsigned long long m_bytesRead;
};

#endif // CSAVINGDLG_H
