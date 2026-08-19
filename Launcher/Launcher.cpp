#include "Launcher.h"

#include <windows.h>
#include <shellapi.h>

void Launcher::SetWindowPosition(HWND hwnd, int x, int y, int width, int height) {
	SetWindowPos(hwnd, NULL, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

	if (hwnd == NULL) {
		MessageBoxW(NULL, L"Window not found!", L"Error", MB_OK | MB_ICONERROR);
	}
}

HWND Launcher::WaitForWindow(const wchar_t* title)
{
    HWND hwnd = NULL;

    while (hwnd == NULL)
    {
        hwnd = FindWindowW(NULL, title);
        Sleep(50);
    }

    return hwnd;
}

void Launcher::Run() {
    ShellExecuteW(NULL, L"open", L"FlightComputer.exe", NULL, NULL, SW_SHOW);
	Sleep(10);

    ShellExecuteW(NULL, L"open", L"GPS.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    ShellExecuteW(NULL, L"open", L"IRS.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    ShellExecuteW(NULL, L"open", L"ADC.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    ShellExecuteW(NULL, L"open", L"Radio.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    for (int i = 0;i < 4;i++) {
        ShellExecuteW(NULL, L"open", L"RemoteTerminal.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
    }
    
    ShellExecuteW(NULL, L"open", L"BUS.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    ShellExecuteW(NULL, L"open", L"Display.exe", NULL, NULL, SW_SHOW);
    Sleep(10);


    ShellExecuteW(NULL, L"open", L"BusMonitor.exe", NULL, NULL, SW_SHOW);
    Sleep(10);

    display = WaitForWindow(L"Display");
    SetWindowPosition(display, 0, 0, 500, 1080);

    busMonitor = WaitForWindow(L"BusMonitor");
    SetWindowPosition(busMonitor, 500, 0, 960, 1080);


}