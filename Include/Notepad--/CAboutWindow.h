#ifndef CABOUTWINDOW_H
#define CABOUTWINDOW_H

#include <atlbase.h>
#include <atlapp.h>
#include <atlwinx.h>
#include <atlcrack.h>
#include <atlscrl.h>

/**
 * A window that renders REAMDE.md inside itself.
 */
class CAboutWindow : public CScrollWindowImpl<CAboutWindow>
{
public:

    DECLARE_WND_CLASS_EX(NULL, 0, -1)

private:

    BEGIN_MSG_MAP(CAboutWindow)
 
        MSG_WM_CREATE    (OnCreate)
        MSG_WM_DESTROY   (OnDestroy)
        MSG_WM_CLOSE     (OnClose)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)

        CHAIN_MSG_MAP(CScrollWindowImpl<CAboutWindow>)

    END_MSG_MAP();

    /**
     * Handles the creation message by loading raw resource
     * which contains REAMDE.md
     */
    LRESULT OnCreate(LPCREATESTRUCT);

    /**
     * Handles the destroy message by unloading the resource
     * which contains README.md
     */
    void OnDestroy();

    /**
     * Processes the close message and destroys itself.
     */
    void OnClose();

    /**
     * Handles background erasement messages.
     * Is needed to correctly update window's content
     * when redrawing.
     */
    BOOL OnEraseBkgnd(CDCHandle);

    /**
     * Repaints window's content by drawing ASCII text
     * contained in README.md
     */
    void DoPaint(CDCHandle);

    /**
     * A handle to the resource that holds README.md's textual data.
     */
    HRSRC m_hResource;

    /**
     * Size of the resource.
     */
    int m_sizeOfResource;

    /**
     * A handle to the memory block with loaded resource.
     */
    HGLOBAL m_hGlobal;

    /**
     * A pointer to a memory block obtained by pseudo-locking m_hGlobal.
     */
    char* m_text;
};

#endif // CABOUTWINODOW_H
