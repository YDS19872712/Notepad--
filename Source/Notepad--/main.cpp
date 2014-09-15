#include <windows.h>
#include <Notepad--/CMainFrame.h>

CAppModule _Module;

int __stdcall WinMain(HINSTANCE hinst, HINSTANCE, PSTR cmdLine, int cmdShow)
{
    _Module.Init(0, hinst, 0);

    CMessageLoop msgLoop;
    _Module.AddMessageLoop(&msgLoop);

    int result = -1;

    CMainFrame frame;

    frame.Create(
        ::GetDesktopWindow(),
        CWindow::rcDefault,
        TEXT("Notepad--"));

    if (frame.IsWindow()) {
        frame.ShowWindow(cmdShow);
        result = msgLoop.Run();
    }

    _Module.RemoveMessageLoop();
    _Module.Term();

    return result;
}
