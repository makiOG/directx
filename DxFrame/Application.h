#pragma once
#include <string>
#include <memory>

class Window;
typedef std::shared_ptr< Window > WindowPtr;

class Application
{
public:
	/**
	*  メッセージキューにある全てのwindowsイベントを処理する。
	*/
	static bool DoEvents();

	/**
	*  指定サイズのクライアント領域を持つ、ゲーム用の単純なウィンドウを作成する
	*/
	static WindowPtr CreateGameWindow(const std::string& title, int client_width, int client_height);

};