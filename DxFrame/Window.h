#pragma once
#include <windows.h>

/**
*  HWND�̃��b�p�[�N���X
*/
class Window
{
public:
	/**
	*  �R���X�g���N�^
	*/
	Window(HWND hwnd);

public:
	/**
	*  �E�B���h�E������ꂽ��
	*/
	bool IsClosed()const { return ::IsWindow(hwnd) == FALSE; }

	HWND GetHWND()const;

private:
	HWND hwnd;
};