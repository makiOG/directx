#pragma once
#include <memory>
#include"Graphics.h"
#include"WindowsPtr.h"
#include"SceneManager.h"

/**
*  Gameクラス
*/
class Game
{
	//  -------------------------------------------------------------------------------
	//  生成と破棄
	//  -------------------------------------------------------------------------------
public:
	/**
	*  コンストラクタ
	*/
	Game(const WindowWeakPtr& window);

	//  -------------------------------------------------------------------------------
	//  基本機能
	//  -------------------------------------------------------------------------------
public:
	/**
	*  ゲームの初期化処理を行う
	*/
	bool Start();
	/**
	*  ゲームの解放処理を行う
	*/
	void End();
	/**
	*  ゲーム１フレームの処理を行う
	*/
	void Update();
	//
	void Draw();

	void frameEnd();

private:
	WindowWeakPtr _Window;   ///<    既存ウィンドウを指すポインタ

	//シーン管理クラス
	SceneManager _SceneManager;
};