#include "Window.h"

/**
*  �R���X�g���N�^�B�����ς݂̃E�B���h�E�n���h����n���B
*/
Window::Window(HWND hwnd)
	: hwnd(hwnd)
{
}

HWND Window::GetHWND()const
{
	return hwnd;
}
