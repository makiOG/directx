#pragma once
#include<map>
#include<memory>
#include"EnumIDs.h"
class Scene;

//�V�[���̊Ǘ����s���AGame�y�т��̔h���N���X�ŗ��p����
class SceneManager {
public:
	SceneManager();
	void start();
	void Update(float deltaTime);
	void Draw();

	void End();
	//�V�[�����I�����Ă��邩���m�F���A�K�v�ł���Ύ��̃V�[���ɑJ�ڂ���
	void checkIsEnd();
	//���̃V�[���֑J�ڂ���
	void next();


private:
	//�V�[���ꗗ
	std::map<SceneType, std::shared_ptr<Scene>> scenes_;
	//���݂̃V�[��
	SceneType currentScene_;
};