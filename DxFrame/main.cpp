#include "Application.h"
#include "Window.h"
#include"Game.h"

#pragma comment(linker,"/entry:mainCRTStartup")

/**
*  �G���g���[�|�C���g
*/
int main()
{
	//���������[�N�̌��o��L����
	//  �A�E�g�v�b�g�E�B���h�E�Ƀ��������[�N���̏�񂪏o�͂����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�Q�[���p�E�B���h�E�𐶐�
	WindowPtr window = Application::CreateGameWindow("hoge", 1000, 800);
	if (!window)return 0;

	Game game(window);
	//�Q�[�������������āA�Q�[�����[�v���J�n����
	if (game.Start())
	{
		//�Q�[�����[�v
		//  �E�B���h�E��������܂łP�t���[���̏������J��Ԃ�
		while (!window->IsClosed())
		{
			//�P�t���[���̏���
			game.Update();

			game.Draw();

			game.frameEnd();

			//���b�Z�[�W�L���[�ɂ���S�Ă�windows�C�x���g����������
			Application::DoEvents();
		}
	}
	game.End();
	return 0;
}