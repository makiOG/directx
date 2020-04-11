#include "Window.h"

/**
*  コンストラクタ。生成済みのウィンドウハンドルを渡す。
*/
Window::Window(HWND hwnd)
	: hwnd(hwnd)
{
}

HWND Window::GetHWND()const
{
	return hwnd;
}
