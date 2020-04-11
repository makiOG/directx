#include "Game.h"
#include <iostream>
#include"Window.h"

/**
*  コンストラクタ
*
*  @param  window  既存ウィンドウのweak_ptr
*/
Game::Game(const WindowWeakPtr& window)
	: _Window(window)
{
}

/**
*  ゲームの初期化処理を行う
*
*  @param  初期化成功時trueを、失敗時falseを返す
*/
bool Game::Start()
{
	Graphics::GetInstance().Start(_Window);
	_SceneManager.start();
	return true;    //初期化成功
}

/**
*  ゲームの解放処理を行う
*/
void Game::End()
{
	_SceneManager.End();
	Graphics::GetInstance().End();
}

/**
*  ゲーム１フレームの処理を行う
*/
void Game::Update()
{
	
	_SceneManager.Update(1.0f);
	Graphics::GetInstance().Update();
	
}
//描画
void Game::Draw()
{
	
	_SceneManager.Draw();
	Graphics::GetInstance().Draw();
	
}

void Game::frameEnd()
{
	_SceneManager.checkIsEnd();
}
