#pragma once
#include <Windows.h>

class Launcher {
public:
	//Launcher();
	//~Launcher();

	void Run();

private:
	void SetWindowPosition(HWND hwnd, int x, int y, int width, int height);

	HWND WaitForWindow(const wchar_t* windowName);

private:
	HWND busMonitor;
	HWND display;



};