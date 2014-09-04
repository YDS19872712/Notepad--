#include <windows.h>
#include <Notepad--/CMainFrame.h>

CAppModule _Module;

int __stdcall WinMain(HINSTANCE hinst, HINSTANCE, PSTR cmdLine, int cmdShow)
{
    _Module.Init(0, hinst, 0);

    CMainFrame frame;

    frame.Create(
        ::GetDesktopWindow(),
        CWindow::rcDefault,
        TEXT("Notepad--"));

    frame.ShowWindow(cmdShow);

    CMessageLoop msgLoop;

    int result = msgLoop.Run();

    _Module.Term();

    return result;
}
