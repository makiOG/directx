#pragma once
#include <memory>
#include"Graphics.h"
#include"WindowsPtr.h"
#include"SceneManager.h"

/**
*  Game�N���X
*/
class Game
{
	//  -------------------------------------------------------------------------------
	//  �����Ɣj��
	//  -------------------------------------------------------------------------------
public:
	/**
	*  �R���X�g���N�^
	*/
	Game(const WindowWeakPtr& window);

	//  -------------------------------------------------------------------------------
	//  ��{�@�\
	//  -------------------------------------------------------------------------------
public:
	/**
	*  �Q�[���̏������������s��
	*/
	bool Start();
	/**
	*  �Q�[���̉���������s��
	*/
	void End();
	/**
	*  �Q�[���P�t���[���̏������s��
	*/
	void Update();
	//
	void Draw();

	void frameEnd();

private:
	WindowWeakPtr _Window;   ///<    �����E�B���h�E���w���|�C���^

	//�V�[���Ǘ��N���X
	SceneManager _SceneManager;
};