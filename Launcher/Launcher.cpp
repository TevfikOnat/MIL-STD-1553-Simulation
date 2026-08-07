#include "Launcher.h"

#include <windows.h>
#include <shellapi.h>

void Launcher::Run() {
    ShellExecuteW(
        NULL,
        L"open",
        L"FlightComputer.exe",
        NULL,
        NULL,
        SW_SHOW);

    ShellExecuteW(NULL, L"open", L"GPS.exe", NULL, NULL, SW_SHOW);
    Sleep(100);

    ShellExecuteW(NULL, L"open", L"IRS.exe", NULL, NULL, SW_SHOW);
    Sleep(100);

    ShellExecuteW(NULL, L"open", L"ADC.exe", NULL, NULL, SW_SHOW);
    Sleep(100);

    ShellExecuteW(NULL, L"open", L"RadioAltimeter.exe", NULL, NULL, SW_SHOW);
    Sleep(100);

    ShellExecuteW(NULL, L"open", L"BusMonitor.exe", NULL, NULL, SW_SHOW);
}