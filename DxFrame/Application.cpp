#include "Application.h"
#include "Window.h"
#include <iostream>

#define WIN_STYLE WS_OVERLAPPEDWINDOW

/**
*  ���b�Z�[�W�L���[�ɂ���S�Ă�windows�C�x���g����������B
*
*  @return WM_QUIT���b�Z�[�W���󂯎�������ǂ���
*/
bool Application::DoEvents()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);        //�E�B���h�E�v���V�[�W���̌Ăяo��
	}
	else
	{
		//�P�t���[���̏������s�����߂ɃQ�[�����[�v�֏�����߂�
		return true;
	}
}


/**
*  �w��T�C�Y�̃N���C�A���g�̈�����A�Q�[���p�̒P���ȃE�B���h�E���쐬����
*  @param  title   �E�B���h�E�̃^�C�g�����w��
*  @param  width   �N���C�A���g�̈�̕����w��
*  @param  height  �N���C�A���g�̈�̍������w��
*  @return         ��������Window�N���X�ւ�std::shared_ptr��Ԃ��B���s����nullptr��Ԃ��B
*/
WindowPtr Application::CreateGameWindow(const std::string& title, int client_width, int client_height)
{
	/*
	const std::string class_name = "hoge";
	const DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	const DWORD exstyle = 0;
	const HINSTANCE instance = ::GetModuleHandle(nullptr);

	//�E�B���h�E�N���X�̓o�^
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

	//�N���C�A���g�T�C�Y����E�B���h�E�T�C�Y���v�Z
	RECT rect = { 0, 0, client_width, client_height };
	::AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	//�E�B���h�E����
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
	// �E�B���h�E�N���X��o�^����
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "main_window";
	RegisterClass(&wc);

	// �E�B���h�E�̍쐬
	WHandle = CreateWindow("main_window", title.c_str(), WIN_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, client_width, client_height, NULL, NULL, hInstance, NULL);
	if (WHandle == NULL) return 0;
	ShowWindow(WHandle, SW_SHOW);

	WindowPtr window(new Window(WHandle));

	//std::cout << "�E�B���h�E�𐶐�( " << title << "," << client_width << "," << client_height << " )" << std::endl;
	return window;
}