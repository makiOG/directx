#include "Application.h"
#include "Window.h"
#include"Game.h"

#pragma comment(linker,"/entry:mainCRTStartup")

/**
*  エントリーポイント
*/
int main()
{
	//メモリリークの検出を有効化
	//  アウトプットウィンドウにメモリリーク時の情報が出力される
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//ゲーム用ウィンドウを生成
	WindowPtr window = Application::CreateGameWindow("hoge", 1000, 800);
	if (!window)return 0;

	Game game(window);
	//ゲームを初期化して、ゲームループを開始する
	if (game.Start())
	{
		//ゲームループ
		//  ウィンドウが閉じられるまで１フレームの処理を繰り返す
		while (!window->IsClosed())
		{
			//１フレームの処理
			game.Update();

			game.Draw();

			game.frameEnd();

			//メッセージキューにある全てのwindowsイベントを処理する
			Application::DoEvents();
		}
	}
	game.End();
	return 0;
}