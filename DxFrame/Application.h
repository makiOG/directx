#pragma once
#include <string>
#include <memory>

class Window;
typedef std::shared_ptr< Window > WindowPtr;

class Application
{
public:
	/**
	*  ���b�Z�[�W�L���[�ɂ���S�Ă�windows�C�x���g����������B
	*/
	static bool DoEvents();

	/**
	*  �w��T�C�Y�̃N���C�A���g�̈�����A�Q�[���p�̒P���ȃE�B���h�E���쐬����
	*/
	static WindowPtr CreateGameWindow(const std::string& title, int client_width, int client_height);

};