#include "Application.h"
#include "Window.h"
#include <iostream>

#define WIN_STYLE WS_OVERLAPPEDWINDOW

/**
*  メッセージキューにある全てのwindowsイベントを処理する。
*
*  @return WM_QUITメッセージを受け取ったかどうか
*/
bool Application::DoEvents()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);        //ウィンドウプロシージャの呼び出し
	}
	else
	{
		//１フレームの処理を行うためにゲームループへ処理を戻す
		return true;
	}
}


/**
*  指定サイズのクライアント領域を持つ、ゲーム用の単純なウィンドウを作成する
*  @param  title   ウィンドウのタイトルを指定
*  @param  width   クライアント領域の幅を指定
*  @param  height  クライアント領域の高さを指定
*  @return         生成したWindowクラスへのstd::shared_ptrを返す。失敗時はnullptrを返す。
*/
WindowPtr Application::CreateGameWindow(const std::string& title, int client_width, int client_height)
{
	/*
	const std::string class_name = "hoge";
	const DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	const DWORD exstyle = 0;
	const HINSTANCE instance = ::GetModuleHandle(nullptr);

	//ウィンドウクラスの登録
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.hInstance = instance;
	wc.lpszClassName = class_name.c_str();
	wc.lpfnWndProc = DefWindowProc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hIcon = static_cast< HICON >(
		::LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
		);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = static_cast< HCURSOR >(
		::LoadImage(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
		);
	wc.hbrBackground = static_cast< HBRUSH >(
		::GetStockObject(WHITE_BRUSH)
		);
	if (!::RegisterClassEx(&wc))return nullptr;

	//クライアントサイズからウィンドウサイズを計算
	RECT rect = { 0, 0, client_width, client_height };
	::AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	//ウィンドウ生成
	HWND hwnd = ::CreateWindowEx(
		exstyle, class_name.c_str(), title.c_str(), style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, instance, nullptr
	);
	if (hwnd == nullptr)return nullptr;
	::ShowWindow(hwnd, SW_SHOW);
	*/
	HWND WHandle;
	const HINSTANCE hInstance = ::GetModuleHandle(nullptr);
	// ウィンドウクラスを登録する
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "main_window";
	RegisterClass(&wc);

	// ウィンドウの作成
	WHandle = CreateWindow("main_window", title.c_str(), WIN_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, client_width, client_height, NULL, NULL, hInstance, NULL);
	if (WHandle == NULL) return 0;
	ShowWindow(WHandle, SW_SHOW);

	WindowPtr window(new Window(WHandle));

	//std::cout << "ウィンドウを生成( " << title << "," << client_width << "," << client_height << " )" << std::endl;
	return window;
}