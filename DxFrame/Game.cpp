#include "Game.h"
#include <iostream>
#include"Window.h"

/**
*  �R���X�g���N�^
*
*  @param  window  �����E�B���h�E��weak_ptr
*/
Game::Game(const WindowWeakPtr& window)
	: _Window(window)
{
}

/**
*  �Q�[���̏������������s��
*
*  @param  ������������true���A���s��false��Ԃ�
*/
bool Game::Start()
{
	Graphics::GetInstance().Start(_Window);
	_SceneManager.start();
	return true;    //����������
}

/**
*  �Q�[���̉���������s��
*/
void Game::End()
{
	_SceneManager.End();
	Graphics::GetInstance().End();
}

/**
*  �Q�[���P�t���[���̏������s��
*/
void Game::Update()
{
	
	_SceneManager.Update(1.0f);
	Graphics::GetInstance().Update();
	
}
//�`��
void Game::Draw()
{
	
	_SceneManager.Draw();
	Graphics::GetInstance().Draw();
	
}

void Game::frameEnd()
{
	_SceneManager.checkIsEnd();
}
