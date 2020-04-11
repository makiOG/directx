#include "SceneManager.h"
#include"Scenes.h"

//���[�f�B���O�V�[������J�n
SceneManager::SceneManager() :currentScene_(SceneType::SCENE_GAMEPLAY) {
	//���p����V�[�����쐬���A�V�[�����X�g�ɒǉ�����
	scenes_[SceneType::SCENE_GAMEPLAY] = std::make_shared<GamePlayScene>();
}

void SceneManager::start() {
	scenes_[currentScene_]->start();
}

void SceneManager::Update(float deltaTime) {
	scenes_[currentScene_]->Update(deltaTime);

}

void SceneManager::Draw() {
	scenes_[currentScene_]->Draw();
	scenes_[currentScene_]->lateDraw();
}

void SceneManager::End()
{
	scenes_[currentScene_]->end();
}

//�V�[�����I�����Ă��邩���m�F���A�K�v�ł���Ύ��̃V�[���ɑJ�ڂ���

void SceneManager::checkIsEnd() {
	if (scenes_.at(currentScene_)->isEnd()) next();

}

//���̃V�[���֑J�ڂ���

void SceneManager::next() {
	//�V�[�����I������
	scenes_[currentScene_]->end();
	SceneType nextScene = scenes_[currentScene_]->nextScene();
	currentScene_ = nextScene;
	//���̃V�[�����J�n����
	scenes_[currentScene_]->baseInit();
	scenes_[currentScene_]->start();
}
