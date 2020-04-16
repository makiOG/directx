#pragma once

#include <windows.h>
#pragma comment(lib,"winmm.lib")

class Window
{
public:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	void InitFps();
	void CalculationFps();
	void CalculationSleep();
	static HWND GethWnd();
	static double GetFps();
private:
	static HWND g_hWnd;
	static double g_dFps;
	LARGE_INTEGER Freq = { 0 };
	LARGE_INTEGER StartTime = { 0 };
	LARGE_INTEGER NowTime = { 0 };
	int iCount = 0;
	DWORD SleepTime = 0;
};