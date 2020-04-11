#pragma once
#include <windows.h>

/**
*  HWNDのラッパークラス
*/
class Window
{
public:
	/**
	*  コンストラクタ
	*/
	Window(HWND hwnd);

public:
	/**
	*  ウィンドウが閉じられたか
	*/
	bool IsClosed()const { return ::IsWindow(hwnd) == FALSE; }

	HWND GetHWND()const;

private:
	HWND hwnd;
};